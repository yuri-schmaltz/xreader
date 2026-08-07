/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; c-indent-level: 8 -*- */
/*
 *  Copyright (C) 2009 Carlos Garcia Campos
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
 *  $Id$
 */

#if !defined (__EV_XREADER_DOCUMENT_H_INSIDE__) && !defined (XREADER_COMPILATION)
#error "Only <xreader-document.h> can be included directly."
#endif

#ifndef EV_DOCUMENT_H
#define EV_DOCUMENT_H

#include <glib-object.h>
#include <glib.h>
#include <gmodule.h>
#include <cairo.h>

#include "ev-document-info.h"
#include "ev-page.h"
#include "ev-render-context.h"

G_BEGIN_DECLS

#define EV_TYPE_DOCUMENT            (ev_document_get_type ())
#define EV_DOCUMENT(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), EV_TYPE_DOCUMENT, EvDocument))
#define EV_DOCUMENT_CLASS(k)        (G_TYPE_CHECK_CLASS_CAST((k), EV_TYPE_DOCUMENT, EvDocumentClass))
#define EV_IS_DOCUMENT(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), EV_TYPE_DOCUMENT))
#define EV_IS_DOCUMENT_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), EV_TYPE_DOCUMENT))
#define EV_DOCUMENT_GET_CLASS(inst) (G_TYPE_INSTANCE_GET_CLASS ((inst), EV_TYPE_DOCUMENT, EvDocumentClass))

typedef struct _EvDocument        EvDocument;
typedef struct _EvDocumentClass   EvDocumentClass;
typedef struct _EvDocumentPrivate EvDocumentPrivate;

#define EV_DOCUMENT_ERROR ev_document_error_quark ()
#define EV_DOC_MUTEX_LOCK (ev_document_doc_mutex_lock ())
#define EV_DOC_MUTEX_UNLOCK (ev_document_doc_mutex_unlock ())

typedef enum
{
        EV_DOCUMENT_ERROR_INVALID,
        EV_DOCUMENT_ERROR_UNSUPPORTED_CONTENT,
        EV_DOCUMENT_ERROR_ENCRYPTED
} EvDocumentError;

typedef struct {
        double x;
        double y;
} EvPoint;

typedef struct _EvRectangle EvRectangle;
typedef struct _EvMapping EvMapping;
typedef struct _EvSourceLink EvSourceLink;
typedef struct _EvDocumentBackendInfo EvDocumentBackendInfo;

struct _EvDocumentBackendInfo
{
	const gchar *name;
	const gchar *version;
};

struct _EvDocument
{
	GObject base;

	EvDocumentPrivate *priv;
	/*
	 * Since we can only access the members of this structure from the window frontend,
	 * we need a flag to detemine whether to replace the xreader-view with a web-view.
	 */
	gboolean	iswebdocument;
};

struct _EvDocumentClass
{
        GObjectClass base_class;

        /* Virtual Methods  */
        gboolean          (* load)            (EvDocument      *document,
                                               const char      *uri,
                                               GError         **error);
        gboolean          (* save)            (EvDocument      *document,
                                               const char      *uri,
                                               GError         **error);
        gint              (* get_n_pages)     (EvDocument      *document);
	EvPage          * (* get_page)        (EvDocument      *document,
					       gint             index);
        void              (* get_page_size)   (EvDocument      *document,
                                               EvPage          *page,
                                               double          *width,
                                               double          *height);
        gchar           * (* get_page_label)  (EvDocument      *document,
                                               EvPage          *page);
        cairo_surface_t * (* render)          (EvDocument      *document,
                                               EvRenderContext *rc);
        EvDocumentInfo  * (* get_info)        (EvDocument      *document);
        gboolean          (* get_backend_info)(EvDocument      *document,
                                               EvDocumentBackendInfo *info);
        gboolean	  (* support_synctex) (EvDocument      *document);

	void              (* toggle_night_mode)  (EvDocument      *document,gboolean night);
	void              (*check_add_night_sheet)(EvDocument      *document);	
};

GType            ev_document_get_type             (void) G_GNUC_CONST;

/**
 * ev_document_error_quark:
 *
 * Gets the #GQuark for the #GError domain used by the
 * #EvDocument interface.  The domain is %EV_DOCUMENT_ERROR
 * (which is a macro that expands to this function).
 *
 * Returns: the #GQuark
 *
 * Since: 4.8.0
 */
