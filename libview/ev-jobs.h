/* this file is part of xreader, a mate document viewer
 *
 *  Copyright (C) 2008 Carlos Garcia Campos <carlosgc@gnome.org>
 *  Copyright (C) 2005 Red Hat, Inc
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

#ifndef __EV_JOBS_H__
#define __EV_JOBS_H__

#include <gio/gio.h>
#include <gtk/gtk.h>
#include <cairo.h>

#include <xreader-document.h>

G_BEGIN_DECLS

typedef struct _EvJob EvJob;
typedef struct _EvJobClass EvJobClass;

typedef struct _EvJobRender EvJobRender;
typedef struct _EvJobRenderClass EvJobRenderClass;

typedef struct _EvJobPageData EvJobPageData;
typedef struct _EvJobPageDataClass EvJobPageDataClass;

typedef struct _EvJobThumbnail EvJobThumbnail;
typedef struct _EvJobThumbnailClass EvJobThumbnailClass;

typedef struct _EvJobLinks EvJobLinks;
typedef struct _EvJobLinksClass EvJobLinksClass;

typedef struct _EvJobAttachments EvJobAttachments;
typedef struct _EvJobAttachmentsClass EvJobAttachmentsClass;

typedef struct _EvJobAnnots EvJobAnnots;
typedef struct _EvJobAnnotsClass EvJobAnnotsClass;

typedef struct _EvJobFonts EvJobFonts;
typedef struct _EvJobFontsClass EvJobFontsClass;

typedef struct _EvJobLoad EvJobLoad;
typedef struct _EvJobLoadClass EvJobLoadClass;

typedef struct _EvJobSave EvJobSave;
typedef struct _EvJobSaveClass EvJobSaveClass;

typedef struct _EvJobFind EvJobFind;
typedef struct _EvJobFindClass EvJobFindClass;

typedef struct _EvJobLayers EvJobLayers;
typedef struct _EvJobLayersClass EvJobLayersClass;

typedef struct _EvJobExport EvJobExport;
typedef struct _EvJobExportClass EvJobExportClass;

typedef struct _EvJobPrint EvJobPrint;
typedef struct _EvJobPrintClass EvJobPrintClass;

#define EV_TYPE_JOB		     	     (ev_job_get_type())
#define EV_JOB(object)		             (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB, EvJob))
#define EV_JOB_CLASS(klass)	             (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB, EvJobClass))
#define EV_IS_JOB(object)		     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB))
#define EV_JOB_GET_CLASS(object)             (G_TYPE_INSTANCE_GET_CLASS((object), EV_TYPE_JOB, EvJobClass))

#define EV_TYPE_JOB_LINKS		     (ev_job_links_get_type())
#define EV_JOB_LINKS(object)		     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_LINKS, EvJobLinks))
#define EV_JOB_LINKS_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_LINKS, EvJobLinksClass))
#define EV_IS_JOB_LINKS(object)		     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_LINKS))

#define EV_TYPE_JOB_ATTACHMENTS		     (ev_job_attachments_get_type())
#define EV_JOB_ATTACHMENTS(object)	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_ATTACHMENTS, EvJobAttachments))
#define EV_JOB_ATTACHMENTS_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_ATTACHMENTS, EvJobAttachmentsClass))
#define EV_IS_JOB_ATTACHMENTS(object)	     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_ATTACHMENTS))

#define EV_TYPE_JOB_ANNOTS                   (ev_job_annots_get_type())
#define EV_JOB_ANNOTS(object)                (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_ANNOTS, EvJobAnnots))
#define EV_JOB_ANNOTS_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_ANNOTS, EvJobAnnotsClass))
#define EV_IS_JOB_ANNOTS(object)             (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_ANNOTS))

#define EV_TYPE_JOB_RENDER		     (ev_job_render_get_type())
#define EV_JOB_RENDER(object)		     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_RENDER, EvJobRender))
#define EV_JOB_RENDER_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_RENDER, EvJobRenderClass))
#define EV_IS_JOB_RENDER(object)	     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_RENDER))

#define EV_TYPE_JOB_PAGE_DATA		     (ev_job_page_data_get_type())
#define EV_JOB_PAGE_DATA(object)	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_PAGE_DATA, EvJobPageData))
#define EV_JOB_PAGE_DATA_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_PAGE_DATA, EvJobPageDataClass))
#define EV_IS_JOB_PAGE_DATA(object)	     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_PAGE_DATA))

#define EV_TYPE_JOB_THUMBNAIL		     (ev_job_thumbnail_get_type())
#define EV_JOB_THUMBNAIL(object)	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_THUMBNAIL, EvJobThumbnail))
#define EV_JOB_THUMBNAIL_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_THUMBNAIL, EvJobThumbnailClass))
#define EV_IS_JOB_THUMBNAIL(object)	     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_THUMBNAIL))

#define EV_TYPE_JOB_FONTS		     (ev_job_fonts_get_type())
#define EV_JOB_FONTS(object)	     	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_FONTS, EvJobFonts))
#define EV_JOB_FONTS_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_FONTS, EvJobFontsClass))
#define EV_IS_JOB_FONTS(object)		     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_FONTS))

#define EV_TYPE_JOB_LOAD		     (ev_job_load_get_type())
#define EV_JOB_LOAD(object)	     	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_LOAD, EvJobLoad))
#define EV_JOB_LOAD_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_LOAD, EvJobLoadClass))
#define EV_IS_JOB_LOAD(object)		     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_LOAD))

#define EV_TYPE_JOB_SAVE		     (ev_job_save_get_type())
#define EV_JOB_SAVE(object)	     	     (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_SAVE, EvJobSave))
#define EV_JOB_SAVE_CLASS(klass)	     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_SAVE, EvJobSaveClass))
#define EV_IS_JOB_SAVE(object)		     (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_SAVE))

#define EV_TYPE_JOB_FIND                     (ev_job_find_get_type())
#define EV_JOB_FIND(object)                  (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_FIND, EvJobFind))
#define EV_JOB_FIND_CLASS(klass)             (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_FIND, EvJobFindClass))
#define EV_IS_JOB_FIND(object)               (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_FIND))

#define EV_TYPE_JOB_LAYERS                   (ev_job_layers_get_type())
#define EV_JOB_LAYERS(object)                (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_LAYERS, EvJobLayers))
#define EV_JOB_LAYERS_CLASS(klass)           (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_LAYERS, EvJobLayersClass))
#define EV_IS_JOB_LAYERS(object)             (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_LAYERS))

#define EV_TYPE_JOB_EXPORT                    (ev_job_export_get_type())
#define EV_JOB_EXPORT(object)                 (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_EXPORT, EvJobExport))
#define EV_JOB_EXPORT_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_EXPORT, EvJobExportClass))
#define EV_IS_JOB_EXPORT(object)              (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_EXPORT))

#define EV_TYPE_JOB_PRINT                    (ev_job_print_get_type())
#define EV_JOB_PRINT(object)                 (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_JOB_PRINT, EvJobPrint))
#define EV_JOB_PRINT_CLASS(klass)            (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_JOB_PRINT, EvJobPrintClass))
#define EV_IS_JOB_PRINT(object)              (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_JOB_PRINT))

typedef enum {
	EV_JOB_RUN_THREAD,
	EV_JOB_RUN_MAIN_LOOP
} EvJobRunMode;

struct _EvJob
{
	GObject parent;
	
	EvDocument *document;

	EvJobRunMode run_mode;

	guint cancelled : 1;
	guint finished : 1;
	guint failed : 1;
	
	GError *error;
	GCancellable *cancellable;

	guint idle_finished_id;
	guint idle_cancelled_id;
};

struct _EvJobClass
{
	GObjectClass parent_class;

	gboolean (*run)         (EvJob *job);
	
	/* Signals */
	void     (* cancelled)  (EvJob *job);
	void     (* finished)   (EvJob *job);
};

