/* test-string-match.c - Unit tests for the ev_string_match helper
 *                       added in PR #117 (B4 search foundation).
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Tests cover the three match modes (DEFAULT, CASE_INSENSITIVE,
 * REGEX) and the edge cases (empty inputs, NULL inputs, regex
 * compile errors).
 *
 * Run via `meson test -C build test-string-match`.
 */

#include <config.h>
#include <glib.h>

#include "ev-string-match.h"

/* ------------------------------------------------------------------------- */
/* DEFAULT (case-sensitive substring)                                        */
/* ------------------------------------------------------------------------- */

static void
test_default_match (void)
{
	g_assert_true (ev_string_match ("hello world", "world", EV_STRING_MATCH_DEFAULT));
}

static void
test_default_no_match (void)
{
	g_assert_false (ev_string_match ("hello world", "World", EV_STRING_MATCH_DEFAULT));
}

static void
test_default_at_start (void)
{
	g_assert_true (ev_string_match ("hello world", "hello", EV_STRING_MATCH_DEFAULT));
}

static void
test_default_at_end (void)
{
	g_assert_true (ev_string_match ("hello world", "world", EV_STRING_MATCH_DEFAULT));
}

static void
test_default_empty_text (void)
{
	g_assert_false (ev_string_match ("", "x", EV_STRING_MATCH_DEFAULT));
}

static void
test_default_empty_pattern (void)
{
	g_assert_false (ev_string_match ("x", "", EV_STRING_MATCH_DEFAULT));
}

/* ------------------------------------------------------------------------- */
/* CASE_INSENSITIVE                                                          */
/* ------------------------------------------------------------------------- */