GQuark           ev_document_error_quark          (void);

/**
 * EvDocumentError:
 * @EV_DOCUMENT_ERROR_INVALID: the document URI is malformed
 *   or does not point to a valid file
 * @EV_DOCUMENT_ERROR_UNSUPPORTED_CONTENT: the document
 *   backend does not support the document's content type
 *   (e.g. trying to load a .docx with the poppler backend)
 * @EV_DOCUMENT_ERROR_ENCRYPTED: the document is encrypted
 *   and cannot be loaded without a password
 *
 * Error codes returned by #EvDocument method calls.  Used
 * with the %EV_DOCUMENT_ERROR quark and g_set_error().
 *
 * Since: 4.8.0
 */

/* Document mutex (a single global mutex shared by all documents) */
GMutex          *ev_document_get_doc_mutex        (void);
void             ev_document_doc_mutex_lock       (void);
void             ev_document_doc_mutex_unlock     (void);
gboolean         ev_document_doc_mutex_trylock    (void) G_GNUC_WARN_UNUSED_RESULT;

/* FontConfig mutex (a single global mutex for FontConfig calls) */
GMutex          *ev_document_get_fc_mutex         (void);
void             ev_document_fc_mutex_lock        (void);
void             ev_document_fc_mutex_unlock      (void);
gboolean         ev_document_fc_mutex_trylock     (void) G_GNUC_WARN_UNUSED_RESULT;

/**
 * ev_document_get_info:
 * @document: an #EvDocument
 *
 * Returns the #EvDocumentInfo describing @document.  The
 * returned object is owned by the document and must not be
 * freed.
 *
 * Returns: (transfer none) (nullable): the #EvDocumentInfo, or %NULL
 *
 * Since: 4.8.0
 */
EvDocumentInfo  *ev_document_get_info             (EvDocument      *document);

/**
 * ev_document_get_backend_info:
 * @document: an #EvDocument
 * @info: (out): an #EvDocumentBackendInfo to populate
 *
 * Populates @info with the name and version of the document's
 * backend (e.g. "Poppler 24.02.0").
 *
 * Returns: %TRUE on success, %FALSE if the backend does not
 *   provide version information
 *
 * Since: 4.8.0
 */
gboolean         ev_document_get_backend_info     (EvDocument      *document,
						   EvDocumentBackendInfo *info);

/**
 * ev_document_get_modified:
 * @document: an #EvDocument
 *
 * Returns whether the document has been modified since it
 * was loaded.  The shell uses this to decide whether to
 * show a "save changes?" dialog on close.
 *
 * Returns: %TRUE if the document has unsaved changes
 *
 * Since: 4.8.0
 */
gboolean         ev_document_get_modified         (EvDocument      *document);

/**
 * ev_document_set_modified:
 * @document: an #EvDocument
 * @modified: the new modified flag
 *
 * Sets the modified flag.  This is typically called by
 * the save code to reset the flag after a successful save.
 *
 * Since: 4.8.0
 */
void             ev_document_set_modified         (EvDocument      *document,
						   gboolean         modified);

/**
 * ev_document_load:
 * @document: an #EvDocument
 * @uri: the URI of the document to load
 * @error: (out) (nullable): return location for a #GError, or %NULL
 *
 * Loads the document from @uri.  The backend opens the
 * file, parses the document metadata, and primes the
 * internal caches.
 *
 * Returns: %TRUE on success, %FALSE on error (with @error set)
 *
 * Since: 4.8.0
 */
gboolean         ev_document_load                 (EvDocument      *document,
						   const char      *uri,
						   GError         **error);

/**
 * ev_document_save:
 * @document: an #EvDocument
 * @uri: the URI to save the document to
 * @error: (out) (nullable): return location for a #GError, or %NULL
 *
 * Saves the document to @uri.  This includes any unsaved
 * form-field values and annotations (the B5 PDF export).
 *
 * Returns: %TRUE on success, %FALSE on error (with @error set)
 *
 * Since: 4.8.0
 */
gboolean         ev_document_save                 (EvDocument      *document,
						   const char      *uri,
						   GError         **error);

