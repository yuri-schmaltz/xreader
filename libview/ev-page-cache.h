/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2009 Carlos Garcia Campos
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

#if !defined (__EV_XREADER_VIEW_H_INSIDE__) && !defined (XREADER_COMPILATION)
#error "Only <xreader-view.h> can be included directly."
#endif

#ifndef EV_PAGE_CACHE_H
#define EV_PAGE_CACHE_H

#include <glib-object.h>
#include <gdk/gdk.h>
#include <xreader-document.h>
#include <xreader-view.h>

G_BEGIN_DECLS

#define EV_TYPE_PAGE_CACHE    (ev_page_cache_get_type ())
#define EV_PAGE_CACHE(obj)    (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_PAGE_CACHE, EvPageCache))
#define EV_IS_PAGE_CACHE(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_PAGE_CACHE))

/**
 * EvPageCache:
 *
 * The #EvPageCache object caches the per-page data (links,
 * images, form fields, annotations, text) of an #EvDocument.
 * Used by the #EvView to lazily load page data as the
 * user scrolls.
 *
 * Since: 4.8.0
 */
typedef struct _EvPageCache        EvPageCache;
typedef struct _EvPageCacheClass   EvPageCacheClass;

GType              ev_page_cache_get_type               (void) G_GNUC_CONST;

/**
 * ev_page_cache_new:
 * @document: the #EvDocument to cache
 *
 * Creates a new #EvPageCache for @document.  Use
 * ev_page_cache_set_page_range() to restrict the cache
 * to a range of pages (e.g. the current view).
 *
 * Returns: (transfer full): a new #EvPageCache
 *
 * Since: 4.8.0
 */
EvPageCache       *ev_page_cache_new                    (EvDocument        *document);

/**
 * ev_page_cache_set_page_range:
 * @cache: an #EvPageCache
 * @start: the 0-based first page to cache (inclusive)
 * @end: the 0-based last page to cache (inclusive)
 *
 * Restricts the cache to pages [@start, @end].  Pages
 * outside this range are evicted from the cache.
 *
 * Since: 4.8.0
 */
void               ev_page_cache_set_page_range         (EvPageCache       *cache,
							 gint               start,
							 gint               end);

/**
 * ev_page_cache_get_flags:
 * @cache: an #EvPageCache
 *
 * Returns: the #EvJobPageDataFlags currently set
 *
 * Since: 4.8.0
 */
EvJobPageDataFlags ev_page_cache_get_flags              (EvPageCache       *cache);

/**
 * ev_page_cache_set_flags:
 * @cache: an #EvPageCache
 * @flags: the new #EvJobPageDataFlags
 *
 * Sets which page data is cached.  Pages already cached
 * keep their old data until they are marked dirty.
 *
 * Since: 4.8.0
 */
void               ev_page_cache_set_flags              (EvPageCache       *cache,
							 EvJobPageDataFlags flags);

/**
 * ev_page_cache_mark_dirty:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 * @flags: which data to mark dirty (or the special value
 *   "all" to mark everything dirty)
 *
 * Marks the @flags part of @page as dirty; the next call
 * to fetch that data will reload it from the backend.
 *
 * Since: 4.8.0
 */
void               ev_page_cache_mark_dirty             (EvPageCache       *cache,
							 gint               page,
                                                         EvJobPageDataFlags flags);

/**
 * ev_page_cache_get_link_mapping:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns: (transfer none): the #EvMappingList of links
 *   on @page, or %NULL if not yet loaded
 *
 * Since: 4.8.0
 */
EvMappingList     *ev_page_cache_get_link_mapping       (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_image_mapping:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns: (transfer none): the #EvMappingList of images
 *   on @page, or %NULL if not yet loaded
 *
 * Since: 4.8.0
 */
EvMappingList     *ev_page_cache_get_image_mapping      (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_form_field_mapping:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns: (transfer none): the #EvMappingList of form
 *   fields on @page, or %NULL if not yet loaded
 *
 * Since: 4.8.0
 */
EvMappingList     *ev_page_cache_get_form_field_mapping (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_annot_mapping:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns: (transfer none): the #EvMappingList of
 *   annotations on @page, or %NULL if not yet loaded
 *
 * Since: 4.8.0
 */
EvMappingList     *ev_page_cache_get_annot_mapping      (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_text_mapping:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns: (transfer none) (nullable): the cairo region
 *   of text glyphs on @page, or %NULL if not yet loaded
 *
 * Since: 4.8.0
 */
cairo_region_t    *ev_page_cache_get_text_mapping       (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_text:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 *
 * Returns the text of @page as a single string.  The
 * string is owned by the cache and must not be freed.
 *
 * Returns: (transfer none) (nullable): the text, or %NULL
 *
 * Since: 4.8.0
 */
const gchar       *ev_page_cache_get_text               (EvPageCache       *cache,
							 gint               page);

/**
 * ev_page_cache_get_text_layout:
 * @cache: an #EvPageCache
 * @page: the 0-based page index
 * @areas: (out) (array length=n_areas) (transfer none):
 *   return location for the per-glyph rectangles
 * @n_areas: (out): return location for the number of glyphs
 *
 * Fetches the per-glyph text layout of @page (the rectangle
 * of each glyph on the page).  Used by the find-tool to
 * highlight matches.
 *
 * Returns: %TRUE on success
 *
 * Since: 4.8.0
 */
gboolean           ev_page_cache_get_text_layout        (EvPageCache       *cache,
							 gint               page,
							 EvRectangle      **areas,
							 guint             *n_areas);

G_END_DECLS

#endif /* EV_PAGE_CACHE_H */
