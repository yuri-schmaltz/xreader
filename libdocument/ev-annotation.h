/* ev-annotation.h
 *  this file is part of xreader, a generic document viewer
 *
 * Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
 * Copyright (C) 2007 Iñigo Martinez <inigomartinez@gmail.com>
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

#ifndef EV_ANNOTATION_H
#define EV_ANNOTATION_H

#include <glib-object.h>
#include <gdk/gdk.h>

#include "ev-document.h"
#include "ev-attachment.h"
#include "ev-macros.h"

G_BEGIN_DECLS

/* EvAnnotation */
#define EV_TYPE_ANNOTATION                         (ev_annotation_get_type())
#define EV_ANNOTATION(object)                      (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_ANNOTATION, EvAnnotation))
#define EV_ANNOTATION_CLASS(klass)                 (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_ANNOTATION, EvAnnotationClass))
#define EV_IS_ANNOTATION(object)                   (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_ANNOTATION))
#define EV_IS_ANNOTATION_CLASS(klass)              (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_ANNOTATION))
#define EV_ANNOTATION_GET_CLASS(object)            (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_ANNOTATION, EvAnnotationClass))

/* EvAnnotationMarkup */
#define EV_TYPE_ANNOTATION_MARKUP                  (ev_annotation_markup_get_type ())
#define EV_ANNOTATION_MARKUP(o)                    (G_TYPE_CHECK_INSTANCE_CAST ((o), EV_TYPE_ANNOTATION_MARKUP, EvAnnotationMarkup))
#define EV_ANNOTATION_MARKUP_IFACE(k)              (G_TYPE_CHECK_CLASS_CAST((k), EV_TYPE_ANNOTATION_MARKUP, EvAnnotationMarkupInterface))
#define EV_IS_ANNOTATION_MARKUP(o)                 (G_TYPE_CHECK_INSTANCE_TYPE ((o), EV_TYPE_ANNOTATION_MARKUP))
#define EV_IS_ANNOTATION_MARKUP_IFACE(k)           (G_TYPE_CHECK_CLASS_TYPE ((k), EV_TYPE_ANNOTATION_MARKUP))
#define EV_ANNOTATION_MARKUP_GET_IFACE(inst)       (G_TYPE_INSTANCE_GET_INTERFACE ((inst), EV_TYPE_ANNOTATION_MARKUP, EvAnnotationMarkupInterface))

/* EvAnnotationText */
#define EV_TYPE_ANNOTATION_TEXT                    (ev_annotation_text_get_type())
#define EV_ANNOTATION_TEXT(object)                 (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_ANNOTATION_TEXT, EvAnnotationText))
#define EV_ANNOTATION_TEXT_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_ANNOTATION_TEXT, EvAnnotationTextClass))
#define EV_IS_ANNOTATION_TEXT(object)              (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_ANNOTATION_TEXT))
#define EV_IS_ANNOTATION_TEXT_CLASS(klass)         (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_ANNOTATION_TEXT))
#define EV_ANNOTATION_TEXT_GET_CLASS(object)       (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_ANNOTATION_TEXT, EvAnnotationTextClass))

/* EvAnnotationAttachment */
#define EV_TYPE_ANNOTATION_ATTACHMENT              (ev_annotation_attachment_get_type())
#define EV_ANNOTATION_ATTACHMENT(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_ANNOTATION_ATTACHMENT, EvAnnotationAttachment))
#define EV_ANNOTATION_ATTACHMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_ANNOTATION_ATTACHMENT, EvAnnotationAttachmentClass))
#define EV_IS_ANNOTATION_ATTACHMENT(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_ANNOTATION_ATTACHMENT))
#define EV_IS_ANNOTATION_ATTACHMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_ANNOTATION_ATTACHMENT))
#define EV_ANNOTATION_ATTACHMENT_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_ANNOTATION_ATTACHMENT, EvAnnotationAttachmentClass))

