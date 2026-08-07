/* test-annotations-filter.c - Unit tests for the B5 annotation
 *                               list helpers (PR #121).
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Tests cover:
 *   - ev_annotations_filter_by_type:
 *     * basic filter (mixed list -> 3 sub-lists)
 *     * empty input -> NULL
 *     * NULL input -> NULL
 *     * no match -> NULL
 *     * order preservation
 *     * NULL elements in the list are skipped
 *   - ev_annotations_count_by_type:
 *     * basic count (mixed list -> 3 counts)
 *     * empty input -> 0
 *     * NULL input -> 0
 *
 * The test uses real EvAnnotation objects created via the
 * public constructors (ev_annotation_text_new etc.) and a
 * single EvPage.  No backend is needed -- the annotations
 * don't need to be "real" (i.e. backed by poppler), they
 * just need to exist so ev_annotation_get_annotation_type()
 * can dispatch on the GType.
 *
 * Run via `meson test -C build test-annotations-filter`.
 */

#include <config.h>
#include <glib.h>
#include <glib-object.h>

#include "ev-annotation.h"
#include "ev-attachment.h"
#include "ev-page.h"

/* Build a small mixed list of annotations on @page.
 *
 * Returns: (transfer full) (element-type EvAnnotation):
 *   3 TEXT + 2 ATTACHMENT + 1 TEXT_MARKUP = 6 elements
 *
 * The caller owns the list (g_list_free) but NOT the
 * elements (they belong to the document; in this test,
 * we unref each one explicitly before g_list_free). */
static GList *
build_mixed_annotations (EvPage *page)
{
	GList *list = NULL;
	EvAnnotation *a;

	/* TEXT x3 */
	a = ev_annotation_text_new (page);
	list = g_list_prepend (list, a);
	a = ev_annotation_text_new (page);
	list = g_list_prepend (list, a);
	a = ev_annotation_text_new (page);
	list = g_list_prepend (list, a);

	/* ATTACHMENT x2 */
	{
		EvAttachment *att1 = ev_attachment_new ("file1.txt", NULL, 100, NULL);
		EvAttachment *att2 = ev_attachment_new ("file2.pdf", NULL, 200, NULL);
		a = ev_annotation_attachment_new (page, att1);
		list = g_list_prepend (list, a);
		a = ev_annotation_attachment_new (page, att2);
		list = g_list_prepend (list, a);
		g_object_unref (att1);
		g_object_unref (att2);
	}

	/* TEXT_MARKUP x1 */
	a = ev_annotation_text_markup_highlight_new (page);
	list = g_list_prepend (list, a);

	/* Note: build order is reversed (prepend); the final
	 * list (after g_list_reverse) is:
	 *   TEXT, TEXT, TEXT, ATTACHMENT, ATTACHMENT, TEXT_MARKUP
	 */
	return g_list_reverse (list);
}

static void
free_annotations (GList *list)
{
	g_list_foreach (list, (GFunc) g_object_unref, NULL);
	g_list_free (list);
}

/* ------------------------------------------------------------------------- */
/* ev_annotations_filter_by_type                                              */
/* ------------------------------------------------------------------------- */

static void
test_filter_text (void)
{
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);
	GList  *text;

	text = ev_annotations_filter_by_type (all, EV_ANNOTATION_TYPE_TEXT);
	g_assert_cmpuint (g_list_length (text), ==, 3);
	g_list_foreach (text, (GFunc) g_object_unref, NULL);
	g_list_free (text);

	free_annotations (all);
	g_object_unref (page);
}

static void
test_filter_attachment (void)
{
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);
	GList  *att;

	att = ev_annotations_filter_by_type (all, EV_ANNOTATION_TYPE_ATTACHMENT);
	g_assert_cmpuint (g_list_length (att), ==, 2);
	g_list_foreach (att, (GFunc) g_object_unref, NULL);
	g_list_free (att);

	free_annotations (all);
	g_object_unref (page);
}

static void
test_filter_text_markup (void)
{
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);
	GList  *markup;

	markup = ev_annotations_filter_by_type (all, EV_ANNOTATION_TYPE_TEXT_MARKUP);
	g_assert_cmpuint (g_list_length (markup), ==, 1);
	g_list_foreach (markup, (GFunc) g_object_unref, NULL);
	g_list_free (markup);

	free_annotations (all);
	g_object_unref (page);
}

static void
test_filter_unknown_no_match (void)
{
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);
	GList  *none;

	/* No annotation in the list is of type UNKNOWN. */
	none = ev_annotations_filter_by_type (all, EV_ANNOTATION_TYPE_UNKNOWN);
	g_assert_null (none);

	free_annotations (all);
	g_object_unref (page);
}

