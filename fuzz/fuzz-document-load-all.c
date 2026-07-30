/* fuzz-document-load-all.c - libFuzzer harness for the xreader document
 *                            factory across ALL backends.
 *
 * Complements fuzz/fuzz-document-load.c (PR #13) which feeds random
 * bytes as a single un-named file.  The existing fuzzer relies on
 * the content-based MIME detection to dispatch to the right backend,
 * which means the fuzzer is biased toward whatever content pattern
 * libmagic happens to match.  In practice, libFuzzer converges on
 * a few "magic byte" prefixes (e.g. %PDF, PK) and rarely exercises
 * the DJVU / DVI / XPS / comics / ODF / tiff / cbz code paths.
 *
 * This harness instead writes the fuzz input to a temp file with
 * a sequence of well-known extensions, one iteration each, so the
 * MIME dispatcher in ev_document_factory_get_document is forced to
 * take the extension-based fast path (or, for the .bin case, the
 * slow content-based path) for every backend:
 *
 *   .pdf   -> poppler backend
 *   .djvu  -> djvu backend
 *   .dvi   -> dvi backend
 *   .xps   -> gxps backend
 *   .oxps  -> gxps backend
 *   .epub  -> epub backend
 *   .tiff  -> tiff backend
 *   .tif   -> tiff backend
 *   .cbz   -> comics backend
 *   .cbr   -> comics backend
 *   .odt   -> odf backend
 *   .ods   -> odf backend
 *   .odp   -> odf backend
 *   .bin   -> content-based dispatch (typically pixbuf)
 *
 * For each extension, the harness writes the same fuzz input to a
 * new temp file, calls ev_document_factory_get_document on it,
 * unrefs the result, and removes the temp file.  The total number
 * of iterations per LLVMFuzzerTestOneInput is N_EXTENSIONS (one
 * per backend family).
 *
 * The output of a good fuzzer run is a coverage map that hits all
 * 8 backends; the existing single-extension fuzzer typically only
 * hits 2-3.
 *
 * The harness reuses the same temp-file + URI pattern as the
 * existing fuzzer, so the CI fuzz job (PR #19) will pick this up
 * automatically once the executable is added to fuzz/meson.build.
 */

#include <config.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "ev-document-factory.h"
#include "ev-init.h"

static const char *BACKEND_EXTENSIONS[] = {
	".pdf", ".djvu", ".dvi", ".xps", ".oxps",
	".epub", ".tiff", ".tif",  ".cbz", ".cbr",
	".odt",  ".ods",  ".odp", ".bin",
};
#define N_EXTENSIONS (sizeof (BACKEND_EXTENSIONS) / sizeof (BACKEND_EXTENSIONS[0]))

static gchar *tmp_dir = NULL;

static void
ensure_tmp_dir (void)
{
	if (tmp_dir != NULL)
		return;
	tmp_dir = g_dir_make_tmp ("xreader-fuzz-all-XXXXXX", NULL);
	g_return_if_fail (tmp_dir != NULL);
}

static gchar *
write_input_to (const char *suffix, const guint8 *data, size_t size)
{
	gchar *path = g_build_filename (tmp_dir, "input", suffix, NULL);
	gint fd = g_open (path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0) {
		g_free (path);
		return NULL;
	}
	if (data != NULL && size > 0) {
		gsize written = 0;
		while (written < size) {
			gssize n = write (fd, data + written, size - written);
			if (n <= 0) {
				close (fd);
				g_unlink (path);
				g_free (path);
				return NULL;
			}
			written += (gsize) n;
		}
	}
	close (fd);
	return path;
}

int
LLVMFuzzerInitialize (int *argc, char ***argv)
{
	g_type_init ();
	ev_init ();
	ensure_tmp_dir ();
	return 0;
}

int
LLVMFuzzerTestOneInput (const guint8 *data, size_t size)
{
	if (size == 0)
		return 0;

	for (gsize i = 0; i < N_EXTENSIONS; i++) {
		const char *ext = BACKEND_EXTENSIONS[i];
		gchar *path = write_input_to (ext, data, size);
		if (path == NULL)
			continue;

		gchar *uri = g_filename_to_uri (path, NULL, NULL);
		if (uri != NULL) {
			GError *error = NULL;
			EvDocument *doc = ev_document_factory_get_document (uri, &error);
			if (doc != NULL)
				g_object_unref (doc);
			g_clear_error (&error);
			g_free (uri);
		}

		g_unlink (path);
		g_free (path);
	}

	return 0;
}
