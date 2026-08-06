/* smoke-headers.c - Smoke-test that every public xreader header
 *                    compiles cleanly when included from a single
 *                    translation unit.
 *
 * Copyright (C) 2026 Yuri Schmaltz / xreader fork
 *
 * This is the C equivalent of the `python3 -c "import rayforge"`
 * smoke test that pires-forge added in PR #91.  xreader is a
 * C / GObject project, so the analogue is: compile a single
 * translation unit that #includes every public header, with
 * -Wall -Werror.  If any header has a syntax error, missing
 * forward declaration, or missing export, the compile fails
 * and CI catches it.
 *
 * The test is run from the meson build (or from a CI job that
 * doesn't need meson: it only needs the right -I flags + the
 * right pkg-config flags).  See .github/workflows/ci-checks.yml
 * for the CI integration (PR #115, ci/smoke-header-compile job).
 *
 * Why not just rely on the warnings job?  Because the warnings
 * job builds the *implementation* files, not a single TU that
 * includes every header.  A header that is no longer #included
 * anywhere (because the implementation moved but the header
 * wasn't updated) will not be caught by the warnings job but
 * will be caught by this smoke test.
 *
 * Keep the #include list in sync with the public headers
 * listed in libdocument/meson.build, libview/meson.build, and
 * shell/meson.build.  The lint-commits.sh pre-commit check
 * does not verify this -- if you add a public header, update
 * this file too.  A future PR will automate that with a
 * script.
 *
 * Run via:
 *   meson test -C build smoke-headers
 *
 * (the meson.build test harness wires up the right -I flags
 * and pkg-config dependencies; see tests/meson.build).
 */

#include <config.h>

/* All public headers, in three groups: document, view, shell. */

/* --- libdocument (the EvDocument abstraction layer) --- */

#include "libdocument/ev-annotation.h"
#include "libdocument/ev-async-renderer.h"
#include "libdocument/ev-attachment.h"
#include "libdocument/ev-backends-manager.h"
#include "libdocument/ev-debug.h"
#include "libdocument/ev-document-annotations.h"
#include "libdocument/ev-document-attachments.h"
#include "libdocument/ev-document-factory.h"
#include "libdocument/ev-document-find.h"
#include "libdocument/ev-document-fonts.h"
#include "libdocument/ev-document-forms.h"
#include "libdocument/ev-document-images.h"
#include "libdocument/ev-document-info.h"
#include "libdocument/ev-document-layers.h"
#include "libdocument/ev-document-links.h"
#include "libdocument/ev-document-misc.h"
#include "libdocument/ev-document-print.h"
#include "libdocument/ev-document-security.h"
#include "libdocument/ev-document-text.h"
#include "libdocument/ev-document-thumbnails.h"
#include "libdocument/ev-document-transition.h"
#include "libdocument/ev-document.h"
#include "libdocument/ev-file-exporter.h"
#include "libdocument/ev-file-helpers.h"
#include "libdocument/ev-form-field.h"
#include "libdocument/ev-image.h"
#include "libdocument/ev-init.h"
#include "libdocument/ev-layer.h"
#include "libdocument/ev-link-action.h"
#include "libdocument/ev-link-dest.h"
#include "libdocument/ev-link.h"
#include "libdocument/ev-macros.h"
#include "libdocument/ev-mapping-list.h"
#include "libdocument/ev-module.h"
#include "libdocument/ev-page.h"
#include "libdocument/ev-render-context.h"
#include "libdocument/ev-selection.h"
#include "libdocument/ev-string-match.h"
#include "libdocument/ev-transition-effect.h"

/* --- libview (the rendering layer) --- */

#include "libview/ev-annotation-window.h"
#include "libview/ev-document-model.h"
#include "libview/ev-job-scheduler.h"
#include "libview/ev-jobs.h"
#include "libview/ev-link-accessible.h"
#include "libview/ev-loading-window.h"
#include "libview/ev-page-cache.h"
#include "libview/ev-pixbuf-cache.h"
#include "libview/ev-print-operation.h"
#include "libview/ev-stock-icons.h"
#include "libview/ev-timeline.h"
#include "libview/ev-transition-animation.h"
#include "libview/ev-view-accessible.h"
#include "libview/ev-view-cursor.h"
#include "libview/ev-view-presentation.h"
#include "libview/ev-view-private.h"
#include "libview/ev-view.h"
#include "libview/ev-web-view.h"

/* --- shell (the GTK application) --- */

#include "shell/eggfindbar.h"
#include "shell/ev-annotation-properties-dialog.h"
#include "shell/ev-annotations-toolbar.h"
#include "shell/ev-application.h"
#include "shell/ev-bookmark-action.h"
#include "shell/ev-bookmarks.h"
#include "shell/ev-file-monitor.h"
#include "shell/ev-history-action-widget.h"
#include "shell/ev-history-action.h"
#include "shell/ev-history.h"
#include "shell/ev-keyring.h"
#include "shell/ev-message-area.h"
#include "shell/ev-metadata.h"
#include "shell/ev-open-recent-action.h"
#include "shell/ev-password-view.h"
#include "shell/ev-preferences-dialog.h"
#include "shell/ev-progress-message-area.h"
#include "shell/ev-properties-dialog.h"
#include "shell/ev-properties-fonts.h"
#include "shell/ev-properties-license.h"
#include "shell/ev-properties-view.h"
#include "shell/ev-sidebar-annotations.h"
#include "shell/ev-sidebar-attachments.h"
#include "shell/ev-sidebar-bookmarks.h"
#include "shell/ev-sidebar-layers.h"
#include "shell/ev-sidebar-links.h"
#include "shell/ev-sidebar-page.h"
#include "shell/ev-sidebar-thumbnails.h"
#include "shell/ev-sidebar.h"
#include "shell/ev-tab-manager.h"
#include "shell/ev-tab.h"
#include "shell/ev-tabbed-window.h"
#include "shell/ev-toolbar.h"
#include "shell/ev-utils.h"
#include "shell/ev-window-title.h"
#include "shell/ev-window.h"
#include "shell/ev-zoom-action.h"

int main (int argc, char *argv[]) {
    (void) argc;
    (void) argv;
    return 0;
}
