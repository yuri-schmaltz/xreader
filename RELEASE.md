# Release process for Xreader

This document is the step-by-step guide for cutting a new
Xreader release.  It is written for the
[yuri-schmaltz/xreader](https://github.com/yuri-schmaltz/xreader)
fork but the steps apply equally to the upstream
[linuxmint/xreader](https://github.com/linuxmint/xreader)
repository -- the only difference is the `origin` remote.

The release cadence is the Linux Mint release cadence: a
new minor (4.x.0) ships with each new Linux Mint release,
plus a stream of micro releases (4.x.y) for security and
bug fixes in between.  There is no fixed schedule for
micro releases; they are cut on demand.

## Versioning

Xreader follows [Semantic Versioning 2.0.0](https://semver.org/):

* **MAJOR** (rare; reserved for the GTK 4 port, see
  `C2` in the work plan) -- incompatible API / behaviour
  change.
* **MINOR** (one per Linux Mint release) -- new features,
  new backends, deprecations.
* **MICRO** (security / bug fixes) -- no new features, only
  fixes that the distros can pick up as point releases.

The current version lives in three places that must stay
in sync:

| File | Field |
| ---- | ----- |
| `meson.build` | `project(..., version : 'X.Y.Z')` |
| `debian/changelog` | top entry header `xreader (X.Y.Z) ...` |
| `data/xreader.appdata.xml` | `<releases><release version="X.Y.Z" .../></releases>` |

The `tools/bump-version.sh` script (not yet written;
see `tools/` follow-ups) would automate the three-way
update.  Until that lands, do it by hand.

## Pre-release checklist

Two weeks before the release, walk through:

* [ ] `git fetch origin && git status` -- working tree
  clean, on `master`.
* [ ] `git fetch upstream && git log upstream/master..HEAD` --
  the fork's `master` is ahead of (or in sync with)
  upstream.  If `linuxmint/xreader` has accepted some of
  the fork's self-PRs, the `master` is ahead; the merge
  conflict window is the inverse: any commit that
  `linuxmint/xreader` has accepted must be re-applied to
  the fork's `master` (usually a no-op since the same
  commit is in both trees).
* [ ] `meson setup build && meson compile -C build && meson
  test -C build` -- clean build, all unit tests pass.
* [ ] `meson setup build-san -Db_sanitize=address,undefined
  && meson test -C build-san` -- ASan + UBSan build is
  clean.
* [ ] `cd build-fuzz && timeout 60s ./fuzz-document-load
  ../fuzz-corpus/ -max_total_time=60` -- fuzz harness
  does not crash on the seed corpus in 60 seconds.
* [ ] `cat NEWS` -- the section for the new version is
  complete, with one bullet per user-visible change and
  explicit `[SECURITY]` markers.
* [ ] `git shortlog -sn --use-mailmap` -- the top
  contributors for the cycle are listed correctly (the
  `.mailmap` from PR #20 normalises the canonical names).

## Cutting a release

### 1. Bump the version

```
# Pick the new version.  Examples:
NEW=4.6.6    # micro
NEW=4.7.0    # minor
NEW=4.7.1    # post-minor micro
OLD=$(sed -n "s/^.*'\([0-9]\+\.[0-9]\+\.[0-9]\+\)'.*$/\1/p" meson.build | head -1)
sed -i "s/'$OLD'/$NEW/" meson.build
git diff meson.build
```

### 2. Update the changelog

```
# Use dch to create a new entry:
dch --newversion "$NEW" --no-auto-nocommit \
    "Release $NEW"

# Or, if you prefer to edit by hand, prepend an entry to
# debian/changelog with the same format as the existing
# entries (look at the last entry for the exact wording).
```

### 3. Update `data/xreader.appdata.xml`

Find the `<releases>` block and add a new `<release
version="X.Y.Z" date="YYYY-MM-DD" />` entry.  The date is
the date you are cutting the release (use `date -I`).

### 4. Commit, tag, push

```
git add meson.build debian/changelog data/xreader.appdata.xml
git commit -m "Release $NEW"
git tag -s -m "Release $NEW" "v$NEW"
git push origin master
git push origin "v$NEW"
```

The tag is signed (`-s`) -- the project uses the
Linux Mint release key.  If you are cutting a release
from a contributor's workstation, drop the `-s` and
push an unsigned tag, then ask the maintainer to
re-tag.

### 5. Build the deb

```
sudo apt build-deps .   # or: sudo mk-build-deps -ir
dpkg-buildpackage -b -uc -us
# Produces ../xreader_<NEW>_<arch>.deb and
# ../xreader-common_<NEW>_<arch>.deb
```

### 6. Smoke-test the deb

```
sudo dpkg -i ../xreader*.deb
xreader --version
xreader /usr/share/doc/xreader-common/copyright  # a real PDF
# Open a DjVu / DVI / XPS / EPUB / TIFF / CBZ file as well
# to make sure the migration to system libzip (PR #12)
# and the other backend fixes are working.
```

### 7. Draft the release notes

Open a GitHub release at
https://github.com/yuri-schmaltz/xreader/releases/new,
pick the `v$NEW` tag, and paste the `Changes in $NEW`
section from `NEWS` as the body.  The first line of the
body becomes the release summary on the GitHub UI; the
remaining lines become the long-form notes.

### 8. Update the README and INSTALL pointers (optional)

If the release changed the dependency list (PR #12 added
`libzip-dev`, for example), update `INSTALL.md` and
`README.md` to match.  The same goes for any new
configure-time option in `meson_options.txt`.

## After the release

* Mark the corresponding `Changes in development`
  section in `NEWS` as `Changes in X.Y.Z` (the version
  you just cut) and add a new empty
  `Changes in development (unreleased)` section below it.
* If the release fixed any security issues, write a
  short advisory in `debian/changelog` and forward the
  CVE number to `security@linuxmint.com` for inclusion
  in the next Linux Mint security update.
* File any follow-up bugs that the release exposed at
  https://github.com/yuri-schmaltz/xreader/issues/new.

## Post-mortem

For micro releases, no post-mortem is required.  For
minor releases, write a short post-mortem covering:

* How many self-PRs were merged (count `git log
  v$PREVIOUS..v$NEW --oneline | wc -l`).
* How many of those were security fixes (`grep -c
  SECURITY debian/changelog` since the last release).
* How many CI runs failed and had to be re-run.
* Anything that surprised you about the release process.

A post-mortem is the only place where we document
process issues; the rest of the repo is for code.
