/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2006 Carlos Garcia Campos <carlosgc@gnome.org>
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

#ifndef __EV_IMAGE_H__
#define __EV_IMAGE_H__

#include <glib-object.h>
#include <gdk/gdk.h>
#include "ev-macros.h"

G_BEGIN_DECLS

typedef struct _EvImage        EvImage;
typedef struct _EvImageClass   EvImageClass;
typedef struct _EvImagePrivate EvImagePrivate;

#define EV_TYPE_IMAGE              (ev_image_get_type())
#define EV_IMAGE(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_IMAGE, EvImage))
#define EV_IMAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_IMAGE, EvImageClass))
#define EV_IS_IMAGE(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_IMAGE))
#define EV_IS_IMAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_IMAGE))
#define EV_IMAGE_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_IMAGE, EvImageClass))

/**
 * EvImage:
 *
 * The #EvImage object represents an image embedded in a
 * document.  The image can be loaded from the document
 * (via ev_image_new() + ev_job_attachments) or constructed
 * from an existing #GdkPixbuf (via ev_image_new_from_pixbuf()).
 *
 * Since: 4.8.0
 */
struct _EvImage {
	GObject base_instance;
	
	EvImagePrivate *priv;
};

struct _EvImageClass {
	GObjectClass base_class;
};

GType        ev_image_get_type         (void) G_GNUC_CONST;

/**
 * ev_image_new:
 * @page: the 0-based page index
 * @img_id: the backend-specific image id
 *
 * Creates a new #EvImage that lazily loads the image
 * with the given (page, img_id) pair.  Use
 * ev_image_get_pixbuf() to trigger the actual load.
 *
 * Returns: (transfer full): a new #EvImage
 *
 * Since: 4.8.0
 */
EvImage     *ev_image_new              (gint             page,
					gint             img_id);

/**
 * ev_image_new_from_pixbuf:
 * @pixbuf: (transfer full): the source pixbuf
 *
 * Creates a new #EvImage from an existing #GdkPixbuf.
 * The pixbuf is owned by the new #EvImage.
 *
 * Returns: (transfer full): a new #EvImage
 *
 * Since: 4.8.0
 */
EvImage     *ev_image_new_from_pixbuf  (GdkPixbuf       *pixbuf) G_GNUC_NONNULL (1);

/**
 * ev_image_get_id:
 * @image: an #EvImage
 *
 * Returns: the backend-specific image id
 *
 * Since: 4.8.0
 */
gint         ev_image_get_id           (EvImage         *image) G_GNUC_NONNULL (1);

/**
 * ev_image_get_page:
 * @image: an #EvImage
 *
 * Returns: the 0-based page index
 *
 * Since: 4.8.0
 */
gint         ev_image_get_page         (EvImage         *image) G_GNUC_NONNULL (1);

/**
 * ev_image_get_pixbuf:
 * @image: an #EvImage
 *
 * Returns the image's pixbuf, loading it from the
 * backend on the first call.  The returned pixbuf is
 * owned by the image; do not unref.
 *
 * Returns: (transfer none) (nullable): the pixbuf, or %NULL
 *
 * Since: 4.8.0
 */
GdkPixbuf   *ev_image_get_pixbuf       (EvImage         *image) G_GNUC_NONNULL (1);

/**
 * ev_image_save_tmp:
 * @image: an #EvImage
 * @pixbuf: the pixbuf to save
 *
 * Saves @pixbuf to a temporary file in PNG format and
 * associates the URI with @image.  The temp file is
 * deleted when the application exits.  The returned
 * string must be freed with g_free().
 *
 * Returns: (transfer full) (nullable): the temp file URI, or %NULL
 *
 * Since: 4.8.0
 */
const gchar *ev_image_save_tmp         (EvImage         *image,
					GdkPixbuf       *pixbuf) G_GNUC_NONNULL (1, 2);

/**
 * ev_image_get_tmp_uri:
 * @image: an #EvImage
 *
 * Returns the temp file URI previously set by
 * ev_image_save_tmp().  The string is owned by the
 * image and must not be freed.
 *
 * Returns: (transfer none) (nullable): the temp file URI, or %NULL
 *
 * Since: 4.8.0
 */
const gchar *ev_image_get_tmp_uri      (EvImage         *image) G_GNUC_NONNULL (1);


G_END_DECLS

#endif /* __EV_IMAGE_H__ */
