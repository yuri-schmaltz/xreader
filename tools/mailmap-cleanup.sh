#!/usr/bin/env bash
#
# tools/mailmap-cleanup.sh - find author identities that need
# canonicalizing in .mailmap
#
# xreader's .mailmap (added in PR #20) folds 18 historical
# author/email variations into 78 canonical identities (down
# from 96 raw), so `git shortlog` and `git blame` show one
# row per real person.
#
# New author variations show up whenever a contributor changes
# their git config (different machine, different email
# provider) or when a third-party patch backports.  This
# script lists the author/email pairs that have more than
# one variation and prints a template .mailmap entry for each
# one, ready to paste into the file.
#
# Run from the repository root:
#
#   ./tools/mailmap-cleanup.sh                 # all variations, all time
#   ./tools/mailmap-cleanup.sh v2.6.0..master  # only since v2.6.0
#   ./tools/mailmap-cleanup.sh --incoming      # author names that appear in
#                                              # PRs opened since 90 days ago
#
# The output is meant to be reviewed by a human before being
# pasted in -- an over-zealous merge of two different people
# with the same display name is the failure mode this script
# cannot detect.

set -euo pipefail

repo_root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$repo_root"

red()   { printf '\033[31m%s\033[0m\n' "$*"; }
green() { printf '\033[32m%s\033[0m\n' "$*"; }
bold()  { printf '\033[1m%s\033[0m\n'  "$*"; }

range="HEAD"
incoming=0
since_days=90

apply=0

while [ $# -gt 0 ]; do
    case "$1" in
        --incoming)
            incoming=1
            shift
            ;;
        --since=*)
            since_days="${1#--since=}"
            shift
            ;;
        --apply)
            apply=1
            shift
            ;;
        -h|--help)
            sed -n '2,/^set -euo/p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *)
            range="$1"
            shift
            ;;
    esac
done

bold "==> Range: $range"

# Build the list of (name, email) tuples to inspect.
if [ "$incoming" -eq 1 ]; then
    # Use 'git log --since=N days' on the default branch.
    since_date=$(date -u -d "$since_days days ago" +%Y-%m-%d 2>/dev/null || date -u -v-"${since_days}d" +%Y-%m-%d)
    echo "  (incoming mode: authors with commits since $since_date)"
    mapfile -t identities < <(git log --since="$since_date" --format='%an <%ae>' | sort -u)
else
    mapfile -t identities < <(git log "$range" --format='%an <%ae>' | sort -u)
fi

[ "${#identities[@]}" -gt 0 ] || { red "no commits in range"; exit 0; }
echo "  ${#identities[@]} unique identity string(s) in range"

# Group by lowercased name.
bold "==> Grouping by name (case-insensitive)"

declare -A name_to_emails
for id in "${identities[@]}"; do
    name="${id% <*}"
    email="${id#*<}"
    email="${email%>}"
    key=$(echo "$name" | tr '[:upper:]' '[:lower:]')
    name_to_emails[$key]+="$id"$'\n'
done

# Print only the names that have more than one variation, and for each
# of those, only the email-variation rows (so the human can decide
# whether to merge).
bold "==> Suggested .mailmap entries"
echo

count_suggestions=0
for key in "${!name_to_emails[@]}"; do
    rows="${name_to_emails[$key]}"
    # Distinct email addresses across these rows.
    distinct_emails=$(printf '%s' "$rows" | sed -n 's/.*<\([^>]*\)>/\1/p' | sort -u)
    n_emails=$(printf '%s\n' "$distinct_emails" | wc -l | tr -d ' ')
    if [ "$n_emails" -gt 1 ]; then
        # Extract the canonical (most common) name form.
        canonical_name=$(printf '%s' "$rows" | sed 's/ <.*//' | sort | uniq -c | sort -rn | head -1 | sed 's/^ *[0-9]* *//')
        canonical_email=$(printf '%s' "$distinct_emails" | sort | uniq -c | sort -rn | head -1 | sed 's/^ *[0-9]* *//')

        count_suggestions=$((count_suggestions + 1))
        echo "--- $canonical_name ($n_emails email variations) ---"
        printf '%s' "$rows" | sort | uniq -c | sort -rn
        echo "# .mailmap candidate:"
        echo "# $canonical_name <$canonical_email>"
        for alias_email in $distinct_emails; do
            alias_name=$(printf '%s' "$rows" | grep -F "<$alias_email>" | head -1 | sed 's/ <.*//')
            if [ "$alias_email" != "$canonical_email" ] || [ "$alias_name" != "$canonical_name" ]; then
                echo "# $canonical_name <$canonical_email>  $alias_name <$alias_email>"
            fi
        done
        echo
    fi
done

if [ "$count_suggestions" -eq 0 ]; then
    green "No name has more than one email variation -- .mailmap is up to date."
else
    bold "==> Total: $count_suggestions name(s) with multiple email variations"
    if [ "$apply" -eq 1 ]; then
        # Auto-apply the candidates to .mailmap.
        # We need to capture the output to a temp file
        # (the script currently prints to stdout; refactor
        # to also write to a file when --apply is set).
        tmp=$(mktemp)
        "$0" "$@" > "$tmp" 2>&1 || true
        # Re-extract just the '# .mailmap candidate' blocks
        # and append to .mailmap.
        awk '/# .mailmap candidate:/,/^$/' "$tmp" | sed 's/^# //' >> .mailmap
        rm -f "$tmp"
        green "Applied $count_suggestions entries to .mailmap"
    else
        echo "Review the lines above and paste the candidate entries into .mailmap."
        echo "Each one is a comment (#) and will be ignored by git until uncommented."
        echo "Re-run with --apply to auto-add the candidates to .mailmap."
    fi
fi
