/* test-rect-intersect.c - Unit tests for ev_rectangle_intersect
 *                         (added in PR #87).
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * ev_rectangle_intersect is a 4-comparison helper that returns
 * TRUE if two EvRectangles overlap (closed-box semantics: edge
 * touches count as intersect).
 *
 * Test cases:
 *   - non-overlapping on x-axis only
 *   - non-overlapping on y-axis only
 *   - non-overlapping on both axes
 *   - fully contained
 *   - edge-touching (returns TRUE: closed-box)
 *   - corner-touching (returns TRUE: closed-box)
 *   - identical rectangles (returns TRUE)
 *   - NULL safety: returns FALSE for either arg NULL
 */

#include <config.h>
#include <glib.h>

#include "ev-document.h"

static EvRectangle *
make_rect (gdouble x1, gdouble y1, gdouble x2, gdouble y2)
{
	EvRectangle *r = ev_rectangle_new ();
	r->x1 = x1;
	r->y1 = y1;
	r->x2 = x2;
	r->y2 = y2;
	return r;
}

static void
test_no_overlap_x (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (20, 0, 30, 10);
	g_assert (!ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_no_overlap_y (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (0, 20, 10, 30);
	g_assert (!ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_no_overlap_both (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (20, 20, 30, 30);
	g_assert (!ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_fully_contained (void)
{
	EvRectangle *outer = make_rect (0, 0, 100, 100);
	EvRectangle *inner = make_rect (10, 10, 20, 20);
	g_assert (ev_rectangle_intersect (outer, inner));
	g_assert (ev_rectangle_intersect (inner, outer));
	ev_rectangle_free (outer);
	ev_rectangle_free (inner);
}

static void
test_edge_touching (void)
{
	/* a ends exactly at b's start (no gap, no overlap).  Closed-box
	 * semantics: returns TRUE because the edge is shared. */
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (10, 0, 20, 10);
	g_assert (ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_corner_touching (void)
{
	/* a ends exactly at b's start, on both axes. */
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (10, 10, 20, 20);
	g_assert (ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_identical (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (0, 0, 10, 10);
	g_assert (ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_partial_overlap (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	EvRectangle *b = make_rect (5, 5, 15, 15);
	g_assert (ev_rectangle_intersect (a, b));
	ev_rectangle_free (a);
	ev_rectangle_free (b);
}

static void
test_null_safety (void)
{
	EvRectangle *a = make_rect (0, 0, 10, 10);
	g_assert (!ev_rectangle_intersect (NULL, a));
	g_assert (!ev_rectangle_intersect (a, NULL));
	g_assert (!ev_rectangle_intersect (NULL, NULL));
	ev_rectangle_free (a);
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/rect-intersect/no-overlap-x",       test_no_overlap_x);
	g_test_add_func ("/rect-intersect/no-overlap-y",       test_no_overlap_y);
	g_test_add_func ("/rect-intersect/no-overlap-both",    test_no_overlap_both);
	g_test_add_func ("/rect-intersect/fully-contained",    test_fully_contained);
	g_test_add_func ("/rect-intersect/edge-touching",      test_edge_touching);
	g_test_add_func ("/rect-intersect/corner-touching",    test_corner_touching);
	g_test_add_func ("/rect-intersect/identical",          test_identical);
	g_test_add_func ("/rect-intersect/partial-overlap",    test_partial_overlap);
	g_test_add_func ("/rect-intersect/null-safety",        test_null_safety);

	return g_test_run ();
}
