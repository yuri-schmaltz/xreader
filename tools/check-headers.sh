#!/usr/bin/env bash
#
# tools/check-headers.sh - verify that tests/smoke-headers.c
#                          stays in sync with the public
#                          header list in {libdocument,
#                          libview, shell}/meson.build
#
# Run from the repository root:
#   ./tools/check-headers.sh
#
# Exit codes:
#   0  -- in sync
#   1  -- drift detected
#
# Cost: <1s, pure bash + grep.

set -u  # NOTE: do NOT use 'set -e' -- the inner pipelines
        # can return 1 from grep "no match" without it being
        # an error.  We check return codes explicitly where
        # they matter.

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"

red()   { printf '\033[31m%s\033[0m\n' "$*"; }
green() { printf '\033[32m%s\033[0m\n' "$*"; }
bold()  { printf '\033[1m%s\033[0m\n'  "$*"; }

err_count=0

# Header directories (add new ones here).
HEADER_DIRS="libdocument libview shell"
SMOKE_HEADERS_FILE="tests/smoke-headers.c"

# ---------------------------------------------------------------------------
# Build the canonical header list (one header per line).
# ---------------------------------------------------------------------------
canonical="$(mktemp)"
trap 'rm -f "$canonical" "$smoke"' EXIT

for dir in $HEADER_DIRS; do
    [ -d "$dir" ] || { red "ERROR: directory $dir does not exist"; err_count=$((err_count + 1)); continue; }

    for meson in "$dir"/*.build "$dir"/meson.build; do
        [ -f "$meson" ] || continue

        # Get generated basenames (meson configure_file input: 'foo.h.in' -> foo.h).
        # These are skipped because smoke-headers.c is for source-tree
        # headers only.
        generated_basenames="$(grep -B 1 -A 1 "input: '" "$meson" 2>/dev/null \
            | grep -oE "'\S+\.h\.in'" \
            | tr -d "'" \
            | sed 's/\.h\.in$/.h/' || true)"

        # Extract quoted .h tokens (both single and double quoted).
        # Match: 'foo.h' or "foo.h", with the quotes removed.
        grep -hoE "['\"][[:alnum:]./_-]+\.h['\"]" "$meson" 2>/dev/null \
            | tr -d "'\"" \
            | sed "s|^|$dir/|" \
            | sort -u \
            > "$canonical.tmp" || true

        # Filter out generated headers.
        if [ -n "$generated_basenames" ]; then
            while IFS= read -r h; do
                [ -z "$h" ] && continue
                base="${h##*/}"
                if ! echo "$generated_basenames" | grep -qxF "$base"; then
                    echo "$h"
                fi
            done < "$canonical.tmp" >> "$canonical"
        else
            cat "$canonical.tmp" >> "$canonical"
        fi
        rm -f "$canonical.tmp"
    done
done

sort -u "$canonical" -o "$canonical"
canonical_count="$(wc -l < "$canonical")"

# ---------------------------------------------------------------------------
# Build the smoke-headers.c include list.
# ---------------------------------------------------------------------------
smoke="$(mktemp)"
trap 'rm -f "$canonical" "$smoke"' EXIT

grep -oE '#include "[^"]+\.h"' "$SMOKE_HEADERS_FILE" 2>/dev/null \
    | sed -E 's|#include "([^"]+)"|\1|' \
    | sort -u \
    > "$smoke" || true
smoke_count="$(wc -l < "$smoke")"

# ---------------------------------------------------------------------------
# Compare.
# ---------------------------------------------------------------------------

bold "==> $canonical_count canonical headers vs $smoke_count in $SMOKE_HEADERS_FILE"

# Headers in meson.build but not in smoke-headers.c.
missing="$(comm -23 "$canonical" "$smoke" || true)"
if [ -n "$missing" ]; then
    red ""
    red "ERROR: in meson.build but missing from $SMOKE_HEADERS_FILE:"
    printf '    %s\n' $missing
    err_count=$((err_count + 1))
fi

# Headers in smoke-headers.c but not in meson.build.
extra="$(comm -13 "$canonical" "$smoke" || true)"
if [ -n "$extra" ]; then
    red ""
    red "ERROR: in $SMOKE_HEADERS_FILE but not in any meson.build:"
    printf '    %s\n' $extra
    err_count=$((err_count + 1))
fi

# ---------------------------------------------------------------------------
# Result.
# ---------------------------------------------------------------------------
if [ "$err_count" -eq 0 ]; then
    green "OK: $SMOKE_HEADERS_FILE is in sync ($canonical_count headers across $HEADER_DIRS)."
    exit 0
fi

red ""
red "FAIL: $err_count drift(s) detected.  Fix $SMOKE_HEADERS_FILE."
exit 1