struct _EvJobLinks
{
	EvJob parent;

	GtkTreeModel *model;
};

struct _EvJobLinksClass
{
	EvJobClass parent_class;
};

struct _EvJobAttachments
{
	EvJob parent;

	GList *attachments;
};

struct _EvJobAttachmentsClass
{
	EvJobClass parent_class;
};

struct _EvJobAnnots
{
	EvJob parent;

	GList *annots;
};

struct _EvJobAnnotsClass
{
	EvJobClass parent_class;
};

struct _EvJobRender
{
	EvJob parent;

	gint page;
	gint rotation;
	gdouble scale;

	gboolean page_ready;
	gint target_width;
	gint target_height;
	cairo_surface_t *surface;

	gboolean include_selection;
	cairo_surface_t *selection;
	cairo_region_t *selection_region;
	EvRectangle selection_points;
	EvSelectionStyle selection_style;
	GdkColor base;
	GdkColor text;
};

struct _EvJobRenderClass
{
	EvJobClass parent_class;
};

typedef enum {
	EV_PAGE_DATA_INCLUDE_NONE         = 0,
	EV_PAGE_DATA_INCLUDE_LINKS        = 1 << 0,
	EV_PAGE_DATA_INCLUDE_TEXT         = 1 << 1,
	EV_PAGE_DATA_INCLUDE_TEXT_MAPPING = 1 << 2,
	EV_PAGE_DATA_INCLUDE_TEXT_LAYOUT  = 1 << 3,
	EV_PAGE_DATA_INCLUDE_IMAGES       = 1 << 4,
	EV_PAGE_DATA_INCLUDE_FORMS        = 1 << 5,
	EV_PAGE_DATA_INCLUDE_ANNOTS       = 1 << 6,
	EV_PAGE_DATA_INCLUDE_ALL          = (1 << 7) - 1
} EvJobPageDataFlags;

