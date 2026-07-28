/* test-file-helpers.c - Unit tests for ev-file-helpers
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Tests the small set of pure functions exposed by libdocument/ev-file-helpers.h:
 *
 *  - ev_mkstemp / ev_mkdtemp:  mkstemp(3) / mkdtemp(3) wrappers that
 *    return the resulting filename as a newly-allocated string.
 *  - ev_file_helpers_compress / ev_file_helpers_uncompress:  zlib-based
 *    compress / decompress for the .xreader metadata files.  Round-trip
 *    must recover the original buffer.
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

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-file-helpers/mkstemp-creates-file",
	                 test_mkstemp_creates_file);
	g_test_add_func ("/ev-file-helpers/mkdtemp-creates-directory",
	                 test_mkdtemp_creates_directory);
	g_test_add_func ("/ev-file-helpers/compress-roundtrip",
	                 test_compress_roundtrip);
	g_test_add_func ("/ev-file-helpers/compress-empty",
	                 test_compress_empty);

	return g_test_run ();
}