/* EvAnnotationTextMarkup */
#define EV_TYPE_ANNOTATION_TEXT_MARKUP              (ev_annotation_text_markup_get_type ())
#define EV_ANNOTATION_TEXT_MARKUP(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), EV_TYPE_ANNOTATION_TEXT_MARKUP, EvAnnotationTextMarkup))
#define EV_ANNOTATION_TEXT_MARKUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), EV_TYPE_ANNOTATION_TEXT_MARKUP, EvAnnotationTextMarkupClass))
#define EV_IS_ANNOTATION_TEXT_MARKUP(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), EV_TYPE_ANNOTATION_TEXT_MARKUP))
#define EV_IS_ANNOTATION_TEXT_MARKUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), EV_TYPE_ANNOTATION_TEXT_MARKUP))
#define EV_ANNOTATION_TEXT_MARKUP_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS ((object), EV_TYPE_ANNOTATION_TEXT_MARKUP, EvAnnotationTextMarkupClass))

typedef struct _EvAnnotation                EvAnnotation;
typedef struct _EvAnnotationClass           EvAnnotationClass;

typedef struct _EvAnnotationMarkup          EvAnnotationMarkup;
typedef struct _EvAnnotationMarkupInterface EvAnnotationMarkupInterface;

typedef struct _EvAnnotationText            EvAnnotationText;
typedef struct _EvAnnotationTextClass       EvAnnotationTextClass;

typedef struct _EvAnnotationAttachment      EvAnnotationAttachment;
typedef struct _EvAnnotationAttachmentClass EvAnnotationAttachmentClass;

typedef struct _EvAnnotationTextMarkup      EvAnnotationTextMarkup;
typedef struct _EvAnnotationTextMarkupClass EvAnnotationTextMarkupClass;

typedef enum {
    EV_ANNOTATION_TYPE_UNKNOWN,
    EV_ANNOTATION_TYPE_TEXT,
	EV_ANNOTATION_TYPE_ATTACHMENT,
	EV_ANNOTATION_TYPE_TEXT_MARKUP
} EvAnnotationType;

typedef enum {
    EV_ANNOTATION_TEXT_ICON_NOTE,
    EV_ANNOTATION_TEXT_ICON_COMMENT,
    EV_ANNOTATION_TEXT_ICON_KEY,
    EV_ANNOTATION_TEXT_ICON_HELP,
    EV_ANNOTATION_TEXT_ICON_NEW_PARAGRAPH,
    EV_ANNOTATION_TEXT_ICON_PARAGRAPH,
    EV_ANNOTATION_TEXT_ICON_INSERT,
    EV_ANNOTATION_TEXT_ICON_CROSS,
    EV_ANNOTATION_TEXT_ICON_CIRCLE,
    EV_ANNOTATION_TEXT_ICON_UNKNOWN
} EvAnnotationTextIcon;

typedef enum {
        EV_ANNOTATION_TEXT_MARKUP_HIGHLIGHT,
        EV_ANNOTATION_TEXT_MARKUP_STRIKE_OUT,
        EV_ANNOTATION_TEXT_MARKUP_UNDERLINE,
        EV_ANNOTATION_TEXT_MARKUP_SQUIGGLY
} EvAnnotationTextMarkupType;

/* EvAnnotation */
/**
 * ev_annotation_get_type:
 *
 * Returns the GType for the EvAnnotation abstract base class.
 *
 * Returns: the GType
 *
 * Since: 1.0
 */
GType                ev_annotation_get_type                  (void) G_GNUC_CONST;

/**
 * ev_annotation_get_annotation_type:
 * @annot: an #EvAnnotation
 *
 * Returns the concrete type of the annotation (text / markup
 * / attachment / text-markup).
 *
 * Returns: the #EvAnnotationType
 *
 * Since: 1.0
 */
EvAnnotationType     ev_annotation_get_annotation_type       (EvAnnotation           *annot);

/**
 * ev_annotation_get_page:
 * @annot: an #EvAnnotation
 *
 * Returns the #EvPage the annotation is attached to.
 * The returned reference is owned by @annot; do not unref.
 *
 * Returns: (transfer none) (nullable): the #EvPage, or %NULL
 *
 * Since: 1.0
 */
EvPage              *ev_annotation_get_page                  (EvAnnotation           *annot);

