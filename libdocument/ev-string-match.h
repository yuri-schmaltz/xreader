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

#if !defined (__EV_XREADER_DOCUMENT_H_INSIDE__) && !defined (XREADER_COMPILATION)
#error "Only <xreader-document.h> can be included directly."
#endif

#ifndef EV_STRING_MATCH_H
#define EV_STRING_MATCH_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * EvStringMatchFlags:
 * @EV_STRING_MATCH_DEFAULT: plain substring match, case-sensitive
 *   (equivalent to strstr()).
 * @EV_STRING_MATCH_CASE_INSENSITIVE: substring match, ignoring case
 *   (g_ascii_strcasecmp on each position).
 * @EV_STRING_MATCH_REGEX: treat @pattern as a POSIX extended regex
 *   (passed to g_regex_match_simple).  Implies
 *   %EV_STRING_MATCH_CASE_INSENSITIVE if the regex has no explicit
 *   "(?-i)" prefix.
 *
 * Flags controlling how ev_string_match() compares @text and
 * @pattern.  The flags are additive; multiple flags can be OR'd
 * together.
 *
 * The B4 search-improvements roadmap (4.10.0) adds:
 *   - %EV_STRING_MATCH_WHOLE_WORD (substring with word boundaries
 *     on both sides)
 *
 * Since: 4.9.0
 */
typedef enum
{
	EV_STRING_MATCH_DEFAULT          = 0,
	EV_STRING_MATCH_CASE_INSENSITIVE = 1 << 0,
	EV_STRING_MATCH_REGEX            = 1 << 1
} EvStringMatchFlags;

/**
 * ev_string_match:
 * @text: the haystack (may be NULL or empty)
 * @pattern: the needle (may be NULL or empty)
 * @flags: a bitwise-OR of %EvStringMatchFlags
 *
 * Returns %TRUE if @pattern is found in @text, using the
 * match mode specified by @flags.
 *
 * If either argument is NULL or empty, returns %FALSE
 * (no match).  This matches the behaviour of strstr() and
 * g_regex_match_simple().
 *
 * The case-insensitive match uses g_ascii_strcasecmp() at
 * each position (i.e. locale-independent ASCII-only).  This
 * is consistent with the rest of xreader's case-insensitive
 * code paths (cf. ev_form_field_type_from_string()).
 *
 * If %EV_STRING_MATCH_REGEX is set, @pattern is compiled
 * with g_regex_new().  Compilation errors are returned as
 * %FALSE (no match), with a g_warning() to the log.
 *
 * Returns: %TRUE if @pattern matches somewhere in @text
 *
 * Since: 4.9.0
 */
gboolean ev_string_match (const gchar         *text,
			  const gchar         *pattern,
			  EvStringMatchFlags   flags);

G_END_DECLS

#endif /* !EV_STRING_MATCH_H */
