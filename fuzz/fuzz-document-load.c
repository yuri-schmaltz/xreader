/* fuzz-document-load.c - libFuzzer harness for ev_document_factory_get_document
 *
 * Feeds random bytes as a document into the xreader document factory, which
 * dispatches to the appropriate backend (poppler / djvu / dvi / pixbuf /
 * comics / xps / epub / tiff) based on the file content.  The harness does
 * not care about the document type or the resulting EvDocument; it only
 * cares that the backend does not crash, leak, or assert on a malformed
 * input.
 *
 * Build:
 *   CC=clang meson setup build \
 *     -Db_sanitize=address,undefined \
 *     -Dcpp_args='-fsanitize=fuzzer-no-link' \
 *     -Dc_args='-fsanitize=fuzzer-no-link'
 *   meson compile -C build fuzz-document-load
 *
 * Run with a seed corpus:
 *   mkdir -p fuzz-corpus
 *   cp test/test-*.pdf test/*.epub test/testBookmarksMenu.py \
 *      fuzz-corpus/   # any test document works as a seed
 *   ./build/fuzz-document-load fuzz-corpus/ -max_len=1048576
 *
 * Or run oss-fuzz style with a curated corpus.  See fuzz/Makefile for the
 * exact integration commands.
 *
 * The harness writes the fuzz input to a temp file, gets a file:// URI,
 * calls ev_document_factory_get_document on it, unrefs the result, and
 * removes the temp file.  GErrors are deliberately ignored (the harness
 * is not asserting on correctness, only on absence of crashes).
 */

#include <config.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ev-document-factory.h"
#include "ev-init.h"

static gchar *tmp_template_path = NULL;

static void
ensure_tmp_template (void)
{
	if (tmp_template_path != NULL)
		return;
	gchar *tmpdir = g_dir_make_tmp ("xreader-fuzz-XXXXXX", NULL);
	g_return_if_fail (tmpdir != NULL);
	tmp_template_path = g_build_filename (tmpdir, "doc-XXXXXX", NULL);
	g_free (tmpdir);
}

static gboolean
write_fuzz_input (const guint8 *data, size_t size, GError **error)
{
	gchar *path = g_strdup (tmp_template_path);
	gint fd = g_mkstemp (path);
	if (fd < 0) {
		g_set_error (error, G_FILE_ERROR, g_file_error_from_errno (errno),
		             "g_mkstemp failed: %s", g_strerror (errno));
		g_free (path);
		return FALSE;
	}

	gsize written = 0;
	while (written < size) {
		gssize n = write (fd, data + written, size - written);
		if (n <= 0) {
			if (errno == EINTR)
				continue;
			g_set_error (error, G_FILE_ERROR, g_file_error_from_errno (errno),
			             "write failed: %s", g_strerror (errno));
			close (fd);
			g_unlink (path);
			g_free (path);
			return FALSE;
		}
		written += (gsize) n;
	}
	close (fd);

	/* Stash the path on the caller side via static; simpler than threading
	 * the path through g_mkstemp's gpointer. */
	g_free (tmp_template_path);
	tmp_template_path = path;
	return TRUE;
}

int
LLVMFuzzerInitialize (int *argc, char ***argv)
{
	g_type_init ();
	ev_init ();
	ensure_tmp_template ();
	return 0;
}

int
LLVMFuzzerTestOneInput (const guint8 *data, size_t size)
{
	GError *error = NULL;

	if (size == 0)
		return 0;

	/* Reset the temp file for this iteration. */
	if (tmp_template_path != NULL)
		g_unlink (tmp_template_path);
	if (!write_fuzz_input (data, size, &error)) {
		g_clear_error (&error);
		return 0;
	}

	gchar *uri = g_filename_to_uri (tmp_template_path, NULL, &error);
	if (uri == NULL) {
		g_clear_error (&error);
		return 0;
	}

	EvDocument *doc = ev_document_factory_get_document (uri, &error);
	if (doc != NULL)
		g_object_unref (doc);
	g_clear_error (&error);

	g_free (uri);
	g_unlink (tmp_template_path);
	return 0;
}
