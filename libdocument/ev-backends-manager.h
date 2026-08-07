/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2007 Carlos Garcia Campos <carlosgc@gnome.org>
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

#ifndef EV_BACKENDS_MANAGER
#define EV_BACKENDS_MANAGER

#include <glib.h>

#include "ev-document.h"
#include "ev-macros.h"

G_BEGIN_DECLS

/**
 * EvTypeInfo:
 * @desc: a human-readable description (e.g. "PDF Document")
 * @mime_types: (array zero-terminated=1): the list of MIME
 *   types (e.g. "application/pdf")
 *
 * Per-backend metadata.  Returned by
 * ev_backends_manager_get_all_types_info().
 *
 * Since: 4.8.0
 */
typedef struct _EvTypeInfo {
	const gchar  *desc;
	const gchar **mime_types;
} EvTypeInfo;

/**
 * _ev_backends_manager_init:
 *
 * Initializes the backend modules.  Called once at application
 * startup.  Loads all backend .so modules from the backends
 * directory.
 *
 * Returns: %TRUE on success, %FALSE on failure
 *
 * Since: 4.8.0
 */
gboolean    _ev_backends_manager_init                     (void);

/**
 * _ev_backends_manager_shutdown:
 *
 * Unloads the backend modules.  Called once at application
 * shutdown.
 *
 * Since: 4.8.0
 */
void        _ev_backends_manager_shutdown                 (void);

/**
 * ev_backends_manager_get_document:
 * @mime_type: the MIME type of the document to load
 *
 * Returns a new #EvDocument that handles @mime_type, or
 * %NULL if no backend supports the MIME type.
 *
 * Returns: (transfer full) (nullable): a new #EvDocument
 *
 * Since: 4.8.0
 */
EvDocument  *ev_backends_manager_get_document             (const gchar *mime_type);

/**
 * ev_backends_manager_get_document_module_name:
 * @document: an #EvDocument
 *
 * Returns: (transfer none) (nullable): the GModule name of
 *   the backend that provided @document, or %NULL
 *
 * Deprecated: 4.8.0: Backends are now private.
 *
 * Since: 4.8.0
 */
EV_DEPRECATED
const gchar *ev_backends_manager_get_document_module_name (EvDocument  *document);

/**
 * ev_backends_manager_get_document_type_info:
 * @document: an #EvDocument
 *
 * Returns: (transfer none) (nullable): the #EvTypeInfo of
 *   the backend that provided @document, or %NULL
 *
 * Deprecated: 4.8.0: Backends are now private.
 *
 * Since: 4.8.0
 */
EV_DEPRECATED
EvTypeInfo  *ev_backends_manager_get_document_type_info   (EvDocument  *document);

/**
 * ev_backends_manager_get_all_types_info:
 *
 * Returns the list of all #EvTypeInfo, one per loaded backend.
 *
 * Returns: (transfer full) (element-type EvTypeInfo): the list
 *
 * Since: 4.8.0
 */
GList       *ev_backends_manager_get_all_types_info       (void);

/**
 * ev_backends_manager_get_backends_dir:
 *
 * Returns the directory where backend .so modules are
 * loaded from.
 *
 * Returns: (transfer none): the backends directory path
 *
 * Since: 4.8.0
 */
const gchar *ev_backends_manager_get_backends_dir         (void);
G_END_DECLS

#endif /* EV_BACKENDS_MANAGER */
