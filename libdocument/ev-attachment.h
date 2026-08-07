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

#ifndef __EV_ATTACHMENT_H__
#define __EV_ATTACHMENT_H__

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

typedef struct _EvAttachment        EvAttachment;
typedef struct _EvAttachmentClass   EvAttachmentClass;
typedef struct _EvAttachmentPrivate EvAttachmentPrivate;

#define EV_TYPE_ATTACHMENT              (ev_attachment_get_type())
#define EV_ATTACHMENT(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_ATTACHMENT, EvAttachment))
#define EV_ATTACHMENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_ATTACHMENT, EvAttachmentClass))
#define EV_IS_ATTACHMENT(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_ATTACHMENT))
#define EV_IS_ATTACHMENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_ATTACHMENT))
#define EV_ATTACHMENT_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_ATTACHMENT, EvAttachmentClass))

#define EV_ATTACHMENT_ERROR (ev_attachment_error_quark ())

/**
 * EvAttachment:
 *
 * The #EvAttachment object represents a single file
 * embedded in a document (e.g. an image attached to a PDF,
 * a spreadsheet embedded in a Word document).  Attachments
 * are extracted from the document by the backend
 * (poppler, etc.) and shown in the shell attachment
 * sidebar; the user can save them to disk or open them
 * in the default application.
 *
 * Since: 4.8.0
 */
struct _EvAttachment {
	GObject base_instance;
	
	EvAttachmentPrivate *priv;
};

struct _EvAttachmentClass {
	GObjectClass base_class;
};

GType         ev_attachment_get_type             (void) G_GNUC_CONST;

/**
 * ev_attachment_error_quark:
 *
 * Returns: the #GQuark for the #GError domain used by
 *   the #EvAttachment functions
 *
 * Since: 4.8.0
 */
GQuark        ev_attachment_error_quark          (void) G_GNUC_CONST;

/**
 * ev_attachment_new:
 * @name: the human-readable name of the attachment
 * @description: (nullable): a longer description, or %NULL
 * @mtime: (transfer full) (nullable): the modification
 *   time, or %NULL
 * @ctime: (transfer full) (nullable): the creation time,
 *   or %NULL
 * @size: the size in bytes
 * @data: (transfer full) (nullable): the raw data, or %NULL
 *   (data is copied)
 *
 * Creates a new #EvAttachment.  Copies are made of all
 * string / time / data arguments.
 *
 * Returns: (transfer full): a new #EvAttachment
 *
 * Since: 4.8.0
 */
EvAttachment *ev_attachment_new                  (const gchar  *name,
						  const gchar  *description,
						  GDateTime    *mtime,
						  GDateTime    *ctime,
						  gsize         size,
						  gpointer      data);

/**
 * ev_attachment_get_name:
 * @attachment: an #EvAttachment
 *
 * Returns: (transfer none) (nullable): the name, or %NULL
 *
 * Since: 4.8.0
 */
const gchar *ev_attachment_get_name              (EvAttachment *attachment);

/**
 * ev_attachment_get_description:
 * @attachment: an #EvAttachment
 *
 * Returns: (transfer none) (nullable): the description, or %NULL
 *
 * Since: 4.8.0
 */
const gchar *ev_attachment_get_description       (EvAttachment *attachment);

/**
 * ev_attachment_get_modification_date:
 * @attachment: an #EvAttachment
 *
 * Returns: (transfer none) (nullable): the modification
 *   time, or %NULL
 *
 * Since: 4.8.0
 */
GDateTime   *ev_attachment_get_modification_date (EvAttachment *attachment);

/**
 * ev_attachment_get_creation_date:
 * @attachment: an #EvAttachment
 *
 * Returns: (transfer none) (nullable): the creation time, or %NULL
 *
 * Since: 4.8.0
 */
GDateTime   *ev_attachment_get_creation_date     (EvAttachment *attachment);

/**
 * ev_attachment_get_mime_type:
 * @attachment: an #EvAttachment
 *
 * Returns: (transfer none) (nullable): the MIME type, or %NULL
 *
 * Since: 4.8.0
 */
const gchar *ev_attachment_get_mime_type         (EvAttachment *attachment);

/**
 * ev_attachment_save:
 * @attachment: an #EvAttachment
 * @file: the destination #GFile
 * @error: (out) (nullable): return location for a #GError, or %NULL
 *
 * Saves the attachment to @file.
 *
 * Returns: %TRUE on success, %FALSE on error (with @error set)
 *
 * Since: 4.8.0
 */
gboolean     ev_attachment_save                  (EvAttachment *attachment,
						  GFile        *file,
						  GError      **error);

/**
 * ev_attachment_open:
 * @attachment: an #EvAttachment
 * @screen: (nullable): the #GdkScreen to show the attachment on, or %NULL
 * @timestamp: the timestamp of the user interaction that
 *   triggered the open
 * @error: (out) (nullable): return location for a #GError, or %NULL
 *
 * Opens the attachment in the user's default application
 * for the attachment's MIME type.
 *
 * Returns: %TRUE on success, %FALSE on error (with @error set)
 *
 * Since: 4.8.0
 */
gboolean     ev_attachment_open                  (EvAttachment *attachment,
						  GdkScreen    *screen,
						  guint32       timestamp,
						  GError      **error);

G_END_DECLS

#endif /* __EV_ATTACHMENT_H__ */