struct _EvJobPageData
{
	EvJob parent;

	gint page;
	EvJobPageDataFlags flags;

	EvMappingList  *link_mapping;
	EvMappingList  *image_mapping;
	EvMappingList  *form_field_mapping;
	EvMappingList  *annot_mapping;
	cairo_region_t *text_mapping;
	gchar *text;
	EvRectangle *text_layout;
	guint text_layout_length;
};

struct _EvJobPageDataClass
{
	EvJobClass parent_class;
};

struct _EvJobThumbnail
{
	EvJob parent;
	gint page;
	gint rotation;
	gdouble scale;
	cairo_surface_t *surface;
	GdkPixbuf *thumbnail;
};

struct _EvJobThumbnailClass
{
	EvJobClass parent_class;
};

struct _EvJobFonts
{
	EvJob parent;
	gboolean scan_completed;
};

struct _EvJobFontsClass
{
        EvJobClass parent_class;

	/* Signals */
	void (* updated)  (EvJobFonts *job,
			   gdouble     progress);
};

struct _EvJobLoad
{
	EvJob parent;

	gchar *uri;
	gchar *password;
};

struct _EvJobLoadClass
{
	EvJobClass parent_class;
};

struct _EvJobSave
{
	EvJob parent;

	gchar *uri;
	gchar *document_uri;
};

struct _EvJobSaveClass
{
	EvJobClass parent_class;
};

struct _EvJobFind
{
	EvJob parent;

	gint start_page;
	gint current_page;
	gint n_pages;
	guint total_count;
	GList **pages;
	guint *results;
	gchar *text;
	gboolean case_sensitive;
	gboolean has_results;
};

struct _EvJobFindClass
{
	EvJobClass parent_class;

	/* Signals */
	void (* updated)  (EvJobFind *job,
			   gint       page);
};

struct _EvJobLayers
{
	EvJob parent;

	GtkTreeModel *model;
};

struct _EvJobLayersClass
{
	EvJobClass parent_class;
};

struct _EvJobExport
{
	EvJob parent;

	gint page;
	EvRenderContext *rc;
};

struct _EvJobExportClass
{
	EvJobClass parent_class;
};

struct _EvJobPrint
{
	EvJob parent;

	gint page;
	cairo_t *cr;
};

struct _EvJobPrintClass
{
	EvJobClass parent_class;
};

/* Base job class */

/**
 * EvJob:
 *
 * The #EvJob abstract base class represents an asynchronous
 * operation on an #EvDocument (e.g. "render page N", "find
 * all occurrences of 'foo'", "load document at URI").  All
 * concrete jobs in libview (EvJobRender, EvJobFind,
 * EvJobLoad, etc.) are subclasses of #EvJob.
 *
 * Jobs are scheduled by the #EvJobScheduler.  The base class
 * tracks the run state (run mode, finished, failed, progress)
 * and provides the standard error-reporting helpers
 * (ev_job_failed(), ev_job_succeeded()).
 *
 * Since: 4.8.0
 */
GType           ev_job_get_type           (void) G_GNUC_CONST;

/**
 * ev_job_run:
 * @job: an #EvJob
 *
 * Starts the job.  Returns immediately (the actual work
 * happens in a background thread); the caller is notified of
 * completion via the "finished" signal.
 *
 * Returns: %TRUE if the job started successfully
 *
 * Since: 4.8.0
 */
