/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2008 Carlos Garcia Campos <carlosgc@gnome.org>
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

#ifndef EV_PAGE_H
#define EV_PAGE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define EV_TYPE_PAGE              (ev_page_get_type())
#define EV_PAGE(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_PAGE, EvPage))
#define EV_PAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_PAGE, EvPageClass))
#define EV_IS_PAGE(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_PAGE))
#define EV_IS_PAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_PAGE))
#define EV_PAGE_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_PAGE, EvPageClass))

typedef struct _EvPage        EvPage;
typedef struct _EvPageClass   EvPageClass;

typedef gpointer       EvBackendPage;
typedef GDestroyNotify EvBackendPageDestroyFunc;

struct _EvPage {
	GObject base_instance;
	
	gint index;

	EvBackendPage            backend_page;
	EvBackendPageDestroyFunc backend_destroy_func;
};

struct _EvPageClass {
	GObjectClass base_class;
};

GType   ev_page_get_type (void) G_GNUC_CONST;

/**
 * ev_page_new:
 * @index: the page index (0-based)
 *
 * Creates a new #EvPage wrapping a backend-specific page handle
 * (EvBackendPage is a gpointer, typed by the backend that
 * created it -- see ev-poppler-page.c for the poppler
 * implementation, ev-dvi-page.c for the dvi implementation, ...).
 * The #EvPage is a thin GObject wrapper that adds refcounting +
 * a destroy notify so the backend can free its page handle when
 * the #EvPage is unreferenced.
 *
 * The 0-based @index is the position of the page within the
 * document, used by the rendering layer to address the page
 * regardless of which backend is in use.
 *
 * Returns: (transfer full): a new #EvPage
 */
EvPage *ev_page_new      (gint index);

G_END_DECLS

#endif /* EV_PAGE_H */