/**
 * ev_document_get_n_pages:
 * @document: an #EvDocument
 *
 * Returns the number of pages in the document.
 *
 * Returns: the page count
 *
 * Since: 4.8.0
 */
gint             ev_document_get_n_pages          (EvDocument      *document);

/**
 * ev_document_get_page:
 * @document: an #EvDocument
 * @index: the 0-based page index
 *
 * Returns the #EvPage for @index.  The returned page is
 * owned by the document; do not unref.  Pages are created
 * lazily on first access.
 *
 * Returns: (transfer none) (nullable): the #EvPage, or %NULL
 *   if @index is out of range
 *
 * Since: 4.8.0
 */
EvPage          *ev_document_get_page             (EvDocument      *document,
						   gint             index);

/**
 * ev_document_get_page_size:
 * @document: an #EvDocument
 * @page_index: the 0-based page index
 * @width: (out): return location for the page width, in points
 * @height: (out): return location for the page height, in points
 *
 * Fetches the size of page @page_index.  For documents with
 * non-uniform page sizes (e.g. landscape pages inserted in
 * a portrait document), the size depends on @page_index.
 *
 * Since: 4.8.0
 */
void             ev_document_get_page_size        (EvDocument      *document,
						   gint             page_index,
						   double          *width,
						   double          *height);

/**
 * ev_document_get_page_label:
 * @document: an #EvDocument
 * @page_index: the 0-based page index
 *
 * Returns the printed page label of page @page_index
 * (e.g. "iv" for the fourth page in a Roman-numeral
 * document, or "B-12" for an appendix page).  The
 * returned string must be freed with g_free().
 *
 * Returns: (transfer full) (nullable): the label, or %NULL
 *
 * Since: 4.8.0
 */
gchar           *ev_document_get_page_label       (EvDocument      *document,
						   gint             page_index);

/**
 * ev_document_render:
 * @document: an #EvDocument
 * @rc: an #EvRenderContext with the page + the target size
 *
 * Renders the page described by @rc into a new cairo
 * surface.  The surface must be unreffed with
 * cairo_surface_destroy() when no longer needed.
 *
 * Returns: (transfer full) (nullable): a new cairo surface, or %NULL
 *
 * Since: 4.8.0
 */
cairo_surface_t *ev_document_render               (EvDocument      *document,
						   EvRenderContext *rc);

/**
 * ev_document_get_uri:
 * @document: an #EvDocument
 *
 * Returns the URI of the loaded document.  The string is
 * owned by the document and must not be freed.
 *
 * Returns: (transfer none) (nullable): the URI, or %NULL
 *   if no document is loaded
 *
 * Since: 4.8.0
 */
const gchar     *ev_document_get_uri              (EvDocument      *document);

/**
 * ev_document_get_title:
 * @document: an #EvDocument
 *
 * Returns the title of the document (from the document's
 * metadata, not the filename).  The string is owned by the
 * document and must not be freed.
 *
 * Returns: (transfer none) (nullable): the title, or %NULL
 *
 * Since: 4.8.0
 */
const gchar     *ev_document_get_title            (EvDocument      *document);

/**
 * ev_document_is_page_size_uniform:
 * @document: an #EvDocument
 *
 * Returns whether all pages in @document have the same size.
 * The shell uses this to decide whether to show a "fit page
 * width" / "fit page height" toggle.
 *
 * Returns: %TRUE if all pages have the same size
 *
 * Since: 4.8.0
 */
gboolean         ev_document_is_page_size_uniform (EvDocument      *document);

/**
 * ev_document_get_max_page_size:
 * @document: an #EvDocument
 * @width: (out): return location for the maximum page width
 * @height: (out): return location for the maximum page height
 *
 * Fetches the largest page size in @document.  Useful for
 * sizing the scroll view.
 *
 * Since: 4.8.0
 */
void             ev_document_get_max_page_size    (EvDocument      *document,
						   gdouble         *width,
						   gdouble         *height);

/**
 * ev_document_get_min_page_size:
 * @document: an #EvDocument
 * @width: (out): return location for the minimum page width
 * @height: (out): return location for the minimum page height
 *
 * Fetches the smallest page size in @document.
 *
 * Since: 4.8.0
 */
void             ev_document_get_min_page_size    (EvDocument      *document,
						   gdouble         *width,
						   gdouble         *height);