static void
test_filter_empty_input (void)
{
	/* Empty list -> NULL. */
	g_assert_null (ev_annotations_filter_by_type (NULL, EV_ANNOTATION_TYPE_TEXT));
}

static void
test_filter_null_input (void)
{
	/* NULL list -> NULL. */
	GList *empty = NULL;
	g_assert_null (ev_annotations_filter_by_type (empty, EV_ANNOTATION_TYPE_TEXT));
}

static void
test_filter_preserves_order (void)
{
	/* The filter uses g_list_reverse() at the end so the
	 * output order matches the input order.  Verify that
	 * the 3 TEXT annotations come out in the same order
	 * they went in. */
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);
	GList  *text;
	GList  *l;
	gint   expected_order[3] = { 100, 200, 300 };
	gint   i = 0;

	/* Set the contents to a unique value on each so we
	 * can verify the order. */
	{
		EvAnnotation *a;
		gchar buf[32];
		gint j = 0;
		for (l = all; l != NULL; l = l->next, j++) {
			if (EV_IS_ANNOTATION_TEXT (l->data)) {
				g_snprintf (buf, sizeof (buf), "%d", expected_order[j % 3]);
				ev_annotation_set_contents (l->data, buf);
			}
		}
		a = NULL; /* silence unused */
	}

	text = ev_annotations_filter_by_type (all, EV_ANNOTATION_TYPE_TEXT);
	g_assert_cmpuint (g_list_length (text), ==, 3);
	for (l = text; l != NULL; l = l->next, i++) {
		gchar buf[32];
		g_snprintf (buf, sizeof (buf), "%d", expected_order[i]);
		g_assert_cmpstr (ev_annotation_get_contents (l->data), ==, buf);
	}

	g_list_foreach (text, (GFunc) g_object_unref, NULL);
	g_list_free (text);
	free_annotations (all);
	g_object_unref (page);
}

static void
test_filter_skips_null_elements (void)
{
	/* A list with a NULL element should be skipped (not
	 * crash).  Add a NULL between two TEXT annotations. */
	EvPage *page = ev_page_new (1);
	EvAnnotation *a1 = ev_annotation_text_new (page);
	EvAnnotation *a2 = ev_annotation_text_new (page);
	GList *list = NULL;
	GList *result;

	list = g_list_append (list, a1);
	list = g_list_append (list, NULL);
	list = g_list_append (list, a2);

	result = ev_annotations_filter_by_type (list, EV_ANNOTATION_TYPE_TEXT);
	g_assert_cmpuint (g_list_length (result), ==, 2);

	g_list_foreach (result, (GFunc) g_object_unref, NULL);
	g_list_free (result);
	g_list_free (list);
	g_object_unref (page);
}

/* ------------------------------------------------------------------------- */
/* ev_annotations_count_by_type                                                */
/* ------------------------------------------------------------------------- */

static void
test_count_text (void)
{
	EvPage *page = ev_page_new (1);
	GList  *all  = build_mixed_annotations (page);

	g_assert_cmpuint (ev_annotations_count_by_type (all, EV_ANNOTATION_TYPE_TEXT), ==, 3);
	g_assert_cmpuint (ev_annotations_count_by_type (all, EV_ANNOTATION_TYPE_ATTACHMENT), ==, 2);
	g_assert_cmpuint (ev_annotations_count_by_type (all, EV_ANNOTATION_TYPE_TEXT_MARKUP), ==, 1);
	g_assert_cmpuint (ev_annotations_count_by_type (all, EV_ANNOTATION_TYPE_UNKNOWN), ==, 0);

	free_annotations (all);
	g_object_unref (page);
}

static void
test_count_empty (void)
{
	g_assert_cmpuint (ev_annotations_count_by_type (NULL, EV_ANNOTATION_TYPE_TEXT), ==, 0);
}

/* ------------------------------------------------------------------------- */

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	/* filter */
	g_test_add_func ("/annotations/filter/text",             test_filter_text);
	g_test_add_func ("/annotations/filter/attachment",        test_filter_attachment);
	g_test_add_func ("/annotations/filter/text-markup",      test_filter_text_markup);
	g_test_add_func ("/annotations/filter/unknown-no-match", test_filter_unknown_no_match);
	g_test_add_func ("/annotations/filter/empty-input",      test_filter_empty_input);
	g_test_add_func ("/annotations/filter/null-input",       test_filter_null_input);
	g_test_add_func ("/annotations/filter/preserves-order",  test_filter_preserves_order);
	g_test_add_func ("/annotations/filter/skips-null",       test_filter_skips_null_elements);

	/* count */
	g_test_add_func ("/annotations/count/mixed",            test_count_text);
	g_test_add_func ("/annotations/count/empty",            test_count_empty);

	return g_test_run ();
}
