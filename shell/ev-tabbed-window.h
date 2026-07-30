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

#if !defined (__EV_XREADER_SHELL_H_INSIDE__) && !defined (XREADER_COMPILATION)
#error "Only <xreader-shell.h> can be included directly."
#endif

#ifndef EV_TABBED_WINDOW_H
#define EV_TABBED_WINDOW_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include "ev-tab-manager.h"

G_BEGIN_DECLS

#define EV_TYPE_TABBED_WINDOW (ev_tabbed_window_get_type ())
#define EV_TABBED_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_TABBED_WINDOW, EvTabbedWindow))
#define EV_IS_TABBED_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_TABBED_WINDOW))

typedef struct _EvTabbedWindow EvTabbedWindow;
typedef struct _EvTabbedWindowClass EvTabbedWindowClass;

/**
 * EvTabbedWindow:
 *
 * A top-level window that hosts multiple documents in tabs.
 * The user-visible tab bar is shown when the window has
 * ≥ 2 tabs; with 1 tab the notebook is hidden (the single
 * document fills the window like a normal EvWindow).
 *
 * The window owns an EvTabManager (the data model) and a
 * GtkNotebook (the view); the two are kept in sync via
 * signal handlers.
 *
 * The window implements the "C3" tabbed view feature.
 *
 * Since: 4.8.0
 */
struct _EvTabbedWindow
{
	GtkApplicationWindow parent;

	/*< private >*/
	EvTabbedWindowPrivate *priv;
};

struct _EvTabbedWindowClass
{
	GtkApplicationWindowClass parent_class;
};

GType         ev_tabbed_window_get_type  (void) G_GNUC_CONST;

GtkWidget    *ev_tabbed_window_new       (GtkApplication *app);

/* The EvTabManager owned by this window.  Returns a borrowed
 * reference. */
EvTabManager *ev_tabbed_window_get_tab_manager (EvTabbedWindow *window);

/* The active tab's underlying EvView (the rendering widget).
 * Returns a borrowed reference, or NULL if there is no
 * active tab. */
GtkWidget    *ev_tabbed_window_get_active_view (EvTabbedWindow *window);

/* The active tab's document.  Returns a borrowed reference,
 * or NULL if there is no active tab. */
EvDocument   *ev_tabbed_window_get_active_document (EvTabbedWindow *window);

/* Open a new tab at the end of the list, loading the file at
 * the given GFile.  Returns the new tab, or NULL on error.
 * If a tab already holds the same file, the existing tab is
 * focused (and the new tab is not opened). */
EvTab        *ev_tabbed_window_open_file (EvTabbedWindow *window,
                                          GFile         *file,
                                          GError       **error);

/* Close the active tab.  Returns TRUE if a tab was closed,
 * FALSE if there are no tabs to close (and the window should
 * be closed instead). */
gboolean      ev_tabbed_window_close_active_tab (EvTabbedWindow *window);

/* Select the next / previous tab (wraps around). */
void          ev_tabbed_window_select_next_tab   (EvTabbedWindow *window);
void          ev_tabbed_window_select_prev_tab   (EvTabbedWindow *window);

/* Whether the tab bar is currently visible (only shown when
 * the window has ≥ 2 tabs). */
gboolean      ev_tabbed_window_get_tab_bar_visible (EvTabbedWindow *window);

G_END_DECLS

#endif /* EV_TABBED_WINDOW_H */
