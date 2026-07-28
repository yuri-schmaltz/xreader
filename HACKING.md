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
