/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/*
 *  Copyright (C) 2000-2003 Marco Pesenti Gritti
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#if !defined (__EV_XREADER_DOCUMENT_H_INSIDE__) && !defined (XREADER_COMPILATION)
#error "Only <xreader-document.h> can be included directly."
#endif

#ifndef EV_DOCUMENT_INFO_H
#define EV_DOCUMENT_INFO_H

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

typedef struct _EvDocumentInfo    EvDocumentInfo;
typedef struct _EvDocumentLicense EvDocumentLicense;

#define EV_TYPE_DOCUMENT_INFO (ev_document_info_get_type())

typedef enum
{
	EV_DOCUMENT_LAYOUT_SINGLE_PAGE,
	EV_DOCUMENT_LAYOUT_ONE_COLUMN,
	EV_DOCUMENT_LAYOUT_TWO_COLUMN_LEFT,
	EV_DOCUMENT_LAYOUT_TWO_COLUMN_RIGHT,
	EV_DOCUMENT_LAYOUT_TWO_PAGE_LEFT,
	EV_DOCUMENT_LAYOUT_TWO_PAGE_RIGHT
} EvDocumentLayout;

typedef enum
{
	EV_DOCUMENT_MODE_NONE,
	EV_DOCUMENT_MODE_USE_OC,
	EV_DOCUMENT_MODE_USE_THUMBS,
	EV_DOCUMENT_MODE_FULL_SCREEN,
	EV_DOCUMENT_MODE_USE_ATTACHMENTS,
	EV_DOCUMENT_MODE_PRESENTATION = EV_DOCUMENT_MODE_FULL_SCREEN /* Will these be different? */
} EvDocumentMode;

typedef enum
{
	EV_DOCUMENT_UI_HINT_HIDE_TOOLBAR = 1 << 0,
	EV_DOCUMENT_UI_HINT_HIDE_MENUBAR = 1 << 1,
	EV_DOCUMENT_UI_HINT_HIDE_WINDOWUI = 1 << 2,
	EV_DOCUMENT_UI_HINT_FIT_WINDOW = 1 << 3,
	EV_DOCUMENT_UI_HINT_CENTER_WINDOW = 1 << 4,
	EV_DOCUMENT_UI_HINT_DISPLAY_DOC_TITLE = 1 << 5,
	EV_DOCUMENT_UI_HINT_DIRECTION_RTL = 1 << 6
} EvDocumentUIHints;

/* This define is needed because glib-mkenums chokes with multiple lines */
#define PERMISSIONS_FULL (EV_DOCUMENT_PERMISSIONS_OK_TO_PRINT  \
			| EV_DOCUMENT_PERMISSIONS_OK_TO_MODIFY \
			| EV_DOCUMENT_PERMISSIONS_OK_TO_COPY   \
			| EV_DOCUMENT_PERMISSIONS_OK_TO_ADD_NOTES)

typedef enum
{
	EV_DOCUMENT_PERMISSIONS_OK_TO_PRINT = 1 << 0,
	EV_DOCUMENT_PERMISSIONS_OK_TO_MODIFY = 1 << 1,
	EV_DOCUMENT_PERMISSIONS_OK_TO_COPY = 1 << 2,
	EV_DOCUMENT_PERMISSIONS_OK_TO_ADD_NOTES = 1 << 3,
	EV_DOCUMENT_PERMISSIONS_FULL = PERMISSIONS_FULL
} EvDocumentPermissions;

typedef enum
{
	EV_DOCUMENT_INFO_TITLE = 1 << 0,
	EV_DOCUMENT_INFO_FORMAT = 1 << 1,
	EV_DOCUMENT_INFO_AUTHOR = 1 << 2,
	EV_DOCUMENT_INFO_SUBJECT = 1 << 3,
	EV_DOCUMENT_INFO_KEYWORDS = 1 << 4,
	EV_DOCUMENT_INFO_LAYOUT = 1 << 5,
	EV_DOCUMENT_INFO_CREATOR = 1 << 6,
	EV_DOCUMENT_INFO_PRODUCER = 1 << 7,
	EV_DOCUMENT_INFO_CREATION_DATE = 1 << 8,
	EV_DOCUMENT_INFO_MOD_DATE = 1 << 9,
	EV_DOCUMENT_INFO_LINEARIZED = 1 << 10,
	EV_DOCUMENT_INFO_START_MODE = 1 << 11,
	EV_DOCUMENT_INFO_UI_HINTS = 1 << 12,
	EV_DOCUMENT_INFO_PERMISSIONS = 1 << 13,
	EV_DOCUMENT_INFO_N_PAGES = 1 << 14,
	EV_DOCUMENT_INFO_SECURITY = 1 << 15,
	EV_DOCUMENT_INFO_PAPER_SIZE = 1 << 16,
	EV_DOCUMENT_INFO_LICENSE = 1 << 17,

} EvDocumentInfoFields;