/**
 * ev_annotation_get_page_index:
 * @annot: an #EvAnnotation
 *
 * Returns the index of the page the annotation is attached to
 * (0-based).  Equivalent to ev_page_get_index() on the
 * ev_annotation_get_page() result, but does not require
 * a NULL check.
 *
 * Returns: the page index, or G_MAXUINT if @annot has no page
 *
 * Since: 1.0
 */
guint                ev_annotation_get_page_index            (EvAnnotation           *annot);

/**
 * ev_annotation_equal:
 * @annot: an #EvAnnotation
 * @other: another #EvAnnotation
 *
 * Returns whether the two annotations are equal (same
 * underlying poppler / djvu / etc. object, not just same
 * field values).  Used by the annotations sidebar to
 * dedup loaded annotations.
 *
 * Returns: %TRUE if equal
 *
 * Since: 1.0
 */
gboolean             ev_annotation_equal                     (EvAnnotation           *annot,
                                                              EvAnnotation           *other);

/**
 * ev_annotation_get_contents:
 * @annot: an #EvAnnotation
 *
 * Returns the text contents of the annotation (the body
 * of a note, the text of a text-markup highlight, etc.).
 * The returned string is owned by @annot; do not free.
 *
 * Returns: (transfer none) (nullable): the contents, or %NULL
 *
 * Since: 1.0
 */
const gchar         *ev_annotation_get_contents              (EvAnnotation           *annot);

/**
 * ev_annotation_set_contents:
 * @annot: an #EvAnnotation
 * @contents: (nullable): the new contents
 *
 * Sets the text contents.  Marks the document as modified
 * (the next save will write the new contents back to the
 * PDF).
 *
 * Returns: %TRUE on success
 *
 * Since: 1.0
 */
gboolean             ev_annotation_set_contents              (EvAnnotation           *annot,
                                                              const gchar            *contents);

/**
 * ev_annotation_get_name:
 * @annot: an #EvAnnotation
 *
 * Returns the unique name of the annotation (used internally
 * by the backends to identify the annotation in the document).
 * The returned string is owned by @annot; do not free.
 *
 * Returns: (transfer none) (nullable): the name, or %NULL
 *
 * Since: 1.0
 */
const gchar         *ev_annotation_get_name                  (EvAnnotation           *annot);

/**
 * ev_annotation_set_name:
 * @annot: an #EvAnnotation
 * @name: (nullable): the new name
 *
 * Sets the unique name.  Names must be unique within a
 * document; setting a duplicate name returns %FALSE.
 *
 * Returns: %TRUE on success
 *
 * Since: 1.0
 */
gboolean             ev_annotation_set_name                  (EvAnnotation           *annot,
                                                              const gchar            *name);

/**
 * ev_annotation_get_modified:
 * @annot: an #EvAnnotation
 *
 * Returns the last-modified date of the annotation, as
 * an ISO 8601 string (the format used by the PDF / DjVu
 * metadata).  The returned string is owned by @annot;
 * do not free.
 *
 * For a time_t-typed version, use g_date_time_new_from_iso8601()
 * on the returned string.
 *
 * Returns: (transfer none) (nullable): the modified date, or %NULL
 *
 * Since: 1.0
 */
const gchar         *ev_annotation_get_modified              (EvAnnotation           *annot);

/**
 * ev_annotation_set_modified:
 * @annot: an #EvAnnotation
 * @modified: (nullable): the new modified date (ISO 8601)
 *
 * Sets the last-modified date.  Use
 * ev_annotation_set_modified_from_time() to set it from
 * a time_t.
 *
 * Returns: %TRUE on success
 *
 * Since: 1.0
 */
gboolean             ev_annotation_set_modified              (EvAnnotation           *annot,
                                                              const gchar            *modified);

/**
 * ev_annotation_set_modified_from_time:
 * @annot: an #EvAnnotation
 * @utime: the new modified time (seconds since epoch)
 *
 * Sets the last-modified date from a time_t.  The conversion
 * to the ISO 8601 string is done with localtime_r().
 *
 * Returns: %TRUE on success
 *
 * Since: 1.0
 */
gboolean             ev_annotation_set_modified_from_time    (EvAnnotation           *annot,
                                                              time_t                  utime);
EV_DEPRECATED_FOR(ev_annotaion_get_rgba)
void                 ev_annotation_get_color                 (EvAnnotation           *annot,
                                                              GdkColor               *color);
