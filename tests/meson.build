# tests/meson.build - GLib Testing unit tests for pure helpers in libdocument.
#
# Run via `meson test -C build` (or `meson test -C build test-file-helpers`).
#
# Each test executable is built from a small selection of C source files
# from libdocument that don't pull in a heavy backend (no poppler, no
# webkit, no gtk).  This keeps the test harness fast and lets the CI
# sanitizer job cover the same code paths as the runtime.

test_file_helpers = executable(
    'test-file-helpers',
    'test-file-helpers.c',
    '../libdocument/ev-file-helpers.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
        zlib,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-file-helpers"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-file-helpers',
    test_file_helpers,
    protocol: 'gtest',
)

test_mime = executable(
    'test-mime',
    'test-mime.c',
    '../libdocument/ev-file-helpers.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
        zlib,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-mime"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-mime',
    test_mime,
    protocol: 'gtest',
)

test_document_factory = executable(
    'test-document-factory',
    'test-document-factory.c',
    '../libdocument/ev-file-helpers.c',
    '../libdocument/ev-init.c',
    '../libdocument/ev-debug.c',
    '../libdocument/ev-backends-manager.c',
    '../libdocument/ev-module.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
        gtk,
        zlib,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-document-factory"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-document-factory',
    test_document_factory,
    protocol: 'gtest',
)

test_image = executable(
    'test-image',
    'test-image.c',
    '../libdocument/ev-image.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
        gdk_pixbuf,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-image"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-image',
    test_image,
    protocol: 'gtest',
)

test_page_render = executable(
    'test-page-render',
    'test-page-render.c',
    '../libdocument/ev-page.c',
    '../libdocument/ev-render-context.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-page-render"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-page-render',
    test_page_render,
    protocol: 'gtest',
)

test_form_field = executable(
    'test-form-field',
    'test-form-field.c',
    '../libdocument/ev-form-field.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gio,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-form-field"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-form-field',
    test_form_field,
    protocol: 'gtest',
)

test_rect_intersect = executable(
    'test-rect-intersect',
    'test-rect-intersect.c',
    '../libdocument/ev-document.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gobject,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-rect-intersect"',
        '-DEV_COMPILATION=1',
    ],
)

test(
    'test-rect-intersect',
    test_rect_intersect,
    protocol: 'gtest',
)

test_tab_manager = executable(
    'test-tab-manager',
    'test-tab-manager.c',
    '../shell/ev-tab.c',
    '../shell/ev-tab-manager.c',
    include_directories: include_dirs,
    dependencies: [
        glib,
        gobject,
        gio,
        gtk,
    ],
    c_args: [
        '-DG_LOG_DOMAIN="test-tab-manager"',
        '-DXREADER_COMPILATION=1',
    ],
)

test(
    'test-tab-manager',
    test_tab_manager,
    protocol: 'gtest',
)
