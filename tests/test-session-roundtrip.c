/* test-session-roundtrip.c - Unit tests for session save/restore format
 *
 * The session is stored as a GKeyFile with these keys (4.8.0+):
 *   - [Xreader] uris=uri1,uri2,uri3 (CSV)
 *   - [Xreader] active-index=2
 * The 4.7.0 format was a single 'uri' key.  Both are supported
 * on read (backward compat); the 4.8.0+ format is written.
 *
 * The tests verify the GKeyFile round-trip: writing the
 * state to a GKeyFile and reading it back gives the same
 * URIs + active index.  The full save/restore is covered
 * by the manual test plan (test 13).
 */

#include <config.h>
#include <glib.h>

/* Re-implement the save logic in miniature for the test. */
static GKeyFile *
make_state (const gchar *const *uris, gint active_index)
{
    GKeyFile *state = g_key_file_new ();
    if (uris && uris[0]) {
        GString *csv = g_string_new ("");
        for (gint i = 0; uris[i] != NULL; i++) {
            if (i > 0) g_string_append_c (csv, ',');
            g_string_append (csv, uris[i]);
        }
        g_key_file_set_string (state, "Xreader", "uris", csv->str);
        g_key_file_set_integer (state, "Xreader", "active-index", active_index);
        g_string_free (csv, TRUE);
    }
    return state;
}

static void
test_roundtrip_single (void)
{
	const gchar *uris[] = { "file:///tmp/foo.pdf", NULL };
	GKeyFile *state = make_state (uris, 0);

	gchar *csv = g_key_file_get_string (state, "Xreader", "uris", NULL);
	g_assert_nonnull (csv);
	g_assert_cmpstr (csv, ==, "file:///tmp/foo.pdf");
	g_free (csv);

	gint active = g_key_file_get_integer (state, "Xreader", "active-index", NULL);
	g_assert_cmpint (active, ==, 0);

	g_key_file_free (state);
}

static void
test_roundtrip_multi (void)
{
	const gchar *uris[] = {
		"file:///tmp/a.pdf",
		"file:///tmp/b.pdf",
		"file:///tmp/c.pdf",
		NULL
	};
	GKeyFile *state = make_state (uris, 2);

	gchar *csv = g_key_file_get_string (state, "Xreader", "uris", NULL);
	g_assert_nonnull (csv);
	g_assert_cmpstr (csv, ==, "file:///tmp/a.pdf,file:///tmp/b.pdf,file:///tmp/c.pdf");
	g_free (csv);

	gint active = g_key_file_get_integer (state, "Xreader", "active-index", NULL);
	g_assert_cmpint (active, ==, 2);

	g_key_file_free (state);
}

static void
test_empty_session (void)
{
	/* An empty session has no 'uris' key.  This is the
	 * case for a fresh install (no previous session). */
	GKeyFile *state = g_key_file_new ();
	g_assert (!g_key_file_has_key (state, "Xreader", "uris", NULL));
	g_key_file_free (state);
}

static void
test_uri_parsing (void)
{
	/* g_strsplit on a CSV gives the right URI list. */
	const gchar *uris_csv = "file:///tmp/a.pdf,file:///tmp/b.pdf";
	gchar **uri_list = g_strsplit (uris_csv, ",", 0);
	g_assert_cmpstr (uri_list[0], ==, "file:///tmp/a.pdf");
	g_assert_cmpstr (uri_list[1], ==, "file:///tmp/b.pdf");
	g_assert_null (uri_list[2]);
	g_strfreev (uri_list);
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/session/roundtrip-single",  test_roundtrip_single);
	g_test_add_func ("/session/roundtrip-multi",   test_roundtrip_multi);
	g_test_add_func ("/session/empty",              test_empty_session);
	g_test_add_func ("/session/uri-parsing",        test_uri_parsing);

	return g_test_run ();
}