/**
 * ev_document_check_dimensions:
 * @document: an #EvDocument
 *
 * Returns whether the document's pages have valid (positive,
 * finite) dimensions.  Used to detect malformed documents
 * before rendering.
 *
 * Returns: %TRUE if all pages have valid dimensions
 *
 * Since: 4.8.0
 */
gboolean         ev_document_check_dimensions     (EvDocument      *document);

/**
 * ev_document_get_max_label_len:
 * @document: an #EvDocument
 *
 * Returns the maximum length (in characters) of the
 * printed page labels in @document.  The shell uses this
 * to size the page-number input box.
 *
 * Returns: the max label length
 *
 * Since: 4.8.0
 */
gint             ev_document_get_max_label_len    (EvDocument      *document);

/**
 * ev_document_has_text_page_labels:
 * @document: an #EvDocument
 *
 * Returns whether the document's page labels contain
 * non-numeric characters (e.g. "iv", "B-12").  Documents
 * with all-numeric labels are treated as "1 of N" by the
 * shell.
 *
 * Returns: %TRUE if any page has a non-numeric label
 *
 * Since: 4.8.0
 */
gboolean         ev_document_has_text_page_labels (EvDocument      *document);

/**
 * ev_document_find_page_by_label:
 * @document: an #EvDocument
 * @page_label: the printed page label to find
 * @page_index: (out) (nullable): return location for the
 *   page index, or %NULL
 *
 * Resolves @page_label to a 0-based page index.  The
 * comparison is case-insensitive and accepts the label
 * with or without the document's number prefix (e.g. "12"
 * matches "Page 12").
 *
 * Returns: %TRUE on success, %FALSE if @page_label is not found
 *
 * Since: 4.8.0
 */
gboolean         ev_document_find_page_by_label   (EvDocument      *document,
						   const gchar     *page_label,
						   gint            *page_index);

/**
 * ev_document_has_synctex:
 * @document: an #EvDocument
 *
 * Returns whether the document has SyncTeX data (the
 * inverse-search information that lets the shell jump
 * from a PDF position to the LaTeX source).
 *
 * Returns: %TRUE if SyncTeX data is present
 *
 * Since: 4.8.0
 */
gboolean	 ev_document_has_synctex 	  (EvDocument      *document);

/**
 * ev_document_synctex_backward_search:
 * @document: an #EvDocument
 * @page_index: the 0-based page index
 * @x: the X coordinate on the page, in points
 * @y: the Y coordinate on the page, in points
 *
 * Performs a SyncTeX "backward search": given a position
 * on a PDF page, returns the source LaTeX file + line +
 * column that produced the position.
 *
 * Returns: (transfer full) (nullable): a new #EvSourceLink, or %NULL
 *   if no SyncTeX data is available
 *
 * Since: 4.8.0
 */
EvSourceLink    *ev_document_synctex_backward_search
                                                  (EvDocument      *document,
                                                   gint             page_index,
                                                   gfloat           x,
                                                   gfloat           y);

/**
 * ev_document_synctex_forward_search:
 * @document: an #EvDocument
 * @source_link: the source location to find in the PDF
 *
 * Performs a SyncTeX "forward search": given a source
 * LaTeX position, returns the #EvMapping list (one entry
 * per hit) with the corresponding PDF page + coordinates.
 *
 * Returns: (transfer full) (element-type EvMapping) (nullable):
 *   a new GList of #EvMapping, or %NULL
 *
 * Since: 4.8.0
 */
EvMapping       *ev_document_synctex_forward_search
                                                  (EvDocument      *document,
						   EvSourceLink    *source_link);

/**
 * ev_rect_cmp:
 * @a: the first #EvRectangle
 * @b: the second #EvRectangle
 *
 * Compares two rectangles for sorting.  The ordering is
 * first by y1 (top), then by x1 (left).  This is the
 * canonical "reading order" used by the link-mapping
 * code in the poppler backend.
 *
 * Returns: -1, 0, or +1 if @a is before, equal, or after @b
 *
 * Since: 4.8.0
 */
gint             ev_rect_cmp                      (EvRectangle     *a,
					           EvRectangle     *b);

/**
 * ev_rectangle_intersect:
 * @a: the first #EvRectangle
 * @b: the second #EvRectangle
 *
 * Returns whether @a and @b intersect (have a non-empty
 * overlap).  Two rectangles that touch on a side but do
 * not overlap are NOT considered intersecting.
 *
 * Returns: %TRUE if @a and @b intersect
 *
 * Since: 4.8.0
 */