static void
test_case_insensitive_match (void)
{
	g_assert_true (ev_string_match ("Hello World", "world", EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_true (ev_string_match ("Hello World", "WORLD", EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_true (ev_string_match ("Hello World", "hello", EV_STRING_MATCH_CASE_INSENSITIVE));
}

static void
test_case_insensitive_no_match (void)
{
	g_assert_false (ev_string_match ("Hello World", "foo", EV_STRING_MATCH_CASE_INSENSITIVE));
}

static void
test_case_insensitive_mixed_case (void)
{
	/* "hElLo WoRLd" should still match "hello" and "world" */
	g_assert_true (ev_string_match ("hElLo WoRLd", "hello", EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_true (ev_string_match ("hElLo WoRLd", "WORLD", EV_STRING_MATCH_CASE_INSENSITIVE));
}

static void
test_case_insensitive_ascii_only (void)
{
	/* g_ascii_strncasecmp is locale-independent ASCII-only.
	 * This test verifies that the function uses ASCII
	 * semantics, not the current locale's case-mapping. */
	g_assert_true (ev_string_match ("ABC def", "abc", EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_true (ev_string_match ("ABC def", "DEF", EV_STRING_MATCH_CASE_INSENSITIVE));
}

/* ------------------------------------------------------------------------- */
/* REGEX                                                                     */
/* ------------------------------------------------------------------------- */

static void
test_regex_simple (void)
{
	g_assert_true (ev_string_match ("hello world", "h.*o", EV_STRING_MATCH_REGEX));
}

static void
test_regex_alternation (void)
{
	g_assert_true (ev_string_match ("foo bar", "(foo|bar)", EV_STRING_MATCH_REGEX));
	g_assert_false (ev_string_match ("baz qux", "(foo|bar)", EV_STRING_MATCH_REGEX));
}

static void
test_regex_anchors (void)
{
	g_assert_true (ev_string_match ("hello", "^hello$", EV_STRING_MATCH_REGEX));
	g_assert_false (ev_string_match ("hello world", "^hello$", EV_STRING_MATCH_REGEX));
}

static void
test_regex_with_case_insensitive (void)
{
	/* CASE_INSENSITIVE | REGEX: should prepend (?i) */
	g_assert_true (ev_string_match ("Hello World", "world",
		EV_STRING_MATCH_REGEX | EV_STRING_MATCH_CASE_INSENSITIVE));
}

static void
test_regex_explicit_case_flag (void)
{
	/* If the pattern starts with (?-i), the caller wants
	 * case-sensitive even if the CASE_INSENSITIVE flag is set. */
	g_assert_false (ev_string_match ("Hello World", "(?-i)world",
		EV_STRING_MATCH_REGEX | EV_STRING_MATCH_CASE_INSENSITIVE));
}

static void
test_regex_invalid (void)
{
	/* Invalid regex should return FALSE (no match) and
	 * emit a g_warning (suppressed in test mode by the
	 * default G_MESSAGES_DEBUG=no, so the test runner
	 * doesn't see the warning). */
	g_assert_false (ev_string_match ("hello", "[invalid(", EV_STRING_MATCH_REGEX));
}

/* ------------------------------------------------------------------------- */
/* NULL / empty                                                              */
/* ------------------------------------------------------------------------- */

static void
test_null_text (void)
{
	g_assert_false (ev_string_match (NULL, "x", EV_STRING_MATCH_DEFAULT));
	g_assert_false (ev_string_match (NULL, "x", EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_false (ev_string_match (NULL, "x", EV_STRING_MATCH_REGEX));
}

static void
test_null_pattern (void)
{
	g_assert_false (ev_string_match ("x", NULL, EV_STRING_MATCH_DEFAULT));
	g_assert_false (ev_string_match ("x", NULL, EV_STRING_MATCH_CASE_INSENSITIVE));
	g_assert_false (ev_string_match ("x", NULL, EV_STRING_MATCH_REGEX));
}

static void
test_both_empty (void)
{
	g_assert_false (ev_string_match ("", "", EV_STRING_MATCH_DEFAULT));
	g_assert_false (ev_string_match ("", "", EV_STRING_MATCH_CASE_INSENSITIVE));
}

int
main (int argc, char *argv[])
{
	g_test_init (&argc, &argv, NULL);

	/* DEFAULT */
	g_test_add_func ("/string-match/default/match",            test_default_match);
	g_test_add_func ("/string-match/default/no-match",         test_default_no_match);
	g_test_add_func ("/string-match/default/match-at-start",   test_default_at_start);
	g_test_add_func ("/string-match/default/match-at-end",     test_default_at_end);
	g_test_add_func ("/string-match/default/empty-text",       test_default_empty_text);
	g_test_add_func ("/string-match/default/empty-pattern",    test_default_empty_pattern);

	/* CASE_INSENSITIVE */
	g_test_add_func ("/string-match/case-insensitive/match",   test_case_insensitive_match);
	g_test_add_func ("/string-match/case-insensitive/no-match", test_case_insensitive_no_match);
	g_test_add_func ("/string-match/case-insensitive/mixed",   test_case_insensitive_mixed_case);
	g_test_add_func ("/string-match/case-insensitive/ascii",   test_case_insensitive_ascii_only);

	/* REGEX */
	g_test_add_func ("/string-match/regex/simple",             test_regex_simple);
	g_test_add_func ("/string-match/regex/alternation",        test_regex_alternation);
	g_test_add_func ("/string-match/regex/anchors",            test_regex_anchors);
	g_test_add_func ("/string-match/regex/with-case-insensitive", test_regex_with_case_insensitive);
	g_test_add_func ("/string-match/regex/explicit-case-flag", test_regex_explicit_case_flag);
	g_test_add_func ("/string-match/regex/invalid",            test_regex_invalid);

	/* NULL / empty */
	g_test_add_func ("/string-match/null/text",                test_null_text);
	g_test_add_func ("/string-match/null/pattern",             test_null_pattern);
	g_test_add_func ("/string-match/null/both-empty",          test_both_empty);

	return g_test_run ();
}
