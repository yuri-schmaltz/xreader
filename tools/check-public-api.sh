#!/usr/bin/env bash
#
# tools/check-public-api.sh - verify that every public GType macro
#                              declared in a header has a matching
#                              G_DEFINE_TYPE in the .c file.
#
# This is the C analogue of the Python "is this module
# imported?" sanity check.  It catches:
#   - A header that declares EV_TYPE_FOO and ev_foo_get_type
#     but the .c file has no G_DEFINE_TYPE for it (would
#     fail to link)
#   - A .c file with G_DEFINE_TYPE but the header is
#     missing the macro (the type is private but the .c
#     still exports it -- usually a bug)
#
# Run from the repository root:
#   ./tools/check-public-api.sh
#
# Exit codes:
#   0  -- every public GType macro has a matching define
#   1  -- drift detected; the diff is printed
#
# Cost: <1s, pure bash + grep.  No deps.

set -u  # NOTE: do NOT use 'set -e' -- some greps return 1
        # on no match and that's not a script error.

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"

red()   { printf '\033[31m%s\033[0m\n' "$*"; }
green() { printf '\033[32m%s\033[0m\n' "$*"; }
bold()  { printf '\033[1m%s\033[0m\n'  "$*"; }

err_count=0

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------
HEADER_DIRS="libdocument libview shell"

# ---------------------------------------------------------------------------
# 1. Extract the set of GType names declared in public headers.
#
# A "GType macro declaration" is a line of the form
#   GType   ev_foo_get_type   (void) G_GNUC_CONST;
# We extract ev_foo_get_type, lowercase the prefix, and
# also collect the corresponding EV_TYPE_FOO macro name.
# ---------------------------------------------------------------------------
declared_get_types_file="$(mktemp)"
declared_type_macros_file="$(mktemp)"
trap 'rm -f "$declared_get_types_file" "$declared_type_macros_file" "$defined_get_types_file"' EXIT