gboolean        ev_job_run                (EvJob          *job);

/**
 * ev_job_cancel:
 * @job: an #EvJob
 *
 * Cancels a running job.  The job's "finished" signal is
 * emitted (with the failed flag set to %TRUE).  This is
 * a no-op for jobs that have already finished.
 *
 * Since: 4.8.0
 */
void            ev_job_cancel             (EvJob          *job);

/**
 * ev_job_failed:
 * @job: an #EvJob
 * @domain: the #GError domain
 * @code: the error code
 * @format: a printf-style error message
 * @...: printf-style arguments
 *
 * Marks @job as failed with the given error.  Emits the
 * "finished" signal.  The job cannot be restarted after
 * this call.
 *
 * Since: 4.8.0
 */
void            ev_job_failed             (EvJob          *job,
					   GQuark          domain,
					   gint            code,
					   const gchar    *format,
					   ...);

/**
 * ev_job_failed_from_error:
 * @job: an #EvJob
 * @error: the #GError to wrap
 *
 * Convenience wrapper around ev_job_failed() that uses
 * @error's domain, code, and message.  The @error is
 * unreffed before returning.
 *
 * Since: 4.8.0
 */
void            ev_job_failed_from_error  (EvJob          *job,
					   GError         *error);

/**
 * ev_job_succeeded:
 * @job: an #EvJob
 *
 * Marks @job as successfully completed.  Emits the
 * "finished" signal.
 *
 * Since: 4.8.0
 */
void            ev_job_succeeded          (EvJob          *job);

/**
 * ev_job_is_finished:
 * @job: an #EvJob
 *
 * Returns whether @job has finished (either successfully
 * or with an error).
 *
 * Returns: %TRUE if the job has finished
 *
 * Since: 4.8.0
 */
gboolean        ev_job_is_finished        (EvJob          *job);

/**
 * ev_job_is_failed:
 * @job: an #EvJob
 *
 * Returns whether @job has finished with an error.
 *
 * Returns: %TRUE if the job failed
 *
 * Since: 4.8.0
 */
gboolean        ev_job_is_failed          (EvJob          *job);

/**
 * ev_job_get_run_mode:
 * @job: an #EvJob
 *
 * Returns the #EvJobRunMode of @job.
 *
 * Returns: the run mode
 *
 * Since: 4.8.0
 */
EvJobRunMode    ev_job_get_run_mode       (EvJob          *job);

/**
 * ev_job_set_run_mode:
 * @job: an #EvJob
 * @run_mode: the new run mode
 *
 * Sets the run mode.  Jobs in %EV_JOB_RUN_MODE_MAIN are
 * scheduled on the main thread (between idle handlers);
 * jobs in %EV_JOB_RUN_MODE_BACKGROUND are scheduled on a
 * background thread pool.
 *
 * Since: 4.8.0
 */
void            ev_job_set_run_mode       (EvJob          *job,
					   EvJobRunMode    run_mode);

/* EvJobLinks */

/**
 * ev_job_links_new:
 * @document: the #EvDocument to extract links from
 *
 * Creates a new job that extracts the link list of
 * @document.  Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_links_get_type     (void) G_GNUC_CONST;
EvJob          *ev_job_links_new          (EvDocument     *document);

/* EvJobAttachments */

/**
 * ev_job_attachments_new:
 * @document: the #EvDocument to extract attachments from
 *
 * Creates a new job that extracts the attachment list of
 * @document.  Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_attachments_get_type (void) G_GNUC_CONST;
EvJob          *ev_job_attachments_new      (EvDocument     *document);

/* EvJobAnnots */

/**
 * ev_job_annots_new:
 * @document: the #EvDocument to extract annotations from
 *
 * Creates a new job that extracts the annotation list of
 * @document.  Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_annots_get_type      (void) G_GNUC_CONST;
EvJob          *ev_job_annots_new           (EvDocument     *document);

/* EvJobRender */

