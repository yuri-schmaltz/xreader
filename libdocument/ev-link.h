/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2005 Red Hat, Inc.
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

#ifndef EV_LINK_H
#define EV_LINK_H

#include <glib-object.h>
#include "ev-document.h"
#include "ev-link-action.h"

G_BEGIN_DECLS

typedef struct _EvLink EvLink;
typedef struct _EvLinkClass EvLinkClass;
typedef struct _EvLinkPrivate EvLinkPrivate;

#define EV_TYPE_LINK              (ev_link_get_type())
#define EV_LINK(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_LINK, EvLink))
#define EV_LINK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_LINK, EvLinkClass))
#define EV_IS_LINK(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_LINK))
#define EV_IS_LINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_LINK))
#define EV_LINK_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_LINK, EvLinkClass))

/**
 * EvLink:
 *
 * The #EvLink object represents a single hyperlink extracted from
 * a document (PDF, DjVu, ...).  Each link has a human-readable
 * title (e.g. "Chapter 1") and an #EvLinkAction that describes
 * where the link points to (a URL, a named destination, a launch
 * command, etc.).
 *
 * The #EvLink is part of the "links" interface family; the
 * document-specific backends (poppler, djvu) populate the
 * fields on creation and the shell sidebar reads them back to
 * build the bookmarks / table-of-contents UI.
 *
 * Since: 4.8.0
 */
GType         ev_link_get_type	 (void) G_GNUC_CONST;

/**
 * ev_link_new:
 * @title: (nullable): the human-readable title of the link, or %NULL
 * @action: (transfer none): the #EvLinkAction that describes the
 *   destination
 *
 * Creates a new #EvLink with the given @title and @action.
 * The @action reference is owned by the caller (it is NOT
 * reffed by the link -- the link holds a borrowed reference).
 *
 * Returns: (transfer full): a newly-allocated #EvLink, free
 *   with g_object_unref()
 *
 * Since: 4.8.0
 */
EvLink	     *ev_link_new	 (const gchar  *title,
				  EvLinkAction *action);

/**
 * ev_link_get_title:
 * @self: an #EvLink
 *
 * Returns the human-readable title of the link (e.g. "Chapter 1"
 * or "Section 3.2").  The string is owned by the link and
 * must not be freed.
 *
 * Returns: (transfer none) (nullable): the title, or %NULL
 *
 * Since: 4.8.0
 */
const gchar  *ev_link_get_title  (EvLink       *self);

/**
 * ev_link_get_action:
 * @self: an #EvLink
 *
 * Returns the #EvLinkAction describing where the link points
 * to.  The action is owned by the link and must not be
 * unreffed.
 *
 * Returns: (transfer none) (nullable): the action, or %NULL
 *   if the link is not actionable
 *
 * Since: 4.8.0
 */
EvLinkAction *ev_link_get_action (EvLink       *self);

G_END_DECLS

#endif /* !EV_LINK_H */