for dir in $HEADER_DIRS; do
    [ -d "$dir" ] || continue

    # Find GType _get_type declarations in public headers.
    # Match: "GType ev_foo_bar_get_type (void) G_GNUC_CONST ;"
    # The function name is the second token; the parameter
    # list is the third.  We use a simple awk for clarity.
    for header in "$dir"/*.h; do
        [ -f "$header" ] || continue

        # GType function declarations.
        awk '/^GType[[:space:]]+(ev_[a-z_0-9]+_get_type)[[:space:]]*\(/ {
            print $2
        }' "$header" >> "$declared_get_types_file"

        # EV_TYPE_FOO macros (for cross-checking).
        # Match: "#define EV_TYPE_FOO  ...".
        # We extract FOO (after EV_TYPE_).
        awk '/^#define[[:space:]]+EV_TYPE_[A-Z_0-9]+[[:space:]]/ {
            sub(/^#define[[:space:]]+EV_TYPE_/, "")
            sub(/[[:space:]].*$/, "")
            print $0
        }' "$header" >> "$declared_type_macros_file"
    done
done

sort -u "$declared_get_types_file" -o "$declared_get_types_file"
sort -u "$declared_type_macros_file" -o "$declared_type_macros_file"

declared_count="$(wc -l < "$declared_get_types_file")"
declared_macro_count="$(wc -l < "$declared_type_macros_file")"

# ---------------------------------------------------------------------------
# 2. Extract the set of G_DEFINE_TYPE / G_DEFINE_INTERFACE / G_DEFINE_ABSTRACT_TYPE
# entries in the .c files.  We extract the C identifier (the
# first argument of the macro), lowercase it, and check whether
# the corresponding ev_X_get_type function is declared.
# ---------------------------------------------------------------------------
defined_get_types_file="$(mktemp)"

for dir in $HEADER_DIRS; do
    [ -d "$dir" ] || continue

    for src in "$dir"/*.c; do
        [ -f "$src" ] || continue

        # G_DEFINE_TYPE (ClassName, cname, ParentType)
        # G_DEFINE_ABSTRACT_TYPE (ClassName, cname, ParentType)
        # G_DEFINE_INTERFACE (ClassName, cname, PrerequisiteType)
        # G_DEFINE_TYPE_EXTENDED (...)
        # G_DEFINE_TYPE_WITH_CODE (ClassName, cname, ParentType, code...)
        # G_DEFINE_ABSTRACT_TYPE_WITH_CODE (...)
        # G_DEFINE_BOXED_TYPE (ClassName, cname, copy_func, free_func)
        # G_DEFINE_BOXED_TYPE_EXTENDED (...)
        # G_DEFINE_POINTER_TYPE (ClassName, cname)
        # G_DEFINE_ENUM_TYPE (ClassName, cname, get_type_func)
        # G_DEFINE_FLAGS_TYPE (...)
        # The lowercase cname is the second argument.
        awk '
            /^G_DEFINE_(ABSTRACT_)?TYPE([[:space:]]|_EXTENDED|_WITH_CODE|_WITH_PRIVATE)/ ||
            /^G_DEFINE_INTERFACE[[:space:]]*\(/ ||
            /^G_DEFINE_BOXED_TYPE([[:space:]]|_EXTENDED)/ ||
            /^G_DEFINE_POINTER_TYPE[[:space:]]*\(/ ||
            /^G_DEFINE_ENUM_TYPE[[:space:]]*\(/ ||
            /^G_DEFINE_FLAGS_TYPE([[:space:]]|_WITH_CODE)/ {
                # Read the parenthesized args (could span lines)
                line = $0
                while (line !~ /\)/) {
                    getline next_line
                    line = line " " next_line
                }
                # Extract the 2nd argument
                sub(/^G_DEFINE_[A-Z_]*\(/, "", line)
                sub(/\).*$/, "", line)
                split(line, args, ",")
                cname = args[2]
                gsub(/[[:space:]]+/, "", cname)
                print cname "_get_type"
            }
        ' "$src" >> "$defined_get_types_file" 2>/dev/null
    done
done

sort -u "$defined_get_types_file" -o "$defined_get_types_file"
defined_count="$(wc -l < "$defined_get_types_file")"

# ---------------------------------------------------------------------------
# 3. Cross-check: every declared ev_X_get_type must have a
# matching defined X_get_type.
# ---------------------------------------------------------------------------

bold "==> $declared_count public GType decls vs $defined_count G_DEFINE_TYPE entries"

missing_defines="$(comm -23 "$declared_get_types_file" "$defined_get_types_file" 2>/dev/null || true)"
if [ -n "$missing_defines" ]; then
    red ""
    red "ERROR: GType declared in header but no G_DEFINE_TYPE in any .c file:"
    printf '    %s\n' $missing_defines
    err_count=$((err_count + 1))
fi

extra_defines="$(comm -13 "$declared_get_types_file" "$defined_get_types_file" 2>/dev/null || true)"
if [ -n "$extra_defines" ]; then
    red ""
    red "WARNING: G_DEFINE_TYPE in .c file but no public declaration in any .h:"
    printf '    %s\n' $extra_defines
    # This is a WARNING, not an error -- sometimes a G_DEFINE_TYPE
    # is for a private type (e.g. an enum boilerplate, an
    # internal helper).  Print but don't fail.
fi

# ---------------------------------------------------------------------------
# 4. Cross-check: every EV_TYPE_FOO macro should have a matching
# ev_foo_get_type declaration.  This catches a typo or
# forgotten prototype.
# ---------------------------------------------------------------------------

# Lowercase the macro names to match the function names.
# EV_TYPE_FOO -> ev_foo_get_type (note the ev_ prefix).
declared_macro_lowered_file="$(mktemp)"
trap 'rm -f "$declared_get_types_file" "$declared_type_macros_file" "$declared_macro_lowered_file" "$defined_get_types_file"' EXIT

awk '{ print "ev_" tolower($0) "_get_type" }' "$declared_type_macros_file" > "$declared_macro_lowered_file"
sort -u "$declared_macro_lowered_file" -o "$declared_macro_lowered_file"

macro_mismatch="$(comm -23 "$declared_macro_lowered_file" "$declared_get_types_file" 2>/dev/null || true)"
if [ -n "$macro_mismatch" ]; then
    red ""
    red "WARNING: EV_TYPE_FOO macro defined but matching ev_foo_get_type() not found:"
    printf '    %s\n' $macro_mismatch
    red "  (this is often a real bug -- the macro expands to a function that"
    red "   is never defined.  In some cases it's a naming quirk where the"
    red "   macro name and the function name don't match by design, e.g."
    red "   EV_TYPE_PROPERTIES -> ev_properties_view_get_type.  Manual review.)"
    # WARNING, not an error -- false positives exist for legacy quirks.
fi

# ---------------------------------------------------------------------------
# 5. Result.
# ---------------------------------------------------------------------------

if [ "$err_count" -eq 0 ]; then
    green ""
    green "OK: $declared_count public GType functions, $defined_count G_DEFINE_TYPE entries, in sync."
    exit 0
fi

red ""
red "FAIL: $err_count drift(s) detected."
exit 1
