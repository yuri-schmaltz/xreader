# Hacking on Xreader

This is the developer guide for Xreader.  It covers:

* Setting up a build environment from a fresh checkout
* Running the test suite, including the new GLib Testing unit tests
  and the libFuzzer harness
* Submitting changes back via pull request

If you only want to install and run Xreader, see
[`INSTALL.md`](INSTALL.md) instead.  End-user documentation lives
in the [help/](help/) directory.

## Build prerequisites

### Debian / Ubuntu / Linux Mint

```
sudo apt install \
    git dpkg-dev meson \
    gobject-introspection \
    libdjvulibre-dev libgxps-dev libkpathsea-dev \
    libpoppler-glib-dev libsecret-1-dev libspectre-dev \
    libtiff-dev libwebkit2gtk-4.1-dev libxapp-dev \
    mate-common yelp-tools xsltproc

# optional, for the unit tests:
sudo apt install libgtest-dev

# optional, for the libFuzzer harness:
sudo apt install clang llvm
```

### Fedora / RHEL

```
sudo dnf install \
    meson git \
    djvulibre-devel gxps-devel kpathsea-devel \
    poppler-glib-devel libsecret-devel libspectre-devel \
    libtiff-devel webkit2gtk4.1-devel libxapp-devel \
    mate-common yelp-tools libxslt

# optional, for the unit tests:
sudo dnf install gtest-devel

# optional, for the libFuzzer harness:
sudo dnf install clang llvm
```

## Building

The standard build uses Meson:

```
meson setup builddir
meson compile -C builddir
meson test -C builddir
sudo meson install -C builddir
```

The first `meson setup` will print a summary of which optional
backends were found.  `--reconfigure` re-runs the configuration
step (e.g. after installing a new dev package), and
`meson configure builddir` lets you change individual options
without re-running the whole setup:

```
# Turn off the DVI backend:
meson configure builddir -Ddvi=false

# Turn on debug builds (-O0, assertions, debug symbols):
meson configure builddir --buildtype=debug
```

### Build types

* `release` (default) -- `-O2 -DNDEBUG`, no debug symbols.
* `debug` -- `-O0 -g3`, assertions enabled.
* `debugoptimized` -- `-O1 -g3`, the usual CI choice.
* `plain` -- no optimization flag, no debug info.

## Running the test suite

The test suite is driven by `meson test`:

```
meson test -C builddir
```

This runs three families of checks:

1. **Unit tests** (`tests/test-*.c`) -- pure GLib Testing unit
   tests for the helpers in `libdocument/`.  These are written
   in the GLib Testing style (`g_test_add_func`,
   `g_assert_cmpint`, etc.) and only need glib + the public
   libdocument headers, so they build and run in a few seconds
   even on a slow laptop.

   Run a specific one with:

   ```
   meson test -C builddir test-file-helpers
   ```

2. **Sanitizer build** (`-Db_sanitize=address,undefined`) -- the
   `ci-checks` workflow on GitHub Actions rebuilds with ASan +
   UBSan and runs the same test executables under sanitizers.
   The sanitizer build is also useful locally to chase
   memory-safety bugs that don't reproduce in a normal build.

3. **Fuzzer harness** (`-Dfuzz=true`, requires clang) -- the
   `fuzz/fuzz-document-load.c` libFuzzer harness drives the
   xreader document factory with random bytes.  See
   [`fuzz/`](fuzz/) for build instructions.

## Coding style

