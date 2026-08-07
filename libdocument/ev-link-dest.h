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

#ifndef EV_LINK_DEST_H
#define EV_LINK_DEST_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _EvLinkDest        EvLinkDest;
typedef struct _EvLinkDestClass   EvLinkDestClass;
typedef struct _EvLinkDestPrivate EvLinkDestPrivate;

#define EV_TYPE_LINK_DEST              (ev_link_dest_get_type())
#define EV_LINK_DEST(object)           (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_LINK_DEST, EvLinkDest))
#define EV_LINK_DEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_LINK_DEST, EvLinkDestClass))
#define EV_IS_LINK_DEST(object)        (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_LINK_DEST))
#define EV_IS_LINK_DEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_LINK_DEST))
#define EV_LINK_DEST_GET_CLASS(object) (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_LINK_DEST, EvLinkDestClass))

/**
 * EvLinkDestType:
 * @EV_LINK_DEST_TYPE_PAGE: a "go to page" destination
 *   (only the page index is set)
 * @EV_LINK_DEST_TYPE_XYZ: an "x, y, zoom" destination
 *   (page + left + top + zoom; the change_* flags indicate
 *   which coordinates to apply)
 * @EV_LINK_DEST_TYPE_FIT: a "fit the page" destination
 *   (page; the view is scaled to fit the page)
 * @EV_LINK_DEST_TYPE_FITH: a "fit width" destination
 *   (page + top; the view is scaled to fit the page width)
 * @EV_LINK_DEST_TYPE_FITV: a "fit height" destination
 *   (page + left; the view is scaled to fit the page height)
 * @EV_LINK_DEST_TYPE_FITR: a "fit rectangle" destination
 *   (page + left + bottom + right + top; the view is
 *   scaled to fit the given rectangle)
 * @EV_LINK_DEST_TYPE_NAMED: a "named destination" (a
 *   symbolic name resolved at click-time by the document)
 * @EV_LINK_DEST_TYPE_PAGE_LABEL: a "page label" destination
 *   (the page is identified by its printed label, e.g.
 *   "iv", "B-12", etc.)
 * @EV_LINK_DEST_TYPE_HLINK: an "hyperlink" destination
 *   (the page is identified by the source hyperlink's
 *   named destination; legacy)
 * @EV_LINK_DEST_TYPE_UNKNOWN: an unknown / unsupported
 *   destination type (the action is silently ignored at
 *   click-time)
 *
 * The high-level type of an #EvLinkDest.  This determines
 * which getters are meaningful (e.g. ev_link_dest_get_top()
 * is meaningful only for XYZ / FITH / FITR).
 *
 * Since: 4.8.0
 */
typedef enum {
	EV_LINK_DEST_TYPE_PAGE,
	EV_LINK_DEST_TYPE_XYZ,
	EV_LINK_DEST_TYPE_FIT,
	EV_LINK_DEST_TYPE_FITH,
	EV_LINK_DEST_TYPE_FITV,
	EV_LINK_DEST_TYPE_FITR,
	EV_LINK_DEST_TYPE_NAMED,
	EV_LINK_DEST_TYPE_PAGE_LABEL,
	EV_LINK_DEST_TYPE_HLINK,
	EV_LINK_DEST_TYPE_UNKNOWN
} EvLinkDestType;

/**
 * EvLinkDest:
 *
 * The #EvLinkDest object represents a single hyperlink
 * destination (a page + view coordinates).  An #EvLinkAction
 * of type GOTO_DEST / GOTO_REMOTE carries an #EvLinkDest
 * fetched with ev_link_action_get_dest().
 *
 * Destinations are immutable; once created, the page
 * index and coordinates cannot be changed.  Use the
 * ev_link_dest_new_*() family of constructors to create
 * a destination of the right type.
 *
 * Since: 4.8.0
 */
GType           ev_link_dest_get_type       (void) G_GNUC_CONST;

/**
 * ev_link_dest_get_dest_type:
 * @self: an #EvLinkDest
 *
 * Returns the high-level type of @self.  The caller should
 * dispatch on the result (see the #EvLinkDestType
 * documentation for the mapping).
 *
 * Returns: the #EvLinkDestType
 *
 * Since: 4.8.0
 */
