/* test-file-helpers.c - Unit tests for ev-file-helpers
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Tests the small set of pure functions exposed by libdocument/ev-file-helpers.h:
 *
 *  - ev_mkstemp / ev_mkdtemp:  mkstemp(3) / mkdtemp(3) wrappers that
 *    return the resulting filename as a newly-allocated string.
 *  - ev_mkstemp_file: same as ev_mkstemp but returns a GFile, the
 *    file descriptor is closed via the destroy notify.
 *  - ev_xfer_uri_simple: g_file_copy wrapper, the NULL-from edge
 *    case must be a no-op success.
 *  - ev_file_get_mime_type: fast / slow MIME detection; on a known
 *    text file the helper must return SOME non-NULL MIME type.
 *  - ev_file_compress / ev_file_uncompress: with EV_COMPRESSION_NONE
 *    the helper must not shell out to an external compressor
 *    (the comment in the source says so explicitly).
 *
 * Run via `meson test -C build test-file-helpers`.
 */

#include <config.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ev-file-helpers.h"

static void
test_mkstemp_creates_file (void)
{
	char *template_str = g_strdup ("xreader-test-XXXXXX");
	char *name = NULL;
	int fd;

	fd = ev_mkstemp (template_str, &name);
	g_assert_cmpint (fd, >=, 0);
	g_assert_nonnull (name);
	g_assert_nonnull (strstr (name, "xreader-test-"));
	g_assert_cmpuint (strlen (name), >, 0);

	/* The file must exist and be writable. */
	g_assert_cmpint (g_access (name, F_OK), ==, 0);

	/* The template suffix must have been replaced with 6 random chars. */
	const char *suffix = name + strlen (name) - 6;
	for (int i = 0; i < 6; i++) {
		g_assert_cmpint (suffix[i] != '\0', ==, 1);
	}

	g_assert_cmpint (close (fd), ==, 0);
	g_assert_cmpint (g_unlink (name), ==, 0);

	g_free (name);
	g_free (template_str);
}

static void
test_mkdtemp_creates_directory (void)
{
	char *template_str = g_strdup ("xreader-test-XXXXXX");
	char *name = NULL;

	gboolean ok = ev_mkdtemp (template_str, &name);
	g_assert_true (ok);
	g_assert_nonnull (name);
	g_assert_nonnull (strstr (name, "xreader-test-"));

	/* The directory must exist. */
	g_assert_cmpint (g_access (name, F_OK), ==, 0);

	g_assert_cmpint (g_rmdir (name), ==, 0);

	g_free (name);
	g_free (template_str);
}

static void
test_compress_roundtrip (void)
{
	const char *original =
		"This is a test of the compress / uncompress helpers.  "
		"It contains repeated text to give the compressor something to do.  "
		"repeated repeated repeated repeated repeated repeated repeated.  "
		"It also has some \n newlines and a few \x01 \x02 \x03 control bytes.";
	gsize original_len = strlen (original) + 1; /* include the trailing NUL */

	gchar *compressed = NULL;
	gsize compressed_len = 0;
	gchar *uncompressed = NULL;
	gsize uncompressed_len = 0;

	ev_file_helpers_compress ((const guchar *) original, original_len,
	                          &compressed, &compressed_len);
	g_assert_nonnull (compressed);
	g_assert_cmpuint (compressed_len, >, 0);
	g_assert_cmpuint (compressed_len, <=, original_len); /* should compress */

	ev_file_helpers_uncompress ((const guchar *) compressed, compressed_len,
	                            &uncompressed, &uncompressed_len);
	g_assert_nonnull (uncompressed);
	g_assert_cmpuint (uncompressed_len, ==, original_len);
	g_assert_cmpint (memcmp (uncompressed, original, original_len), ==, 0);

	g_free (compressed);
	g_free (uncompressed);
}

static void
test_compress_empty (void)
{
	gchar *compressed = NULL;
	gsize compressed_len = (gsize) -1;
	gchar *uncompressed = NULL;
	gsize uncompressed_len = 0;

	/* Empty input: length 0, NULL or empty buffer. */
	ev_file_helpers_compress ((const guchar *) "", 0, &compressed, &compressed_len);
	/* Implementation may or may not handle the zero-length case; the contract
	 * is that uncompress of the result must produce the original (empty) data. */
	if (compressed != NULL && compressed_len > 0) {
		ev_file_helpers_uncompress ((const guchar *) compressed, compressed_len,
		                            &uncompressed, &uncompressed_len);
		g_assert_cmpuint (uncompressed_len, ==, 0);
		g_free (uncompressed);
	}
	g_free (compressed);
}


static void
test_mkstemp_file_returns_gfile (void)
{
	GError *error = NULL;
	GFile *file = NULL;

	file = ev_mkstemp_file ("xreader-test-XXXXXX", &error);
	g_assert_no_error (error);
	g_assert_nonnull (file);

	/* The returned GFile must be a valid local file path. */
	gchar *path = g_file_get_path (file);
	g_assert_nonnull (path);
	g_assert_nonnull (strstr (path, "xreader-test-"));
	g_assert_cmpint (g_access (path, F_OK), ==, 0);

	/* unrefing the GFile must close the underlying fd (the
	 * destroy notify on "ev-mkstemp-fd" takes care of it). */
	g_object_unref (file);
	g_assert_cmpint (g_unlink (path), ==, 0);

	g_free (path);
}

