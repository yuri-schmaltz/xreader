# Tabbed view test plan

This document is the manual test plan for the C3 tabbed view feature
(PRs #96-#104).  The automated tests cover the EvTabManager API and
the reopen-closed-tab stack (24 unit tests total); the manual tests
cover the user-visible behavior.

## Pre-requisites

  - A working xreader build (4.8.0 or later)
  - At least 3 test PDF files (any PDF will do; small test files
    are in `tests/` after `meson setup` runs)
  - A way to take screenshots (for the visual tests)

## Test environment

Run xreader with `--tabbed` (or set the GSettings key):
```
gsettings set org.x.reader tabbed-mode true
xreader file1.pdf file2.pdf file3.pdf
```

## Test 1: Multi-tab opening

**Steps:**
  1. Launch xreader with 3 files: `xreader a.pdf b.pdf c.pdf`
  2. Verify 3 tabs are visible
  3. Verify the active tab is `c.pdf` (the last one)

**Expected:**
  - Tab bar is visible (3 tabs)
  - All 3 files are loaded
  - The window title shows `c.pdf`
  - The status bar shows the page count of `c.pdf`

## Test 2: Tab switching (click)

**Steps:**
  1. Click on the `a.pdf` tab
  2. Verify the active tab changes
  3. Click on `b.pdf`
  4. Click on `c.pdf`

**Expected:**
  - Each click switches the active tab
  - The window title updates
  - The page count in the status bar updates

## Test 3: Tab switching (keyboard)

**Steps:**
  1. With `c.pdf` active, press `Ctrl+Tab`
  2. Press `Ctrl+Tab` again
  3. Press `Ctrl+Tab` again (should wrap to `a.pdf`)
  4. Press `Ctrl+Shift+Tab` (should go to `c.pdf`)
  5. Press `Ctrl+PgUp` / `Ctrl+PgDn`

**Expected:**
  - All keyboard shortcuts work
  - Wrapping works (last -> first, first -> last)
  - Both `Ctrl+Tab` and `Ctrl+PgDn` cycle forward
  - Both `Ctrl+Shift+Tab` and `Ctrl+PgUp` cycle backward

## Test 4: Close tab (X button)

**Steps:**
  1. Click the X button on `b.pdf`
  2. Verify `b.pdf` is removed
  3. Verify the active tab is the previous one (or the next one if no previous)

**Expected:**
  - Tab is removed from the bar
  - The active tab is one of the remaining 2
  - The window title updates

## Test 5: Close tab (Ctrl+W)

**Steps:**
  1. With `a.pdf` active, press `Ctrl+W`
  2. Verify `a.pdf` is removed
  3. With the remaining tab active, press `Ctrl+W`

**Expected:**
  - First `Ctrl+W`: `a.pdf` is removed, active is the next tab
  - Second `Ctrl+W`: the remaining tab is removed; the window is closed
    (because there are 0 tabs left)

## Test 6: Reopen last closed (Ctrl+Shift+T)

**Steps:**
  1. With 3 tabs open, close the active one (`Ctrl+W`)
  2. Verify the tab is gone
  3. Press `Ctrl+Shift+T`
  4. Verify the tab is back

**Expected:**
  - The most recently closed tab is restored
  - The page number is preserved (the tab opens at the page that
    was shown when it was closed)
  - The active tab is the restored one

## Test 7: Reopen stack bounded (10 max)

**Steps:**
  1. With 1 tab open, `Ctrl+W` it (window closes)
  2. Re-open the file: `xreader --tabbed a.pdf`
  3. `Ctrl+W` (window closes)
  4. Repeat 15 times
  5. Re-open the file
  6. Press `Ctrl+Shift+T` 10 times
  7. Press `Ctrl+Shift+T` one more time (should be a no-op)

**Expected:**
  - After step 6, 10 tabs are restored (the original 1 + 9 reopens
    from steps 1-4 -- the oldest 5 from steps 1-2 are evicted
    because the stack is bounded at 10)

  - After step 7, the stack is empty; no tab is created
  - (This test is tedious; the automated `test-reopen-stack.c`
    covers the same logic without manual steps)

## Test 8: Drag-and-drop (per tab)

**Steps:**
  1. Open xreader with one tab: `xreader --tabbed a.pdf`
  2. From a file manager, drag `b.pdf` over the `a.pdf` tab
  3. Drop on the tab
  4. Verify `b.pdf` is opened in a new tab

**Expected:**
  - A new tab appears
  - The new tab is the active one
  - The window title shows `b.pdf`

## Test 9: Drag-and-drop (empty state)

**Steps:**
  1. Open xreader with no files: `xreader --tabbed`
  2. Verify the empty-state label is visible
  3. Drag any file over the empty area
  4. Drop
  5. Verify the file is opened in a tab

**Expected:**
  - The empty-state label disappears
  - A new tab appears with the dropped file
  - The tab bar is shown (1 tab is enough to hide the bar; the bar
    appears when there's >= 2 tabs)

## Test 10: Tab bar visibility

**Steps:**
  1. Open 0, 1, 2, 3, then close down to 0 tabs
  2. At each step, check the tab bar

**Expected:**
  - 0 tabs: empty-state label shown, tab bar hidden
  - 1 tab: tab bar hidden (1 tab is enough to hide the bar)
  - 2+ tabs: tab bar shown

## Test 11: Single-window mode (regression)

**Steps:**
  1. Disable tabbed mode: `gsettings set org.x.reader tabbed-mode false`
  2. Open xreader with 3 files: `xreader a.pdf b.pdf c.pdf`
  3. Verify 3 windows open

**Expected:**
  - Each file opens in its own window (the 4.7.0 behavior)
  - No tab bar
  - The application is still in single-window mode

## Test 12: Tab title update

**Steps:**
  1. Open a file with a long title (e.g. a PDF with a 50-char title)
  2. Verify the tab label is truncated

**Expected:**
  - The tab label is truncated to 20 chars with an ellipsis (`...`)
  - Hovering over the tab shows the full title + location

## Test 13: Persistence

**Steps:**
  1. Enable tabbed mode and open 3 files
  2. Close xreader
  3. Re-launch xreader (no file arguments)
  4. Verify... (TBD: session save/restore is 4.9.0)

**Expected (4.8.0):**
  - The 3 tabs are NOT restored (4.8.0 doesn't have session
    save/restore yet)
  - The tabbed-mode GSettings key is still TRUE

## Test 14: Re-enable after disable

**Steps:**
  1. `gsettings set org.x.reader tabbed-mode false`
  2. `xreader a.pdf` (opens in single-window mode)
  3. Close xreader
  4. `xreader --tabbed a.pdf` (forces tabbed mode for this session)
  5. Verify a tabbed window opens

**Expected:**
  - The CLI flag overrides the GSettings key
  - The tabbed window is created
  - The GSettings key is set to TRUE (the CLI flag persists the choice)

## Regression tests

The following 4.7.0 features must still work in 4.8.0:

  - **Single-window mode** (--tabbed not passed): regression test
    in test 11.
  - **Document loading**: all 14 backends work as before.
  - **Find, print, save**: operate on the active tab.
  - **Recent files**: shared across all tabs (in the same window).
  - **Window state persistence** (size, position): works as before.

## Test results template

```
Test 1 (Multi-tab opening):        PASS / FAIL
Test 2 (Tab switching click):      PASS / FAIL
Test 3 (Tab switching keyboard):   PASS / FAIL
Test 4 (Close tab X):              PASS / FAIL
Test 5 (Close tab Ctrl+W):         PASS / FAIL
Test 6 (Reopen Ctrl+Shift+T):      PASS / FAIL
Test 7 (Reopen stack bounded):     PASS / FAIL
Test 8 (Drag-and-drop per tab):    PASS / FAIL
Test 9 (Drag-and-drop empty):      PASS / FAIL
Test 10 (Tab bar visibility):      PASS / FAIL
Test 11 (Single-window mode):      PASS / FAIL
Test 12 (Tab title update):        PASS / FAIL
Test 13 (Persistence):             PASS / FAIL (4.9.0 feature)
Test 14 (Re-enable after disable): PASS / FAIL
Regression tests:                  PASS / FAIL
```