gboolean         ev_rectangle_intersect           (const EvRectangle *a,
                                                   const EvRectangle *b);

/**
 * ev_document_toggle_night_mode:
 * @document: an #EvDocument
 * @night: %TRUE to enable night mode, %FALSE to disable
 *
 * Toggles the document's night-mode color inversion.  When
 * enabled, all pages are rendered with their colors inverted
 * (light becomes dark, dark becomes light) for easier reading
 * in low-light environments.
 *
 * Since: 4.8.0
 */
void            ev_document_toggle_night_mode     (EvDocument *document,gboolean night);

/**
 * ev_document_check_add_night_sheet:
 * @document: an #EvDocument
 *
 * Adds a transparent overlay sheet to @document to support
 * night-mode color inversion.  This is called internally by
 * the shell when night mode is first enabled.
 *
 * Since: 4.8.0
 */
void			ev_document_check_add_night_sheet (EvDocument *document);

#define EV_TYPE_RECTANGLE (ev_rectangle_get_type ())
/**
 * EvRectangle:
 * @x1: the left coordinate, in points
 * @y1: the top coordinate, in points
 * @x2: the right coordinate, in points
 * @y2: the bottom coordinate, in points
 *
 * A simple rectangle in PDF coordinate space (origin at the
 * top-left, Y increases downward).  Used for link areas,
 * selection rectangles, page render targets, etc.
 *
 * Since: 4.8.0
 */
struct _EvRectangle
{
	gdouble x1;
	gdouble y1;
	gdouble x2;
	gdouble y2;
};

GType        ev_rectangle_get_type (void) G_GNUC_CONST;

/**
 * ev_rectangle_new:
 *
 * Creates a new #EvRectangle with all four coordinates
 * initialized to 0.0.  Free with ev_rectangle_free().
 *
 * Returns: (transfer full): a new #EvRectangle
 *
 * Since: 4.8.0
 */
EvRectangle *ev_rectangle_new      (void);

/**
 * ev_rectangle_copy:
 * @ev_rect: the #EvRectangle to copy
 *
 * Makes a deep copy of @ev_rect.
 *
 * Returns: (transfer full): a new #EvRectangle, free with
 *   ev_rectangle_free()
 *
 * Since: 4.8.0
 */
EvRectangle *ev_rectangle_copy     (EvRectangle *ev_rect);

/**
 * ev_rectangle_free:
 * @ev_rect: (nullable): the #EvRectangle to free
 *
 * Frees @ev_rect.  If @ev_rect is %NULL, this is a no-op.
 *
 * Since: 4.8.0
 */
void         ev_rectangle_free     (EvRectangle *ev_rect);

/**
 * EvMapping:
 * @area: the rectangle where @data is rendered
 * @data: an opaque pointer (e.g. a #GdkPixbuf, an #EvImage,
 *   an #EvLink, etc.)
 *
 * A "mapping" is a rectangle + an opaque data pointer.
 * The libdocument API uses #EvMapping lists to return
 * multiple results (e.g. the SyncTeX forward search returns
 * a list of #EvMapping, one per hit).
 *
 * The data pointer is owned by the caller -- free it with
 * the appropriate free function for the type.
 *
 * Since: 4.8.0
 */
struct _EvMapping {
	EvRectangle area;
	gpointer    data;
};

#define EV_TYPE_SOURCE_LINK (ev_source_link_get_type ())
/**
 * EvSourceLink:
 * @filename: the LaTeX source filename
 * @line: the 0-based line number
 * @col: the 0-based column number
 *
 * A "source link" is a position in a LaTeX source file.
 * Used by the SyncTeX backward search (the document
 * returns a #EvSourceLink for a given PDF position) and
 * by the SyncTeX forward search (the document takes a
 * #EvSourceLink and returns a #EvMapping list).
 *
 * The @filename string is owned by the #EvSourceLink;
 * free it with ev_source_link_free().
 *
 * Since: 4.8.0
 */
struct _EvSourceLink
{
        gchar *filename;
        gint   line;
        gint   col;
};

GType          ev_source_link_get_type (void) G_GNUC_CONST;