Xreader follows the Linux Mint developer guide (the
[Evince 3.x](https://gitlab.gnome.org/GNOME/evince/-/tree/3.36)
heritage); tabs for indentation in C, GNU-style control flow,
pointers on the type (`gchar *foo`), 80-column soft limit.

The repository carries both:

* **`.editorconfig`** -- the canonical source for tabs/spaces,
  charset, line endings, trailing-whitespace rules.  Every
  modern editor picks this up automatically.
* **`.clang-format`** -- the canonical source for brace style,
  pointer alignment, argument wrapping, etc.  Use
  `clang-format -i file.c` (or `make format` if you have it
  wired up) to apply the rules to a single file; `git diff`
  should then be small.

If you make a large-scale formatting change (e.g. a mass
rename), add the commit SHA to `.git-blame-ignore-revs` so the
next contributor's `git blame` doesn't have to scroll past
your diff to find the meaningful change.

## Pull requests

1. Fork [yuri-schmaltz/xreader](https://github.com/yuri-schmaltz/xreader)
   and create a topic branch off `master`:
   `git checkout -b fix/something`.
2. Make the change.  Run `meson test -C builddir` to make
   sure the test suite is still green.
3. Add an entry to `NEWS` if the change is user-visible.
4. Push the branch: `git push origin fix/something`.
5. Open a pull request against `master`.  The PR description
   should include a "Test plan" section listing the manual
   steps the reviewer should take to verify the fix.

For security reports, see [`SECURITY.md`](SECURITY.md) -- do
**not** open a public GitHub issue.

## Release process

The release is driven by `makepot/`, `data/xreader.appdata.xml`,
and a few `git tag` steps; details live in `RELEASE.md` (TODO).

## Code style

The project's C code follows the gnome coding style with a few
project-specific tweaks.  The most useful rules of thumb:

* **Commit messages** must follow `tools/lint-commits.sh` (run by
  CI on every PR).  The subject line must be `<component>: ...`
  or `chore: ...` / `fix: ...` / `feat: ...` / etc., 72 chars
  max, imperative mood.  The body must be wrapped at 80 cols
  with no trailing whitespace and no tab characters.
* **C formatting** is enforced by `tools/check-source.sh` (run
  by CI).  Use 8-space tabs for indentation, K&R brace style
  (opening brace on the same line as the if/while/for).
  Run `clang-format` (config in `.clang-format`) before
  submitting a non-trivial change.
* **Public API annotations**: every public function in
  `libdocument/*.h` should have:
  - `G_GNUC_NONNULL(N, ...)` for parameters that must be non-NULL
  - `G_GNUC_PURE` for read-only functions (no side effects)
  - `G_GNUC_MALLOC` for functions that return a newly-allocated
    pointer
  - `G_GNUC_WARN_UNUSED_RESULT` for gboolean returns that
    report a status the caller must check
  - `G_GNUC_PRINTF(N, M)` for printf-like functions
  - A `/** ... */` gtk-doc block above the function (the
    blocks feed the GIR scanner + language bindings)
* **Memory**: use `g_new` / `g_new0` (not `g_slice_*`, deprecated
  in glib 2.68), `g_free` (not `g_slice_free`), `g_get_real_time`
  (not `g_get_current_time` + `GTimeVal`, deprecated in glib
  2.62).  Use `g_autoptr` + `g_autofree` (glib 2.44+) wherever
  possible to avoid manual cleanup-on-error paths.
* **String comparison**: use `g_ascii_strcasecmp` (not
  `strcasecmp`, which is locale-dependent).

## Adding a new unit test

The `tests/` directory has 7 executables (`test-file-helpers`,
`test-mime`, `test-document-factory`, `test-image`, `test-page-render`,
`test-form-field`, plus the new one you're about to add).  Each
one is a small GLib Testing program linked against the libdocument
source files it tests.

To add a new test executable `test-FOO`:

1. Add `tests/test-FOO.c` with a `main()` that calls `g_test_init`
   + `g_test_add_func` for each test case.
2. Edit `tests/meson.build` to add an `executable()` entry that
   compiles `test-FOO.c` against the libdocument sources it needs.
3. Add a `test()` entry that runs the executable via
   `protocol: 'gtest'` (modern, supports `g_assert_*`).
4. Add a `test-FOO-cases: N` line to the PR description.

The new test will be picked up by the existing `meson test -C build`
invocation in the CI's `warnings` + `sanitizers` + `valgrind` jobs
(no CI change required).

## Adding a new fuzzer

The `fuzz/` directory has 2 harnesses (`fuzz-document-load` for
the single-extension content-based dispatch, `fuzz-document-load-all`
for the 14-extension forced-dispatch).  To add a new one:

1. Add `fuzz/fuzz-FOO.c` with an `LLVMFuzzerTestOneInput` function
   that takes a byte buffer, converts it to the input the code
   under test expects, calls the code, and cleans up.
2. Edit `fuzz/meson.build` to add an `executable()` entry.
3. Add a seed file under `fuzz/corpus/` (a few bytes is enough;
   libFuzzer mutates from there).
4. Update `fuzz/corpus/README.md` with a one-line description
   of the new seed.

The CI `fuzz` job (PR #19) runs both harnesses for 60s on
every PR; the new harness is picked up automatically once added
to `fuzz/meson.build`.

## CI

The 12 jobs in `.github/workflows/ci-checks.yml`:

  - `warnings` -- strict-warnings build
  - `sanitizers` -- ASan + UBSan (matrix: ubuntu 22.04 + 24.04)
  - `fuzz` -- 60s libFuzzer smoke run
  - `commit-lint` -- `tools/lint-commits.sh`
  - `cppcheck` -- static analysis
  - `source-style` -- `tools/check-source.sh`
  - `coverage` -- `lcov` HTML report
  - `clang-tidy` -- bugprone + cert + clang-analyzer
  - `iwyu` -- include-what-you-use
  - `valgrind` -- Memcheck on unit tests
  - `coverity` -- commercial Coverity scan (only if
    COVERITY_SCAN_TOKEN secret is set)
  - `clang` -- separate build with clang + ASan + UBSan

All run on every PR.  Some (coverity, valgrind) take longer and
are best-effort.  The maintainer can disable individual jobs by
commenting them out in `.github/workflows/ci-checks.yml`.
