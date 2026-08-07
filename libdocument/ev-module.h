/*
 * ev-module.h
 * This file is part of Xreader
 *
 * Copyright (C) 2005 - Paolo Maggi 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA.
 */
 
/* This is a modified version of gedit-module.h from Epiphany source code.
 * Here the original copyright assignment:
 *
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
 *
 */

/*
 * Modified by the gedit Team, 2005. See the AUTHORS file for a 
 * list of people on the gedit Team.  
 * See the ChangeLog files for a list of changes. 
 *
 * $Id: gedit-module.h 5263 2006-10-08 14:26:02Z pborelli $
 */

/* Modified by Xreader Team */
 
#if !defined (XREADER_COMPILATION)
#error "This is a private header."
#endif

#ifndef EV_MODULE_H
#define EV_MODULE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define EV_TYPE_MODULE            (ev_module_get_type ())
#define EV_MODULE(obj)		  (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_MODULE, EvModule))
#define EV_MODULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EV_TYPE_MODULE, EvModuleClass))
#define EV_IS_MODULE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_MODULE))
#define EV_IS_MODULE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), EV_TYPE_MODULE))
#define EV_MODULE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_MODULE, EvModuleClass))

/**
 * EvModule:
 *
 * The #EvModule is a thin wrapper around GModule that
 * loads a backend .so file and provides a single GType
 * inside it.  The backends-manager creates one #EvModule
 * per loaded backend .so file.
 *
 * Since: 4.8.0
 */
typedef struct _EvModule EvModule;

GType        ev_module_get_type        (void) G_GNUC_CONST;

/**
 * ev_module_new:
 * @path: the path to the .so file
 * @resident: %TRUE to keep the .so loaded after the module
 *   is finalized (useful for hot-reloadable backends)
 *
 * Creates a new #EvModule that wraps @path.
 *
 * Returns: (transfer full): a new #EvModule
 *
 * Since: 4.8.0
 */
EvModule    *ev_module_new             (const gchar *path,
					gboolean     resident);

/**
 * ev_module_get_path:
 * @module: an #EvModule
 *
 * Returns: (transfer none): the path to the .so file
 *
 * Since: 4.8.0
 */
const gchar *ev_module_get_path        (EvModule    *module);

/**
 * ev_module_new_object:
 * @module: an #EvModule
 *
 * Creates a new instance of the GType exported by the .so.
 *
 * Returns: (transfer full) (nullable): a new GObject, or %NULL
 *
 * Since: 4.8.0
 */
GObject     *ev_module_new_object      (EvModule    *module);

/**
 * ev_module_get_object_type:
 * @module: an #EvModule
 *
 * Returns: the GType exported by the .so
 *
 * Since: 4.8.0
 */
GType        ev_module_get_object_type (EvModule    *module);

G_END_DECLS

#endif /* EV_MODULE_H */
