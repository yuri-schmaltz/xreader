/* ev-mapping.h
 *  this file is part of xreader, a mate document viewer
 *
 * Copyright (C) 2009 Carlos Garcia Campos <carlosgc@gnome.org>
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

#ifndef EV_MAPPING_LIST_H
#define EV_MAPPING_LIST_H

#include "ev-document.h"

G_BEGIN_DECLS

/**
 * EvMappingList:
 *
 * A refcounted list of #EvMapping with an associated page
 * index.  The list is sorted in reading order (top-to-bottom,
 * left-to-right) by the constructors.
 *
 * The #EvPageCache uses #EvMappingList to return link /
 * image / form-field / annotation mappings for a given page.
 *
 * Since: 4.8.0
 */
typedef struct _EvMappingList EvMappingList;

/**
 * ev_mapping_list_new:
 * @page: the 0-based page index
 * @list: (transfer full) (element-type EvMapping): the list of #EvMapping
 * @data_destroy_func: (nullable): the #GDestroyNotify for the
 *   data pointer of each #EvMapping, or %NULL
 *
 * Creates a new #EvMappingList.  The list is sorted in
 * reading order (top-to-bottom, left-to-right) by the
 * constructor.
 *
 * Returns: (transfer full): a new #EvMappingList
 *
 * Since: 4.8.0
 */
EvMappingList *ev_mapping_list_new         (guint          page,
					    GList         *list,
					    GDestroyNotify data_destroy_func);

/**
 * ev_mapping_list_ref:
 * @mapping_list: an #EvMappingList
 *
 * Increments the refcount.
 *
 * Returns: (transfer full): the same #EvMappingList
 *
 * Since: 4.8.0
 */
EvMappingList *ev_mapping_list_ref         (EvMappingList *mapping_list);

/**
 * ev_mapping_list_unref:
 * @mapping_list: an #EvMappingList
 *
 * Decrements the refcount.  When the refcount reaches
 * zero, frees the list and the data pointers via the
 * GDestroyNotify.
 *
 * Since: 4.8.0
 */
void           ev_mapping_list_unref       (EvMappingList *mapping_list);

/**
 * ev_mapping_list_get_page:
 * @mapping_list: an #EvMappingList
 *
 * Returns: the 0-based page index
 *
 * Since: 4.8.0
 */
guint          ev_mapping_list_get_page    (EvMappingList *mapping_list);

/**
 * ev_mapping_list_get_list:
 * @mapping_list: an #EvMappingList
 *
 * Returns: (transfer none) (element-type EvMapping): the GList
 *
 * Since: 4.8.0
 */
GList         *ev_mapping_list_get_list    (EvMappingList *mapping_list);

/**
 * ev_mapping_list_find:
 * @mapping_list: an #EvMappingList
 * @data: the data pointer to find
 *
 * Finds the first #EvMapping whose data pointer equals @data.
 *
 * Returns: (transfer none) (nullable): the #EvMapping, or %NULL
 *
 * Since: 4.8.0
 */
EvMapping     *ev_mapping_list_find        (EvMappingList *mapping_list,
					    gconstpointer  data);

/**
 * ev_mapping_list_find_custom:
 * @mapping_list: an #EvMappingList
 * @data: the user data to pass to @func
 * @func: the #GCompareFunc
 *
 * Finds the first #EvMapping for which @func returns 0.
 *
 * Returns: (transfer none) (nullable): the #EvMapping, or %NULL
 *
 * Since: 4.8.0
 */
EvMapping     *ev_mapping_list_find_custom (EvMappingList *mapping_list,
					    gconstpointer  data,
					    GCompareFunc   func);

/**
 * ev_mapping_list_get_data:
 * @mapping_list: an #EvMappingList
 * @x: the X coordinate, in points
 * @y: the Y coordinate, in points
 *
 * Returns the data pointer of the first #EvMapping whose
 * rectangle contains (@x, @y), or %NULL if no #EvMapping
 * contains the point.
 *
 * Returns: (transfer none) (nullable): the data pointer, or %NULL
 *
 * Since: 4.8.0
 */
gpointer       ev_mapping_list_get_data    (EvMappingList *mapping_list,
					    gdouble        x,
					    gdouble        y);

/**
 * ev_mapping_list_remove:
 * @mapping_list: an #EvMappingList
 * @mapping: the #EvMapping to remove
 *
 * Removes @mapping from the list.  The data pointer is
 * freed via the GDestroyNotify.
 *
 * Since: 4.8.0
 */
void           ev_mapping_list_remove      (EvMappingList *mapping_list,
					    EvMapping     *mapping);

/**
 * ev_mapping_list_nth:
 * @mapping_list: an #EvMappingList
 * @n: the 0-based index
 *
 * Returns: (transfer none) (nullable): the @n-th #EvMapping, or %NULL
 *
 * Since: 4.8.0
 */
EvMapping     *ev_mapping_list_nth         (EvMappingList *mapping_list,
                                            guint          n);

/**
 * ev_mapping_list_length:
 * @mapping_list: an #EvMappingList
 *
 * Returns: the number of #EvMapping in the list
 *
 * Since: 4.8.0
 */
guint          ev_mapping_list_length      (EvMappingList *mapping_list);

G_END_DECLS

#endif /* EV_MAPPING_LIST_H */
