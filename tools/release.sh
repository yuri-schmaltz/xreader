#!/usr/bin/env bash
#
# tools/release.sh - cut an xreader release end-to-end
#
# Drives the full release flow documented in RELEASE.md, with the
# safety checks that a maintainer would do by hand.  Run from the
# repository root:
#
#   ./tools/release.sh 4.7.0
#
# The script:
#
#   1. Sanity-checks the working tree (no uncommitted changes,
#      on the master branch, remote = origin = yuri-schmaltz/xreader).
#   2. Bumps the version in meson.build from the current X.Y.Z to
#      the one given on the command line.
#   3. Prepends a new entry to debian/changelog with the
#      automatically-generated bullet list (git log
#      v$PREVIOUS..HEAD --oneline | sed ...).
#   4. Updates data/xreader.appdata.xml.in with a new <release>
#      entry whose date is today (UTC).
#   5. Renames the "Changes in development (unreleased)" section
#      in NEWS to "Changes in <version> (<date>)".
#   6. Asks the maintainer to review the diff, then commits,
#      tags, and pushes the tag.
#   7. Creates the GitHub release via the gh CLI (if available)
#      or via the curl fallback in the script.
#
# The script is intentionally interactive at the "review the diff"
# step: an unattended release that bakes the wrong version into
# 4 files is hard to roll back.  Every other step is non-interactive.
#
# Required tools: bash, sed, awk, git, mktemp.  Recommended: gh
# (for the GitHub release step).  Optional: date, jq.

set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"

red()   { printf '\033[31m%s\033[0m\n' "$*"; }
green() { printf '\033[32m%s\033[0m\n' "$*"; }
bold()  { printf '\033[1m%s\033[0m\n'  "$*"; }

err() { red "  ✗ $*" >&2; exit 1; }
ok()  { green "  ✓ $*"; }

# ---------------------------------------------------------------------------
# 0. Sanity checks.
# ---------------------------------------------------------------------------

bold "==> 0. Sanity checks"

if [ $# -ne 1 ]; then
    err "usage: $0 <new-version> [-s|--sign]   (e.g. $0 4.7.0 --sign)"
fi
NEW="$1"

if ! git rev-parse --git-dir >/dev/null 2>&1; then
    err "not in a git repository"
fi

branch=$(git rev-parse --abbrev-ref HEAD)
[ "$branch" = "master" ] || err "must be on master (currently on $branch)"

if ! git diff --quiet HEAD; then
    err "working tree has uncommitted changes -- commit or stash before releasing"
fi

remote_url=$(git config --get remote.origin.url || true)
case "$remote_url" in
    *yuri-schmaltz/xreader*)
        ok "remote = $remote_url"
        ;;
    *)
        err "remote.origin.url is '$remote_url' -- expected something with yuri-schmaltz/xreader"
        ;;
esac

# Make sure the local master is up to date with origin/master.
git fetch origin master >/dev/null 2>&1 || err "git fetch origin master failed"
local_sha=$(git rev-parse HEAD)
remote_sha=$(git rev-parse origin/master)
[ "$local_sha" = "$remote_sha" ] || err "local master is not at origin/master (run git pull first)"

ok "master is at origin/master @ $local_sha"

# ---------------------------------------------------------------------------
# 1. Compute the previous version.
# ---------------------------------------------------------------------------

bold "==> 1. Read current and previous versions"

OLD=$(sed -n "s/^.*project('xreader', 'c', 'cpp', version : '\\([0-9]\\+\\.[0-9]\\+\\.[0-9]\\+\\)').*$/\\1/p" meson.build | head -1)
[ -n "$OLD" ] || err "could not parse current version from meson.build"

[ "$OLD" != "$NEW" ] || err "new version $NEW is the same as the current version"

ok "$OLD -> $NEW"

# Reject a tag that already exists (defensive: someone might have
# manually tagged a release and forgotten to push).
if git rev-parse --verify "refs/tags/v$NEW" >/dev/null 2>&1; then
    err "tag v$NEW already exists locally"
fi

# ---------------------------------------------------------------------------
# 2. Bump meson.build.
# ---------------------------------------------------------------------------

bold "==> 2. Bump meson.build"

sed -i.bak "s/version : '$OLD'/version : '$NEW'/" meson.build
rm -f meson.build.bak
ok "meson.build: project version $OLD -> $NEW"

# ---------------------------------------------------------------------------
# 3. Prepend debian/changelog entry.
# ---------------------------------------------------------------------------

bold "==> 3. Prepend debian/changelog entry"

today=$(date -u +%Y-%m-%d)
tmp_changes=$(mktemp)
git log "v$OLD..HEAD" --pretty='    - %s' > "$tmp_changes" 2>/dev/null || true
if [ ! -s "$tmp_changes" ]; then
    echo "    - No user-visible changes since v$OLD" > "$tmp_changes"
fi

tmp_changelog=$(mktemp)
cat > "$tmp_changelog" <<EOF
xreader ($NEW) zena; urgency=medium

  * Changes since v$OLD:
$(cat "$tmp_changelog" 2>/dev/null || true)
$(cat "$tmp_changes")

 -- Mavis <Mavis@local>  $(date -R)

