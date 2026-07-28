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