EvLinkDestType  ev_link_dest_get_dest_type  (EvLinkDest  *self);

/**
 * ev_link_dest_get_page:
 * @self: an #EvLinkDest
 *
 * Returns the 0-based page index of @self.  For
 * NAMED destinations, the page index is not known until
 * the named destination is resolved (this function
 * returns -1 in that case; use
 * ev_link_dest_get_named_dest() to fetch the name).
 *
 * Returns: the page index, or -1 if unknown
 *
 * Since: 4.8.0
 */
gint            ev_link_dest_get_page       (EvLinkDest  *self);

/**
 * ev_link_dest_get_top:
 * @self: an #EvLinkDest
 * @change_top: (out) (nullable): return location for the
 *   "change the top coordinate" flag, or %NULL
 *
 * Returns the top coordinate (in PDF points) of @self.
 * Only meaningful for XYZ / FITH / FITR destinations;
 * other types return 0.0.
 *
 * If @change_top is not %NULL, it is set to %TRUE if the
 * caller should apply the top coordinate (i.e. the
 * destination explicitly specifies a top), or %FALSE if
 * the top should be left unchanged.
 *
 * Returns: the top coordinate, in points
 *
 * Since: 4.8.0
 */
gdouble         ev_link_dest_get_top        (EvLinkDest  *self,
					     gboolean    *change_top);

/**
 * ev_link_dest_get_left:
 * @self: an #EvLinkDest
 * @change_left: (out) (nullable): return location for the
 *   "change the left coordinate" flag, or %NULL
 *
 * Returns the left coordinate (in PDF points) of @self.
 * Only meaningful for XYZ / FITV / FITR destinations;
 * other types return 0.0.
 *
 * If @change_left is not %NULL, it is set to %TRUE if the
 * caller should apply the left coordinate.
 *
 * Returns: the left coordinate, in points
 *
 * Since: 4.8.0
 */
gdouble         ev_link_dest_get_left       (EvLinkDest  *self,
					     gboolean    *change_left);

/**
 * ev_link_dest_get_bottom:
 * @self: an #EvLinkDest
 *
 * Returns the bottom coordinate (in PDF points) of @self.
 * Only meaningful for FITR destinations; other types
 * return 0.0.
 *
 * Returns: the bottom coordinate, in points
 *
 * Since: 4.8.0
 */
gdouble         ev_link_dest_get_bottom     (EvLinkDest  *self);

/**
 * ev_link_dest_get_right:
 * @self: an #EvLinkDest
 *
 * Returns the right coordinate (in PDF points) of @self.
 * Only meaningful for FITR destinations; other types
 * return 0.0.
 *
 * Returns: the right coordinate, in points
 *
 * Since: 4.8.0
 */
gdouble         ev_link_dest_get_right      (EvLinkDest  *self);

/**
 * ev_link_dest_get_zoom:
 * @self: an #EvLinkDest
 * @change_zoom: (out) (nullable): return location for the
 *   "change the zoom factor" flag, or %NULL
 *
 * Returns the zoom factor of @self.  Only meaningful for
 * XYZ destinations; other types return 1.0.
 *
 * If @change_zoom is not %NULL, it is set to %TRUE if the
 * caller should apply the zoom factor.
 *
 * Returns: the zoom factor
 *
 * Since: 4.8.0
 */
gdouble         ev_link_dest_get_zoom       (EvLinkDest  *self,
					     gboolean    *change_zoom);

/**
 * ev_link_dest_get_named_dest:
 * @self: an #EvLinkDest
 *
 * Returns the named destination of a NAMED destination.
 * For other types, returns %NULL.
 *
 * The string is owned by the destination and must not be
 * freed.
 *
 * Returns: (transfer none) (nullable): the name, or %NULL
 *
 * Since: 4.8.0
 */
const gchar    *ev_link_dest_get_named_dest (EvLinkDest  *self);

/**
 * ev_link_dest_get_page_label:
 * @self: an #EvLinkDest
 *
 * Returns the page label of a PAGE_LABEL destination
 * (e.g. "iv" for the fourth page in a Roman-numeral
 * document, or "B-12" for an appendix page).
 *
 * The string is owned by the destination and must not be
 * freed.
 *
 * Returns: (transfer none) (nullable): the label, or %NULL
 *
 * Since: 4.8.0
 */
