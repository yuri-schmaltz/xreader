# fuzz/meson.build - libFuzzer harnesses
#
# Build the document-load fuzzer with:
#
#   CC=clang meson setup build-fuzz \
#     -Db_sanitize=address,undefined,fuzzer \
#     --wipe
#   meson compile -C build-fuzz fuzz-document-load
#
# Or use the ASan/UBSan-only variant (no fuzzer, no corpus) for CI:
#
#   meson setup build -Db_sanitize=address,undefined
#   meson test -C build fuzz-document-load
#
# The default build leaves the fuzz harness out; it's only built when
# the user opts in.  This keeps the default `ninja -C build` cheap.

if get_option('fuzz').enabled()
  fuzz_deps = [
    glib,
    gio,
    libdocument_dep,
  ]

  if cc.has_multi_arguments(['-fsanitize=fuzzer-no-link'],
                            dependencies: fuzz_deps)
    fuzz_link_args = ['-fsanitize=fuzzer']
  else
    fuzz_link_args = []
    warning('libFuzzer not available; fuzz harness will not be built')
  endif

  executable(
    'fuzz-document-load',
    'fuzz-document-load.c',
    include_directories: include_dirs,
    dependencies: fuzz_deps,
    c_args: [
      '-DG_LOG_DOMAIN="fuzz-document-load"',
    ],
    link_args: fuzz_link_args,
    build_by_default: false,
  )
endif