struct _EvDocumentInfo
{
	char *title;
	char *format; /* eg, "pdf-1.5" */
	char *author;
	char *subject;
	char *keywords;
	char *creator;
	char *producer;
	char *linearized;
        char *security;
	GDateTime *creation_date;
	GDateTime *modified_date;
	EvDocumentLayout layout;
	EvDocumentMode mode;
	guint ui_hints;
	guint permissions;
	int   n_pages;
	double paper_height;
	double paper_width;
	EvDocumentLicense *license;

	/* Mask of all the valid fields */
	guint fields_mask;
};

GType           ev_document_info_get_type (void) G_GNUC_CONST;

/**
 * ev_document_info_copy:
 * @info: (transfer none): an #EvDocumentInfo
 *
 * Returns: (transfer full): a deep copy of @info
 *
 * Since: 4.8.0
 */
EvDocumentInfo *ev_document_info_copy     (EvDocumentInfo *info);

/**
 * ev_document_info_free:
 * @info: (nullable): an #EvDocumentInfo
 *
 * Frees @info and all of its string fields.  If @info is
 * %NULL, this is a no-op.
 *
 * Since: 4.8.0
 */
void            ev_document_info_free     (EvDocumentInfo *info);

/* EvDocumentLicense */
#define EV_TYPE_DOCUMENT_LICENSE (ev_document_license_get_type())
struct _EvDocumentLicense {
	gchar *text;
	gchar *uri;
	gchar *web_statement;
};
GType              ev_document_license_get_type          (void) G_GNUC_CONST;

/**
 * ev_document_license_new:
 *
 * Returns: (transfer full): a new empty #EvDocumentLicense
 *
 * Since: 4.8.0
 */
EvDocumentLicense *ev_document_license_new               (void);

/**
 * ev_document_license_copy:
 * @license: an #EvDocumentLicense
 *
 * Returns: (transfer full): a deep copy of @license
 *
 * Since: 4.8.0
 */
EvDocumentLicense *ev_document_license_copy              (EvDocumentLicense *license);

/**
 * ev_document_license_free:
 * @license: (nullable): an #EvDocumentLicense
 *
 * Frees @license and all of its string fields.  If @license
 * is %NULL, this is a no-op.
 *
 * Since: 4.8.0
 */
void               ev_document_license_free              (EvDocumentLicense *license);

/**
 * ev_document_license_get_text:
 * @license: an #EvDocumentLicense
 *
 * Returns the human-readable license text (e.g. "This
 * document is licensed under CC-BY-SA 4.0").  The string
 * is owned by @license and must not be freed.
 *
 * Returns: (transfer none) (nullable): the license text, or %NULL
 *
 * Since: 4.8.0
 */
const gchar       *ev_document_license_get_text          (EvDocumentLicense *license);

/**
 * ev_document_license_get_uri:
 * @license: an #EvDocumentLicense
 *
 * Returns the URI of the license (e.g.
 * "https://creativecommons.org/licenses/by-sa/4.0/").
 * The string is owned by @license and must not be freed.
 *
 * Returns: (transfer none) (nullable): the license URI, or %NULL
 *
 * Since: 4.8.0
 */
const gchar       *ev_document_license_get_uri           (EvDocumentLicense *license);

/**
 * ev_document_license_get_web_statement:
 * @license: an #EvDocumentLicense
 *
 * Returns the web-statement URL (a human-readable page
 * about the license, e.g.
 * "https://example.com/this-doc-license.html").  The
 * string is owned by @license and must not be freed.
 *
 * Returns: (transfer none) (nullable): the web-statement URL, or %NULL
 *
 * Since: 4.8.0
 */
const gchar       *ev_document_license_get_web_statement (EvDocumentLicense *license);

G_END_DECLS

#endif /* EV_DOCUMENT_INFO_H */