/**
 * ev_job_render_new:
 * @document: the #EvDocument to render
 * @page: the 0-based page index
 * @rotation: the rotation in degrees (0, 90, 180, 270)
 * @scale: the render scale (1.0 = native resolution)
 * @width: the target width, in pixels
 * @height: the target height, in pixels
 *
 * Creates a new job that renders @page of @document to a
 * surface of size @width x @height.  Use ev_job_run() to
 * schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_render_get_type    (void) G_GNUC_CONST;
EvJob          *ev_job_render_new         (EvDocument      *document,
					   gint             page,
					   gint             rotation,
					   gdouble          scale,
					   gint             width,
					   gint             height);

/**
 * ev_job_render_set_selection_info:
 * @job: an #EvJobRender
 * @selection_points: (nullable): the selection rectangle, or %NULL
 * @selection_style: the selection style (single-line / block / glyph)
 * @text: the selection text color
 * @base: the selection background color
 *
 * Configures a "selection render" (i.e. a render with a
 * highlighted selection).  Pass %NULL for @selection_points
 * to render without a selection.
 *
 * Since: 4.8.0
 */
void     ev_job_render_set_selection_info (EvJobRender     *job,
					   EvRectangle     *selection_points,
					   EvSelectionStyle selection_style,
					   GdkColor        *text,
					   GdkColor        *base);
/* EvJobPageData */

/**
 * ev_job_page_data_new:
 * @document: the #EvDocument
 * @page: the 0-based page index
 * @flags: the #EvJobPageDataFlags to load
 *
 * Creates a new job that loads the page data (text, links,
 * images, forms, annotations, ...) of @page of @document.
 * Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_page_data_get_type (void) G_GNUC_CONST;
EvJob          *ev_job_page_data_new      (EvDocument      *document,
					   gint             page,
					   EvJobPageDataFlags flags);

/* EvJobThumbnail */

/**
 * ev_job_thumbnail_new:
 * @document: the #EvDocument
 * @page: the 0-based page index
 * @rotation: the rotation in degrees (0, 90, 180, 270)
 * @scale: the render scale (1.0 = native resolution)
 *
 * Creates a new job that renders a thumbnail of @page of
 * @document.  Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_thumbnail_get_type (void) G_GNUC_CONST;
EvJob          *ev_job_thumbnail_new      (EvDocument      *document,
					   gint             page,
					   gint             rotation,
					   gdouble          scale);

/* EvJobFonts */

/**
 * ev_job_fonts_new:
 * @document: the #EvDocument to scan
 *
 * Creates a new job that scans @document for embedded
 * fonts.  Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType 		ev_job_fonts_get_type 	  (void) G_GNUC_CONST;
EvJob 	       *ev_job_fonts_new 	  (EvDocument      *document);

/* EvJobLoad */

/**
 * ev_job_load_new:
 * @uri: the URI of the document to load
 *
 * Creates a new job that loads the document at @uri.  The
 * result (an #EvDocument) is fetched via the "document"
 * property of the #EvJob.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType 		ev_job_load_get_type 	  (void) G_GNUC_CONST;
EvJob 	       *ev_job_load_new 	  (const gchar 	   *uri);

/**
 * ev_job_load_set_uri:
 * @load: an #EvJobLoad
 * @uri: the new URI
 *
 * Resets the URI of @load (only valid before ev_job_run()).
 *
 * Since: 4.8.0
 */
void            ev_job_load_set_uri       (EvJobLoad       *load,
					   const gchar     *uri);

/**
 * ev_job_load_set_password:
 * @job: an #EvJobLoad
 * @password: the password (or %NULL to clear)
 *
 * Sets the password used to decrypt encrypted documents.
 * A copy of @password is made.
 *
 * Since: 4.8.0
 */
void            ev_job_load_set_password  (EvJobLoad       *job,
					   const gchar     *password);

/* EvJobSave */

/**
 * ev_job_save_new:
 * @document: the #EvDocument to save
 * @uri: the URI to save to
 * @document_uri: the original document URI (for relative
 *   link rewriting, may be %NULL)
 *
 * Creates a new job that saves @document to @uri.  Use
 * ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_save_get_type      (void) G_GNUC_CONST;
EvJob          *ev_job_save_new           (EvDocument      *document,
					   const gchar     *uri,
					   const gchar     *document_uri);
/* EvJobFind */

/**
 * ev_job_find_new:
 * @document: the #EvDocument to search
 * @start_page: the 0-based page index to start the search from
 * @n_pages: the number of pages to search (or -1 for all)
 * @text: the search text
 * @case_sensitive: %TRUE for case-sensitive, %FALSE otherwise
 *
 * Creates a new job that finds all occurrences of @text in
 * @document starting at @start_page.  Use ev_job_run() to
 * schedule it.  See ev_string_match() (PR #117) for the
 * regex / case-insensitive flags used internally.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_find_get_type      (void) G_GNUC_CONST;
EvJob          *ev_job_find_new           (EvDocument      *document,
					   gint             start_page,
					   gint             n_pages,
					   const gchar     *text,
					   gboolean         case_sensitive);

/**
 * ev_job_find_get_n_results:
 * @job: an #EvJobFind
 * @pages: the 0-based page index (or -1 for total)
 *
 * Returns the number of search results on @pages (or the
 * total number if @pages is -1).
 *
 * Returns: the result count
 *
 * Since: 4.8.0
 */