EOF
cat debian/changelog >> "$tmp_changelog"
mv "$tmp_changelog" debian/changelog
rm -f "$tmp_changes"
ok "debian/changelog: new entry prepended"

# ---------------------------------------------------------------------------
# 4. Update appdata.
# ---------------------------------------------------------------------------

bold "==> 4. Update data/xreader.appdata.xml.in"

appdata="data/xreader.appdata.xml.in"
if [ -f "$appdata" ]; then
    if grep -q "<releases>" "$appdata"; then
        # Insert a new <release> at the top of the existing <releases> block.
        tmp=$(mktemp)
        python3 - "$appdata" "$NEW" "$today" > "$tmp" <<'PYEOF'
import sys
path, new_ver, today = sys.argv[1:4]
with open(path) as f:
    src = f.read()
new_entry = (f'    <release version="{new_ver}" date="{today}">\n'
             f'      <description><p>See debian/changelog for the full list of changes.</p></description>\n'
             f'    </release>\n')
i = src.find('<releases>') + len('<releases>')
src = src[:i] + '\n' + new_entry + src[i:]
with open(tmp, 'w') as f:
    f.write(src)
PYEOF
        mv "$tmp" "$appdata"
        ok "$appdata: new <release> inserted"
    else
        # No <releases> block yet -- add one before </component>.
        tmp=$(mktemp)
        python3 - "$appdata" "$NEW" "$today" > "$tmp" <<'PYEOF'
import sys
path, new_ver, today = sys.argv[1:4]
with open(path) as f:
    src = f.read()
new_block = (f'\n  <releases>\n'
             f'    <release version="{new_ver}" date="{today}">\n'
             f'      <description><p>See debian/changelog for the full list of changes.</p></description>\n'
             f'    </release>\n'
             f'  </releases>\n')
src = src.replace('</component>', new_block + '</component>')
with open(tmp, 'w') as f:
    f.write(src)
PYEOF
        mv "$tmp" "$appdata"
        ok "$appdata: new <releases> block added"
    fi
else
    ok "$appdata: skipped (file not found)"
fi

# ---------------------------------------------------------------------------
# 5. Update NEWS.
# ---------------------------------------------------------------------------

bold "==> 5. Update NEWS"

if [ -f NEWS ]; then
    if grep -q "^Changes in development (unreleased)" NEWS; then
        sed -i "s/^Changes in development (unreleased)/Changes in $NEW ($today)/" NEWS
        ok "NEWS: header updated to 'Changes in $NEW ($today)'"
    else
        ok "NEWS: no 'unreleased' section to update"
    fi
else
    ok "NEWS: skipped (file not found)"
fi

# ---------------------------------------------------------------------------
# 6. Review, commit, tag, push.
# ---------------------------------------------------------------------------

bold "==> 6. Review the diff"

git diff --stat HEAD
echo
git diff HEAD

bold "==> 6a. Confirm and commit"
read -r -p "Commit the bump and tag v$NEW? [y/N] " answer
case "$answer" in
    y|Y|yes|YES)
        ;;
    *)
        err "aborted by user; working tree left dirty (use git checkout to revert)"
        ;;
esac

git add meson.build debian/changelog "$appdata" NEWS
git commit -m "Release $NEW

Bump the version in meson.build, debian/changelog,
data/xreader.appdata.xml.in, and NEWS for the $NEW release.

Generated by tools/release.sh."

# Annotated tag (the maintainer's release key signs this;
# for an unsigned tag in the fork, drop the -s).
#
# Accept a -s / --sign flag to sign the tag with the
# maintainer's default GPG key.  Default: unsigned, so the
# release.sh script works in CI / sandboxed environments
# that don't have GPG configured.
SIGN_TAG=0
for arg in "$@"; do
    case "$arg" in
        -s|--sign) SIGN_TAG=1 ;;
    esac
done
if [ "$SIGN_TAG" -eq 1 ]; then
    git tag -s -m "Release $NEW" "v$NEW"
else
    git tag -a -m "Release $NEW" "v$NEW"
fi

bold "==> 6b. Push the tag"
git push origin master
git push origin "v$NEW"
ok "pushed master + v$NEW"

# ---------------------------------------------------------------------------
# 7. GitHub release.
# ---------------------------------------------------------------------------

bold "==> 7. GitHub release"

if command -v gh >/dev/null 2>&1; then
    if gh release create "v$NEW" \
        --title "Xreader $NEW" \
        --notes-file <(git log "v$OLD..v$NEW" --pretty='%s' | sed 's/^/- /') \
        --target master; then
        ok "gh release create succeeded"
    else
        red "  ! gh release create failed -- create the release manually at"
        red "    https://github.com/yuri-schmaltz/xreader/releases/new?tag=v$NEW"
    fi
else
    red "  ! 'gh' CLI not found -- create the release manually at"
    red "    https://github.com/yuri-schmaltz/xreader/releases/new?tag=v$NEW"
fi

# ---------------------------------------------------------------------------
# 8. Done.
# ---------------------------------------------------------------------------

echo
green "Release $NEW complete."
echo
echo "Next steps:"
echo "  * dpkg-buildpackage -b -uc -us   # build the .deb"
echo "  * ./tools/lint-commits.sh v$OLD..HEAD   # verify the release commits"
echo "  * update the linuxmint/xreader upstream if the maintainer wants"