EV_DEPRECATED_FOR(ev_annotaion_set_rgba)
gboolean             ev_annotation_set_color                 (EvAnnotation           *annot,
                                                              const GdkColor         *color);
void                 ev_annotation_get_rgba                  (EvAnnotation           *annot,
                                                              GdkRGBA                *rgba);
gboolean             ev_annotation_set_rgba                  (EvAnnotation           *annot,
                                                              const GdkRGBA          *rgba);
void                 ev_annotation_get_area                  (EvAnnotation           *annot,
                                                              EvRectangle            *area);
gboolean             ev_annotation_set_area                  (EvAnnotation           *annot,
                                                              const EvRectangle      *area);

/* EvAnnotationMarkup */
GType                ev_annotation_markup_get_type           (void) G_GNUC_CONST;
const gchar         *ev_annotation_markup_get_label          (EvAnnotationMarkup     *markup);
gboolean             ev_annotation_markup_set_label          (EvAnnotationMarkup     *markup,
                                                              const gchar            *label);
gdouble              ev_annotation_markup_get_opacity        (EvAnnotationMarkup     *markup);
gboolean             ev_annotation_markup_set_opacity        (EvAnnotationMarkup     *markup,
                                                              gdouble                 opacity);
gboolean             ev_annotation_markup_can_have_popup     (EvAnnotationMarkup     *markup);
gboolean             ev_annotation_markup_has_popup          (EvAnnotationMarkup     *markup);
gboolean             ev_annotation_markup_set_has_popup      (EvAnnotationMarkup     *markup,
                                                              gboolean                has_popup);
void                 ev_annotation_markup_get_rectangle      (EvAnnotationMarkup     *markup,
                                                              EvRectangle            *ev_rect);
gboolean             ev_annotation_markup_set_rectangle      (EvAnnotationMarkup     *markup,
                                                              const EvRectangle      *ev_rect);
gboolean             ev_annotation_markup_get_popup_is_open  (EvAnnotationMarkup     *markup);
gboolean             ev_annotation_markup_set_popup_is_open  (EvAnnotationMarkup     *markup,
                                                              gboolean                is_open);

/* EvAnnotationText */
GType                ev_annotation_text_get_type             (void) G_GNUC_CONST;
EvAnnotation        *ev_annotation_text_new                  (EvPage                 *page);
EvAnnotationTextIcon ev_annotation_text_get_icon             (EvAnnotationText       *text);
gboolean             ev_annotation_text_set_icon             (EvAnnotationText       *text,
                                                              EvAnnotationTextIcon    icon);
gboolean             ev_annotation_text_get_is_open          (EvAnnotationText       *text);
gboolean             ev_annotation_text_set_is_open          (EvAnnotationText       *text,
                                                              gboolean                is_open);

/* EvAnnotationAttachment */
GType                ev_annotation_attachment_get_type       (void) G_GNUC_CONST;
EvAnnotation        *ev_annotation_attachment_new            (EvPage                 *page,
                                                              EvAttachment           *attachment);
EvAttachment        *ev_annotation_attachment_get_attachment (EvAnnotationAttachment *annot);
gboolean             ev_annotation_attachment_set_attachment (EvAnnotationAttachment *annot,
                                                              EvAttachment           *attachment);

/* EvAnnotationTextMarkup */
GType                      ev_annotation_text_markup_get_type        (void) G_GNUC_CONST;
EvAnnotation              *ev_annotation_text_markup_highlight_new   (EvPage                    *page);
EvAnnotation              *ev_annotation_text_markup_strike_out_new  (EvPage                    *page);
EvAnnotation              *ev_annotation_text_markup_underline_new   (EvPage                    *page);
EvAnnotation              *ev_annotation_text_markup_squiggly_new    (EvPage                    *page);
EvAnnotationTextMarkupType ev_annotation_text_markup_get_markup_type (EvAnnotationTextMarkup    *annot);
gboolean                   ev_annotation_text_markup_set_markup_type (EvAnnotationTextMarkup    *annot,
                                                                      EvAnnotationTextMarkupType markup_type);

G_END_DECLS

#endif /* EV_ANNOTATION_H */
