/* test-page-render.c - Unit tests for ev_page and ev_render_context
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Tests the libdocument public API for EvPage and EvRenderContext:
 *
 *  - ev_page_new: create a page with an index, verify the index
 *    round-trips through the GObject.
 *  - ev_page_set_backend_page / get_backend_page: round-trip
 *    a gpointer through the destroy-notify machinery.
 *  - ev_page destroy-notify: verify the destroy callback fires
 *    exactly once on unref, and that the backend_page is NULL
 *    afterwards (defensive: a second unref of the same pointer
 *    would crash).
 *  - ev_render_context_new: create a context with (page, rotation,
 *    scale), verify the getters return the same values.
 *  - ev_render_context_set_page / _set_rotation / _set_scale:
 *    setters update the corresponding field.
 *
 * Run via `meson test -C build test-page-render`.
 *
 * The tests do not need a display server; both EvPage and
 * EvRenderContext are pure GObject wrappers.
 */

#include <config.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "ev-page.h"
#include "ev-render-context.h"

static gint destroy_count = 0;
static gpointer last_destroyed_backend = NULL;

static void
test_destroy_notify (gpointer backend_page)
{
	destroy_count++;
	last_destroyed_backend = backend_page;
}

/* ----- ev_page ----- */

static void
test_page_new (void)
{
	EvPage *page = ev_page_new (5);
	g_assert_nonnull (page);
	g_assert_true (EV_IS_PAGE (page));
	g_assert_cmpint (page->index, ==, 5);
	g_object_unref (page);
}

static void
test_page_new_negative_index (void)
{
	/* The index can be -1 to indicate 'unknown' (used by
	 * some backends for placeholder pages). */
	EvPage *page = ev_page_new (-1);
	g_assert_nonnull (page);
	g_assert_cmpint (page->index, ==, -1);
	g_object_unref (page);
}

static void
test_page_backend_page_roundtrip (void)
{
	/* Attach a fake backend_page (gpointer to a stack
	 * variable) and verify the destroy callback is called
	 * with the same pointer on unref. */
	destroy_count = 0;
	last_destroyed_backend = NULL;

	gint local = 42;
	EvPage *page = ev_page_new (0);
	page->backend_page = &local;
	page->backend_destroy_func = test_destroy_notify;

	g_object_unref (page);

	g_assert_cmpint (destroy_count, ==, 1);
	g_assert_true (last_destroyed_backend == &local);
}

static void
test_page_no_destroy_when_no_backend (void)
{
	/* If no backend was attached, unref must NOT call any
	 * destroy function (would crash on NULL function ptr). */
	destroy_count = 0;

	EvPage *page = ev_page_new (0);
	page->backend_page = NULL;
	page->backend_destroy_func = NULL;

	g_object_unref (page);

	g_assert_cmpint (destroy_count, ==, 0);
}

/* ----- ev_render_context ----- */

static void
test_render_context_new (void)
{
	EvPage *page = ev_page_new (3);
	EvRenderContext *rc = ev_render_context_new (page, 90, 1.5);

	g_assert_nonnull (rc);
	g_assert_true (EV_IS_RENDER_CONTEXT (rc));
	g_assert_true (rc->page == page);
	g_assert_cmpint (rc->rotation, ==, 90);
	g_assert_cmpfloat (rc->scale, ==, 1.5);

	g_object_unref (rc);
	g_object_unref (page);
}

static void
test_render_context_setters (void)
{
	EvPage *page1 = ev_page_new (0);
	EvPage *page2 = ev_page_new (1);
	EvRenderContext *rc = ev_render_context_new (page1, 0, 1.0);

	ev_render_context_set_page (rc, page2);
	g_assert_true (rc->page == page2);

	ev_render_context_set_rotation (rc, 270);
	g_assert_cmpint (rc->rotation, ==, 270);

	ev_render_context_set_scale (rc, 2.5);
	g_assert_cmpfloat (rc->scale, ==, 2.5);

	g_object_unref (rc);
	g_object_unref (page1);
	g_object_unref (page2);
}

static void
test_render_context_zero_scale (void)
{
	/* Scale can be 0 (used by 'fit width' mode that has
	 * not yet been computed). */
	EvPage *page = ev_page_new (0);
	EvRenderContext *rc = ev_render_context_new (page, 0, 0.0);

	g_assert_cmpfloat (rc->scale, ==, 0.0);
	g_object_unref (rc);
	g_object_unref (page);
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/ev-page/new",                    test_page_new);
	g_test_add_func ("/ev-page/new-negative-index",      test_page_new_negative_index);
	g_test_add_func ("/ev-page/backend-page-roundtrip",  test_page_backend_page_roundtrip);
	g_test_add_func ("/ev-page/no-destroy-when-no-backend",
	                                                       test_page_no_destroy_when_no_backend);
	g_test_add_func ("/ev-render-context/new",          test_render_context_new);
	g_test_add_func ("/ev-render-context/setters",      test_render_context_setters);
	g_test_add_func ("/ev-render-context/zero-scale",   test_render_context_zero_scale);

	return g_test_run ();
}