/**
 * ev_source_link_new:
 * @filename: the source filename (a copy is made)
 * @line: the 0-based line number
 * @col: the 0-based column number
 *
 * Creates a new #EvSourceLink.  Free with ev_source_link_free().
 *
 * Returns: (transfer full): a new #EvSourceLink
 *
 * Since: 4.8.0
 */
EvSourceLink  *ev_source_link_new      (const gchar *filename,
					gint         line,
					gint         col);

/**
 * ev_source_link_copy:
 * @link: the #EvSourceLink to copy
 *
 * Makes a deep copy of @link.
 *
 * Returns: (transfer full): a new #EvSourceLink
 *
 * Since: 4.8.0
 */
EvSourceLink  *ev_source_link_copy     (EvSourceLink *link);

/**
 * ev_source_link_free:
 * @link: (nullable): the #EvSourceLink to free
 *
 * Frees @link.  If @link is %NULL, this is a no-op.
 *
 * Since: 4.8.0
 */
void           ev_source_link_free     (EvSourceLink *link);

/* convenience macro to ease interface addition in the CODE
 * section of EV_BACKEND_REGISTER_WITH_CODE (this macro relies on
 * the g_define_type_id present within EV_BACKEND_REGISTER_WITH_CODE()).
 * usage example:
 * EV_BACKEND_REGISTER_WITH_CODE (PdfDocument, pdf_document,
 *                          EV_BACKEND_IMPLEMENT_INTERFACE (EV_TYPE_DOCUMENT_THUMBNAILS,
 *                                                 pdf_document_document_thumbnails_iface_init));
 */
#define EV_BACKEND_IMPLEMENT_INTERFACE(TYPE_IFACE, iface_init) {                \
	const GInterfaceInfo g_implement_interface_info = {                     \
		(GInterfaceInitFunc) iface_init, NULL, NULL                     \
	};                                                                      \
	g_type_module_add_interface (module,                                    \
				     g_define_type_id,                          \
				     TYPE_IFACE,                                \
				     &g_implement_interface_info);              \
}

/*
 * Utility macro used to register backends
 *
 * use: EV_BACKEND_REGISTER_WITH_CODE(BackendName, backend_name, CODE)
 */
#define EV_BACKEND_REGISTER_WITH_CODE(BackendName, backend_name, CODE)	        \
										\
static GType g_define_type_id = 0;						\
										\
GType										\
backend_name##_get_type (void)							\
{										\
	return g_define_type_id;						\
}										\
										\
static void     backend_name##_init              (BackendName        *self);	\
static void     backend_name##_class_init        (BackendName##Class *klass);	\
static gpointer backend_name##_parent_class = NULL;				\
static void     backend_name##_class_intern_init (gpointer klass)		\
{										\
	backend_name##_parent_class = g_type_class_peek_parent (klass);		\
	backend_name##_class_init ((BackendName##Class *) klass);		\
}										\
										\
G_MODULE_EXPORT GType								\
register_xreader_backend (GTypeModule *module)					\
{										\
	const GTypeInfo our_info = {  				                \
		sizeof (BackendName##Class),					\
		NULL, /* base_init */						\
		NULL, /* base_finalize */					\
		(GClassInitFunc) backend_name##_class_intern_init,		\
		NULL,								\
		NULL, /* class_data */						\
		sizeof (BackendName),						\
		0, /* n_preallocs */						\
		(GInstanceInitFunc) backend_name##_init				\
	};									\
										\
	/* Initialise the i18n stuff */						\
	bindtextdomain (GETTEXT_PACKAGE, XREADER_LOCALE_DIR);			\
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");			\
                                                                                \
	g_define_type_id = g_type_module_register_type (module,		        \
					                EV_TYPE_DOCUMENT,	\
					                #BackendName,		\
					                &our_info,		\
					                (GTypeFlags)0);	        \
							                        \
	CODE									\
										\
	return g_define_type_id;						\
}

/*
 * Utility macro used to register backend
 *
 * use: EV_BACKEND_REGISTER(BackendName, backend_name)
 */
#define EV_BACKEND_REGISTER(BackendName, backend_name)			\
	EV_BACKEND_REGISTER_WITH_CODE(BackendName, backend_name, ;)

G_END_DECLS

#endif /* EV_DOCUMENT_H */
