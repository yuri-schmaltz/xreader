/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2006 Carlos Garcia Campos <carlosgc@gnome.org>
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

#ifndef EV_LINK_ACTION_H
#define EV_LINK_ACTION_H

#include <glib-object.h>
#include "ev-link-dest.h"

G_BEGIN_DECLS

typedef struct _EvLinkAction        EvLinkAction;
typedef struct _EvLinkActionClass   EvLinkActionClass;
typedef struct _EvLinkActionPrivate EvLinkActionPrivate;

#define EV_TYPE_LINK_ACTION              (ev_link_action_get_type())
#define EV_LINK_ACTION(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_LINK_ACTION, EvLinkAction))
#define EV_LINK_ACTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_LINK_ACTION, EvLinkActionClass))
#define EV_IS_LINK_ACTION(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_LINK_ACTION))
#define EV_IS_LINK_ACTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_LINK_ACTION))
#define EV_LINK_ACTION_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_LINK_ACTION, EvLinkActionClass))

/**
 * EvLinkActionType:
 * @EV_LINK_ACTION_TYPE_GOTO_DEST: a "go to destination" action
 *   within the same document (e.g. a TOC entry pointing to
 *   a page).  The destination is fetched with
 *   ev_link_action_get_dest().
 * @EV_LINK_ACTION_TYPE_GOTO_REMOTE: a "go to destination in
 *   another file" action.  Both the target file (filename)
 *   and the destination are set.
 * @EV_LINK_ACTION_TYPE_EXTERNAL_URI: an external URI (http://,
 *   mailto:, ...).  Use ev_link_action_get_uri() to fetch.
 * @EV_LINK_ACTION_TYPE_LAUNCH: a "launch a file" action
 *   (e.g. a PDF that links to a .html file).  The target
 *   file is fetched with ev_link_action_get_filename() and
 *   the command-line parameters with
 *   ev_link_action_get_params().
 * @EV_LINK_ACTION_TYPE_NAMED: a "named action" (PDF spec:
 *   "NextPage", "PrevPage", "FirstPage", "LastPage",
 *   "GoToPage", etc.).  Use ev_link_action_get_name().
 * @EV_LINK_ACTION_TYPE_LAYERS_STATE: a "set layer visibility
 *   state" action.  The action carries 3 GList of layer
 *   names: show, hide, toggle.
 *
 * The high-level type of an #EvLinkAction.  This is the
 * dispatch key for the shell "follow link" handler:
 *
 *   case EV_LINK_ACTION_TYPE_GOTO_DEST:
 *     ev_view_handle_link(ev_view, action);
 *     break;
 *   case EV_LINK_ACTION_TYPE_EXTERNAL_URI:
 *     gtk_show_uri(..., ev_link_action_get_uri(action));
 *     break;
 *   ...
 *
 * Since: 4.8.0
 */
typedef enum {
	EV_LINK_ACTION_TYPE_GOTO_DEST,
	EV_LINK_ACTION_TYPE_GOTO_REMOTE,
	EV_LINK_ACTION_TYPE_EXTERNAL_URI,
	EV_LINK_ACTION_TYPE_LAUNCH,
	EV_LINK_ACTION_TYPE_NAMED,
	EV_LINK_ACTION_TYPE_LAYERS_STATE
	/* We'll probably fill this in more as we support the other types of
	 * actions */
} EvLinkActionType;

/**
 * EvLinkAction:
 *
 * The #EvLinkAction object describes what happens when the
 * user activates a link.  An #EvLink always has exactly one
 * #EvLinkAction; the type of the action determines which
 * getter returns the destination (e.g.
 * ev_link_action_get_dest() for GOTO_DEST, vs.
 * ev_link_action_get_uri() for EXTERNAL_URI).
 *
 * Since: 4.8.0
 */
GType            ev_link_action_get_type         (void) G_GNUC_CONST;

/**
 * ev_link_action_get_action_type:
 * @self: an #EvLinkAction
 *
 * Returns the high-level type of @self.  The caller should
 * dispatch on the result (see the #EvLinkActionType
 * documentation for the mapping).
 *
 * Returns: the #EvLinkActionType
 *
 * Since: 4.8.0
 */
EvLinkActionType ev_link_action_get_action_type  (EvLinkAction *self);

/**
 * ev_link_action_get_dest:
 * @self: an #EvLinkAction
 *
 * Returns the destination of a GOTO_DEST or GOTO_REMOTE
 * action.  For other action types, returns %NULL.
 *
 * The destination is owned by the action and must not be
 * unreffed.
 *
 * Returns: (transfer none) (nullable): the #EvLinkDest, or %NULL
 *
 * Since: 4.8.0
 */
EvLinkDest      *ev_link_action_get_dest         (EvLinkAction *self);

/**
 * ev_link_action_get_uri:
 * @self: an #EvLinkAction
 *
 * Returns the URI of an EXTERNAL_URI action.  For other
 * action types, returns %NULL.
 *
 * The string is owned by the action and must not be freed.
 *
 * Returns: (transfer none) (nullable): the URI, or %NULL
 *
 * Since: 4.8.0
 */
const gchar     *ev_link_action_get_uri          (EvLinkAction *self);

/**
 * ev_link_action_get_filename:
 * @self: an #EvLinkAction
 *
 * Returns the target filename of a GOTO_REMOTE or LAUNCH
 * action.  For other action types, returns %NULL.
 *
 * The string is owned by the action and must not be freed.
 *
 * Returns: (transfer none) (nullable): the filename, or %NULL
 *
 * Since: 4.8.0
 */
const gchar     *ev_link_action_get_filename     (EvLinkAction *self);

