# GtkAction → GAction migration plan

This document tracks the long-term migration of xreader's UI from the
deprecated GtkAction / UIManager API to the modern GAction / GMenu
API.  The migration is the B1 item in the project's roadmap
(see HACKING.md for the full roadmap).

## Status

As of 4.8.0 (in progress):

  - **PR #76** (4.7.0): added the GAction namespace
    (g_action_map_add_action_entries for `app_actions[]` with
    `open-location`).
  - **PR #77** (4.8.0): added 3 more GAction entries to
    `app_actions[]` (`quit`, `help`, `about`).
  - **PR #89** (4.8.0): added 3 more (`print`, `save`, `find`).

Total: 7 GAction entries registered with the EvApplication
GActionMap.  All are stubs (or trivial implementations) -- the
real work is in the dispatch (wiring the actions to the existing
ev_window_* functions).

## Why migrate

The GtkAction + UIManager API has been deprecated since GTK 3.10
(2013) and is removed in GTK 4.  Xreader tracks GTK 3, so the
deprecation warnings are not blocking the build, but:

  - Future GTK 4 porting (the C1 item) will be much easier with
    a clean GAction namespace already in place.
  - GAction entries are accessible via D-Bus, so external
    scripts can drive xreader (e.g. `gdbus call --session
    --dest org.xreader.Application --object-path /org/xreader/App
    --method org.gtk.Actions.Activate quit []`).
  - GAction has a strict state model (`activate` for stateless
    actions, `change-state` for stateful ones like `view-mode`).
    This makes the menu code easier to reason about.
  - GMenu (the modern menu API) integrates with the
    GApplication / GActionMap automatically, so a future
    .menu file replacement of the UIManager .ui file
    requires no action-mapping code.

## Plan

### Phase 1: namespace bootstrap (in progress, 4.8.0)

Add the 10-15 application-scope GAction entries to
ev-application.c (the `app_actions[]` table).  Each entry is
a stub or trivial implementation; the dispatch to the
existing `ev_window_*` functions is deferred to Phase 2.

Entries to add (in addition to the 7 already added):

  - `view-mode` (stateful: 'single' | 'continuous' | 'dual')
  - `zoom-in` / `zoom-out` / `zoom-reset` (stateful: 'normal')
  - `next-page` / `prev-page` (stateless)
  - `reload` (stateless)
  - `fullscreen` (stateful: 'normal' | 'fullscreen')
  - `presentation` (stateful: 'normal' | 'presentation')
  - `night-mode` (stateful: 'normal' | 'inverted')

Total: 14 entries in `app_actions[]` when Phase 1 is complete.

### Phase 2: window-scope GAction namespace (4.9.0)

The application-scope actions (Phase 1) are stateless across
windows.  The window-scope actions (zoom, page nav, view mode)
need to be registered per-EvWindow.

Add an `ev_window_class_install_actions()` that calls
`g_action_map_add_action_entries()` for the
`window_actions[]` table (10-12 entries).

This is a 1-PR change (no menu wiring).

### Phase 3: dispatch (4.9.0 - 4.10.0)

For each `app_actions[]` and `window_actions[]` entry, wire
the activate callback to the existing `ev_window_*` function.

For example, `quit_action_activate` should call
`g_application_quit(G_APPLICATION(application))` (already done
in PR #77); `next_page_action_activate` should call
`ev_window_next_page(EV_WINDOW(active_window))`.

This is a 5-7 PR series, one per logical group (file ops,
view ops, edit ops, etc.).

### Phase 4: menu replacement (4.11.0 - 4.12.0)

Replace the UIManager .ui files (`shell/ev-window-ui.xml`,
`shell/ev-application-ui.xml`) with .menu files
(`shell/ev-window.menu`, `shell/ev-application.menu`).

The .menu file format uses `<attribute name="action">`
references instead of the old `<action name="...">` /
`<accelerator>` combo.  The full menu can be replaced in 1-2
PRs once Phases 1-3 are complete.

### Phase 5: GtkAction removal (4.13.0)

Once Phases 1-4 are complete and the .ui files are gone, do
a global grep for `GtkAction` and remove the remaining
references.  After this PR the codebase has no GtkAction
references and is fully on the modern API.

The 6,127-line `shell/ev-window.c` will be the most-touched
file in this phase (the UIManager glue code is in there).

## Risk mitigation

  - **Big-bang vs. incremental**: incremental, with the UIManager
    paths kept in parallel for 1-2 releases after each phase.
  - **Testing**: the dispatch (Phase 3) is the riskiest part.
    A new `test-gaction-dispatch.c` will be added in Phase 3
    to verify each action routes to the right `ev_window_*`
    function.
  - **Backwards compat**: until Phase 5, the old GtkAction
    `activate` handlers are still wired to the same
    `ev_window_*` functions; removing them in Phase 5 changes
    nothing for users.
