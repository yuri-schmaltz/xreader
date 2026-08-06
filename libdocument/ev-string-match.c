/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * Xreader is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Xreader is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "config.h"

#include <string.h>

#include "ev-string-match.h"

/**
 * SECTION:ev-string-match
 * @short_description: String matching helpers for document search
 * @title: EvStringMatch
 * @include: xreader-document.h
 *
 * The B4 search-improvements roadmap (4.10.0) wants regex,
 * case-insensitive, and whole-word match modes for the
 * document search.  This module provides the building block
 * (ev_string_match) that the shell (EvJobFind) and the
 * backends (poppler, etc.) can use.
 *
 * Design notes:
 *   - The case-insensitive mode uses g_ascii_strcasecmp() at
 *     each position, NOT g_utf8_strdown() on the whole text.
 *     The former is O(n*m) and locale-independent; the
 *     latter is O(n) but locale-dependent and allocates a
 *     copy.  For document search (where the haystack is a
 *     page of text and the needle is a few words), the
 *     former is simpler and predictable.
 *   - The regex mode uses g_regex_new() / g_regex_match().
 *     PCRE2 is bundled with glib, so no extra dependency.
 *   - NULL and empty strings are treated as "no match",
 *     which is the natural extension of strstr() (which
 *     returns the haystack when the needle is empty) but
 *     simpler for the caller to reason about.
 */

static gboolean
substring_match_case_sensitive (const gchar *text,
			       const gchar *pattern)
{
	return strstr (text, pattern) != NULL;
}

static gboolean
substring_match_case_insensitive (const gchar *text,
				 const gchar *pattern)
{
	gsize text_len;
	gsize pattern_len;
	gsize i;

	text_len = strlen (text);
	pattern_len = strlen (pattern);

	if (pattern_len > text_len)
		return FALSE;

	for (i = 0; i <= text_len - pattern_len; i++) {
		if (g_ascii_strncasecmp (text + i, pattern, pattern_len) == 0)
			return TRUE;
	}

	return FALSE;
}

static gboolean
regex_match (const gchar *text,
	     const gchar *pattern,
	     gboolean     case_insensitive)
{
	GRegexCompileFlags compile_flags = 0;
	GRegex *regex;
	GMatchInfo *match_info = NULL;
	gboolean matched;

	/* If the regex has no explicit (?i) / (?-i) prefix, honor
	 * the case-insensitive flag from the caller.  g_regex_new
	 * treats a missing prefix as case-sensitive, so we prepend
	 * (?i) when needed.  We use a simple heuristic: if the
	 * pattern starts with "(?" we trust the user; otherwise
	 * we prepend "(?i)" if case_insensitive is set.
	 *
	 * The (?i) prefix is a PCRE directive that means
	 * "case-insensitive match from this point on". */
	if (case_insensitive &&
	    (pattern[0] != '(' || pattern[1] != '?')) {
		gchar *prefixed = g_strconcat ("(?i)", pattern, NULL);
		regex = g_regex_new (prefixed, compile_flags, 0, NULL);
		g_free (prefixed);
	} else {
		regex = g_regex_new (pattern, compile_flags, 0, NULL);
	}

	if (regex == NULL) {
		g_warning ("ev_string_match: invalid regex '%s'", pattern);
		return FALSE;
	}

	matched = g_regex_match (regex, text, 0, &match_info);
	g_match_info_free (match_info);
	g_regex_unref (regex);

	return matched;
}

gboolean
ev_string_match (const gchar         *text,
		 const gchar         *pattern,
		 EvStringMatchFlags   flags)
{
	g_return_val_if_fail (text != NULL, FALSE);
	g_return_val_if_fail (pattern != NULL, FALSE);

	if (text[0] == '\0' || pattern[0] == '\0')
		return FALSE;

	if (flags & EV_STRING_MATCH_REGEX) {
		return regex_match (text, pattern,
				    (flags & EV_STRING_MATCH_CASE_INSENSITIVE) != 0);
	}

	if (flags & EV_STRING_MATCH_CASE_INSENSITIVE) {
		return substring_match_case_insensitive (text, pattern);
	}

	return substring_match_case_sensitive (text, pattern);
}