/**
 * ev_link_action_get_params:
 * @self: an #EvLinkAction
 *
 * Returns the command-line parameters of a LAUNCH action.
 * For other action types, returns %NULL.
 *
 * The string is owned by the action and must not be freed.
 *
 * Returns: (transfer none) (nullable): the parameters, or %NULL
 *
 * Since: 4.8.0
 */
const gchar     *ev_link_action_get_params       (EvLinkAction *self);

/**
 * ev_link_action_get_name:
 * @self: an #EvLinkAction
 *
 * Returns the name of a NAMED action (e.g. "NextPage",
 * "PrevPage", "FirstPage", "LastPage", "GoToPage",
 * "Print", "Quit", etc.).
 *
 * The string is owned by the action and must not be freed.
 *
 * Returns: (transfer none) (nullable): the name, or %NULL
 *
 * Since: 4.8.0
 */
const gchar     *ev_link_action_get_name         (EvLinkAction *self);

/**
 * ev_link_action_get_show_list:
 * @self: an #EvLinkAction
 *
 * Returns the list of layer names to show (in a
 * LAYERS_STATE action).  Each element is a const-gchar*
 * layer name.
 *
 * The list is owned by the action and must not be freed
 * or modified.
 *
 * Returns: (transfer none) (element-type utf8) (nullable):
 *   the show-list, or %NULL
 *
 * Since: 4.8.0
 */
GList           *ev_link_action_get_show_list    (EvLinkAction *self);

/**
 * ev_link_action_get_hide_list:
 * @self: an #EvLinkAction
 *
 * Returns the list of layer names to hide (in a
 * LAYERS_STATE action).  Each element is a const-gchar*
 * layer name.
 *
 * The list is owned by the action and must not be freed
 * or modified.
 *
 * Returns: (transfer none) (element-type utf8) (nullable):
 *   the hide-list, or %NULL
 *
 * Since: 4.8.0
 */
GList           *ev_link_action_get_hide_list    (EvLinkAction *self);

/**
 * ev_link_action_get_toggle_list:
 * @self: an #EvLinkAction
 *
 * Returns the list of layer names to toggle (in a
 * LAYERS_STATE action).  Each element is a const-gchar*
 * layer name.
 *
 * The list is owned by the action and must not be freed
 * or modified.
 *
 * Returns: (transfer none) (element-type utf8) (nullable):
 *   the toggle-list, or %NULL
 *
 * Since: 4.8.0
 */
GList           *ev_link_action_get_toggle_list  (EvLinkAction *self);

/**
 * ev_link_action_new_dest:
 * @dest: (transfer full): the destination
 *
 * Creates a new GOTO_DEST action with @dest as its
 * destination.  The action takes ownership of @dest
 * (it will unref it when the action is finalized).
 *
 * Returns: (transfer full): a new #EvLinkAction, free
 *   with g_object_unref()
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_dest         (EvLinkDest   *dest);

/**
 * ev_link_action_new_remote:
 * @dest: (transfer full): the destination in the remote file
 * @filename: the target filename
 *
 * Creates a new GOTO_REMOTE action.  The action takes
 * ownership of @dest and makes a copy of @filename.
 *
 * Returns: (transfer full): a new #EvLinkAction
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_remote       (EvLinkDest   *dest,
						  const gchar  *filename);

/**
 * ev_link_action_new_external_uri:
 * @uri: the external URI
 *
 * Creates a new EXTERNAL_URI action with @uri as its
 * destination.  A copy of @uri is made.
 *
 * Returns: (transfer full): a new #EvLinkAction
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_external_uri (const gchar  *uri);

/**
 * ev_link_action_new_launch:
 * @filename: the file to launch
 * @params: (nullable): the command-line parameters, or %NULL
 *
 * Creates a new LAUNCH action.  Copies of @filename and
 * @params are made.
 *
 * Returns: (transfer full): a new #EvLinkAction
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_launch       (const gchar  *filename,
						  const gchar  *params);

/**
 * ev_link_action_new_named:
 * @name: the named action name
 *
 * Creates a new NAMED action with @name as its name.
 * A copy of @name is made.
 *
 * Returns: (transfer full): a new #EvLinkAction
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_named        (const gchar  *name);

/**
 * ev_link_action_new_layers_state:
 * @show_list: (element-type utf8) (transfer full) (nullable):
 *   the list of layer names to show
 * @hide_list: (element-type utf8) (transfer full) (nullable):
 *   the list of layer names to hide
 * @toggle_list: (element-type utf8) (transfer full) (nullable):
 *   the list of layer names to toggle
 *
 * Creates a new LAYERS_STATE action.  The action takes
 * ownership of the three lists (and frees them with
 * g_list_free_full(strings, g_free) when the action is
 * finalized).
 *
 * Returns: (transfer full): a new #EvLinkAction
 *
 * Since: 4.8.0
 */
EvLinkAction    *ev_link_action_new_layers_state (GList        *show_list,
						  GList        *hide_list,
						  GList        *toggle_list);

/**
 * ev_link_action_equal:
 * @a: (nullable): the first #EvLinkAction
 * @b: (nullable): the second #EvLinkAction
 *
 * Returns whether @a and @b are equal.  Two actions are
 * equal if they have the same type and the same payload
 * (same destination / same URI / same name / etc.).
 *
 * Returns: %TRUE if @a and @b are equal
 *
 * Since: 4.8.0
 */
gboolean         ev_link_action_equal            (EvLinkAction *a,
                                                  EvLinkAction *b);

G_END_DECLS

#endif /* EV_LINK_ACTION_H */