const gchar    *ev_link_dest_get_page_label (EvLinkDest  *self);

/**
 * ev_link_dest_new_page:
 * @page: the 0-based page index
 *
 * Creates a new PAGE destination pointing to @page.
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_page       (gint         page);

/**
 * ev_link_dest_new_xyz:
 * @page: the 0-based page index
 * @left: the left coordinate, in points
 * @top: the top coordinate, in points
 * @zoom: the zoom factor
 * @change_left: whether to apply the left coordinate
 * @change_top: whether to apply the top coordinate
 * @change_zoom: whether to apply the zoom factor
 *
 * Creates a new XYZ destination.  The three @change_*
 * flags tell the view whether to apply each coordinate
 * (e.g. an external TOC entry might only want to set the
 * page, not the view position).
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_xyz        (gint         page,
					     gdouble      left,
					     gdouble      top,
					     gdouble      zoom,
					     gboolean     change_left,
					     gboolean     change_top,
					     gboolean     change_zoom);

/**
 * ev_link_dest_new_fit:
 * @page: the 0-based page index
 *
 * Creates a new FIT destination (the view is scaled to
 * fit the page).
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_fit        (gint         page);

/**
 * ev_link_dest_new_fith:
 * @page: the 0-based page index
 * @top: the top coordinate, in points
 * @change_top: whether to apply the top coordinate
 *
 * Creates a new FITH destination (the view is scaled to
 * fit the page width; the top is optionally applied).
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_fith       (gint         page,
					     gdouble      top,
					     gboolean     change_top);

/**
 * ev_link_dest_new_fitv:
 * @page: the 0-based page index
 * @left: the left coordinate, in points
 * @change_left: whether to apply the left coordinate
 *
 * Creates a new FITV destination (the view is scaled to
 * fit the page height; the left is optionally applied).
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_fitv       (gint         page,
					     gdouble      left,
					     gboolean     change_left);

/**
 * ev_link_dest_new_fitr:
 * @page: the 0-based page index
 * @left: the left coordinate of the fit-rect, in points
 * @bottom: the bottom coordinate of the fit-rect, in points
 * @right: the right coordinate of the fit-rect, in points
 * @top: the top coordinate of the fit-rect, in points
 *
 * Creates a new FITR destination (the view is scaled to
 * fit the given rectangle).
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_fitr       (gint         page,
					     gdouble      left,
					     gdouble      bottom,
					     gdouble      right,
					     gdouble      top);

/**
 * ev_link_dest_new_named:
 * @named_dest: the named destination
 *
 * Creates a new NAMED destination.  A copy of @named_dest
 * is made.
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_named      (const gchar *named_dest);

/**
 * ev_link_dest_new_page_label:
 * @page_label: the printed page label
 *
 * Creates a new PAGE_LABEL destination (the page is
 * identified by its printed label, e.g. "iv" or "B-12").
 * A copy of @page_label is made.
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_page_label (const gchar *page_label);

/**
 * ev_link_dest_new_hlink:
 * @hlink: the source hyperlink's named destination
 * @page: the 0-based page index
 *
 * Creates a new HLINK destination (legacy).  A copy of
 * @hlink is made.
 *
 * Returns: (transfer full): a new #EvLinkDest
 *
 * Since: 4.8.0
 */
EvLinkDest     *ev_link_dest_new_hlink      (const gchar* hlink,gint page);

/**
 * ev_link_dest_equal:
 * @a: (nullable): the first #EvLinkDest
 * @b: (nullable): the second #EvLinkDest
 *
 * Returns whether @a and @b are equal.  Two destinations
 * are equal if they have the same type and the same
 * payload (same page index, same coordinates, same named
 * destination, etc.).
 *
 * Returns: %TRUE if @a and @b are equal
 *
 * Since: 4.8.0
 */
gboolean        ev_link_dest_equal          (EvLinkDest  *a,
                                             EvLinkDest  *b);

G_END_DECLS

#endif /* EV_LINK_DEST_H */