gint            ev_job_find_get_n_results (EvJobFind       *job,
					   gint             pages);

/**
 * ev_job_find_get_text:
 * @job: an #EvJobFind
 *
 * Returns the search text of @job.  The string is owned
 * by the job and must not be freed.
 *
 * Returns: (transfer none) (nullable): the search text, or %NULL
 *
 * Since: 4.8.0
 */
const gchar    *ev_job_find_get_text	  (EvJobFind       *job);

/**
 * ev_job_find_get_progress:
 * @job: an #EvJobFind
 *
 * Returns the search progress as a fraction (0.0 to 1.0).
 * Returns 1.0 if the search has finished.
 *
 * Returns: the progress fraction
 *
 * Since: 4.8.0
 */
gdouble         ev_job_find_get_progress  (EvJobFind       *job);

/**
 * ev_job_find_has_results:
 * @job: an #EvJobFind
 *
 * Returns whether @job has any search results.
 *
 * Returns: %TRUE if @job has at least one result
 *
 * Since: 4.8.0
 */
gboolean        ev_job_find_has_results   (EvJobFind       *job);

/**
 * ev_job_find_get_results:
 * @job: an #EvJobFind
 *
 * Returns a pointer to the #GList of #EvFindResult of @job.
 * The list is owned by the job and must not be freed.
 *
 * Returns: (transfer none) (element-type EvFindResult) (nullable):
 *   a pointer to the result list, or %NULL
 *
 * Since: 4.8.0
 */
GList         **ev_job_find_get_results   (EvJobFind       *job);

/* EvJobLayers */

/**
 * ev_job_layers_new:
 * @document: the #EvDocument
 *
 * Creates a new job that loads the layer list of @document.
 * Use ev_job_run() to schedule it.
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_layers_get_type    (void) G_GNUC_CONST;
EvJob          *ev_job_layers_new         (EvDocument     *document);

/* EvJobExport */

/**
 * ev_job_export_new:
 * @document: the #EvDocument to export
 *
 * Creates a new job that exports @document to PDF.  Use
 * ev_job_run() to schedule it.  The export URI is set
 * separately with ev_job_export_set_uri() (TODO, 4.12.0+).
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_export_get_type    (void) G_GNUC_CONST;
EvJob          *ev_job_export_new         (EvDocument     *document);

/**
 * ev_job_export_set_page:
 * @job: an #EvJobExport
 * @page: the 0-based page index (or -1 for all)
 *
 * Restricts the export to a single @page.  Pass -1 to
 * export all pages.
 *
 * Since: 4.8.0
 */
void            ev_job_export_set_page    (EvJobExport    *job,
					   gint            page);
/* EvJobPrint */

/**
 * ev_job_print_new:
 * @document: the #EvDocument to print
 *
 * Creates a new job that prints @document.  Use
 * ev_job_run() to schedule it.  The print cairo context
 * is set with ev_job_print_set_cairo().
 *
 * Returns: (transfer full): a new #EvJob
 *
 * Since: 4.8.0
 */
GType           ev_job_print_get_type    (void) G_GNUC_CONST;
EvJob          *ev_job_print_new         (EvDocument     *document);

/**
 * ev_job_print_set_page:
 * @job: an #EvJobPrint
 * @page: the 0-based page index (or -1 for all)
 *
 * Restricts the print to a single @page.  Pass -1 to
 * print all pages.
 *
 * Since: 4.8.0
 */
void            ev_job_print_set_page    (EvJobPrint     *job,
					  gint            page);

/**
 * ev_job_print_set_cairo:
 * @job: an #EvJobPrint
 * @cr: a cairo context
 *
 * Sets the cairo context to render to.  The cairo context
 * is owned by the caller (the job does not free it).
 *
 * Since: 4.8.0
 */
void            ev_job_print_set_cairo   (EvJobPrint     *job,
					  cairo_t        *cr);

G_END_DECLS

#endif /* __EV_JOBS_H__ */