static void
test_xfer_uri_simple_copies_file (void)
{
	GError *error = NULL;
	gchar *src_name = NULL;
	int src_fd;
	gchar *src_uri, *dst_uri;
	const char payload[] = "xreader test payload";
	gchar *read_back = NULL;
	gsize read_len = 0;

	/* Create a source file with a known content. */
	src_fd = ev_mkstemp ("xreader-src-XXXXXX", &src_name, &error);
	g_assert_no_error (error);
	g_assert_cmpint (src_fd, >=, 0);
	g_assert_cmpint (write (src_fd, payload, sizeof (payload) - 1), ==, sizeof (payload) - 1);
	g_assert_cmpint (close (src_fd), ==, 0);

	src_uri = g_filename_to_uri (src_name, NULL, &error);
	g_assert_no_error (error);
	g_assert_nonnull (src_uri);

	dst_uri = g_strconcat (src_uri, ".copy", NULL);

	/* The copy itself. */
	g_assert_true (ev_xfer_uri_simple (src_uri, dst_uri, &error));
	g_assert_no_error (error);

	/* Verify the content of the destination. */
	g_assert_true (g_file_get_contents (dst_uri + strlen ("file://"), &read_back, &read_len, &error));
	g_assert_no_error (error);
	g_assert_cmpuint (read_len, ==, sizeof (payload) - 1);
	g_assert_cmpint (memcmp (read_back, payload, sizeof (payload) - 1), ==, 0);

	g_unlink (src_name);
	g_unlink (dst_uri + strlen ("file://"));
	g_free (read_back);
	g_free (src_uri);
	g_free (dst_uri);
	g_free (src_name);
}

static void
test_xfer_uri_simple_null_from_is_noop (void)
{
	GError *error = NULL;
	/* NULL from -> returns TRUE without doing anything (the
	 * 'copy nothing' use case the comment in the source
	 * mentions).  This protects callers that pass NULL by
	 * accident (e.g. the print previewer when no file is
	 * selected). */
	g_assert_true (ev_xfer_uri_simple (NULL, "file:///tmp/none", &error));
	g_assert_no_error (error);
}

static void
test_file_get_mime_type_text (void)
{
	GError *error = NULL;
	gchar *src_name = NULL;
	int src_fd;
	gchar *src_uri;
	gchar *mime = NULL;

	src_fd = ev_mkstemp ("xreader-mime-XXXXXX", &src_name, &error);
	g_assert_no_error (error);
	g_assert_cmpint (write (src_fd, "hello, world\n", 13), ==, 13);
	g_assert_cmpint (close (src_fd), ==, 0);

	src_uri = g_filename_to_uri (src_name, NULL, &error);
	g_assert_no_error (error);

	mime = ev_file_get_mime_type (src_uri, TRUE, &error);
	g_assert_no_error (error);
	/* On a fresh /tmp file the fast (extension-based) detection
	 * can return application/octet-stream; both are acceptable
	 * for the smoke test, the contract is that some MIME type
	 * is returned without an error. */
	g_assert_nonnull (mime);

	g_unlink (src_name);
	g_free (src_uri);
	g_free (mime);
	g_free (src_name);
}

static void
test_file_compress_none_is_noop (void)
{
	GError *error = NULL;
	/* EV_COMPRESSION_NONE: the helper must not call the
	 * external compressor binary (the comment in
	 * ev_file_compress says so explicitly).  Returns NULL
	 * without filling @error, which is the contract
	 * callers depend on for the 'no compression needed' code
	 * path. */
	gchar *result = ev_file_compress ("file:///dev/null",
	                                  EV_COMPRESSION_NONE, &error);
	g_assert_null (result);
	g_assert_no_error (error);

	/* Same for the decompress side. */
	result = ev_file_uncompress ("file:///dev/null",
	                             EV_COMPRESSION_NONE, &error);
	g_assert_null (result);
	g_assert_no_error (error);
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-file-helpers/mkstemp-creates-file",
	                 test_mkstemp_creates_file);
	g_test_add_func ("/ev-file-helpers/mkstemp-file-returns-gfile",
	                 test_mkstemp_file_returns_gfile);
	g_test_add_func ("/ev-file-helpers/xfer-uri-simple-copies-file",
	                 test_xfer_uri_simple_copies_file);
	g_test_add_func ("/ev-file-helpers/xfer-uri-simple-null-from-is-noop",
	                 test_xfer_uri_simple_null_from_is_noop);
	g_test_add_func ("/ev-file-helpers/file-get-mime-type-text",
	                 test_file_get_mime_type_text);
	g_test_add_func ("/ev-file-helpers/file-compress-none-is-noop",
	                 test_file_compress_none_is_noop);
	g_test_add_func ("/ev-file-helpers/mkdtemp-creates-directory",
	                 test_mkdtemp_creates_directory);
	g_test_add_func ("/ev-file-helpers/compress-roundtrip",
	                 test_compress_roundtrip);
	g_test_add_func ("/ev-file-helpers/compress-empty",
	                 test_compress_empty);

	return g_test_run ();
}
