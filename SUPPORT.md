# Support for Xreader

This document explains where to get help with Xreader.

## Before asking for help

Please check:

  1. **The man page** (`man xreader`) -- covers the
     command-line options, the keyboard shortcuts, and
     the supported document formats.
  2. **The help built into the app** -- `Help > Contents`
     in the menu, or `F1` from anywhere in the app.
  3. **The FAQ** -- below.

If your question is not answered by the above, the next
section explains how to get human help.

## How to get help

Xreader is community-supported.  There are several places
to ask questions, in order of preference:

  1. **Linux Mint forums** -- the project that hosts the
     `xreader` package on most distros has a very active
     community at <https://forums.linuxmint.com/>.  Most
     user questions are answered within hours.
  2. **The xreader fork issue tracker** --
     <https://github.com/yuri-schmaltz/xreader/issues>.
     Use this for bugs that are specific to the fork
     (e.g. a new feature that was added in the fork and
     not in upstream).
  3. **The Linux Mint IRC channel** --
     `#linuxmint-help` on Libera.Chat.  Real-time help
     from volunteers.
  4. **Email** -- if you prefer not to use any of the
     above, you can email the maintainer at
     `Mavis@local` (note: this is a low-traffic alias;
     response time is days, not hours).

## FAQ

### Q: Xreader won't open my file / my file is the wrong format

Xreader supports:

  - PDF (via poppler)
  - DjVu (via djvulibre)
  - XPS / OpenXPS (via libgxps)
  - DVI (via libkpathsea)
  - PostScript (via libspectre)
  - TIFF (single + multi-page)
  - Comic book archives (CBR / CBZ)
  - Plain text (with syntax highlighting for code files)
  - Images (PNG, JPEG, GIF, WebP, BMP, ...)

If your file is in a format Xreader doesn't support, try
a tool that does (e.g. `libreoffice` for Word documents).

### Q: Xreader crashes when I open a specific PDF

If the crash is reproducible, please file a bug report
with:

  1. The exact xreader version (`xreader --version`)
  2. The Linux distro and version
  3. The PDF file (if sharable; if not, a screenshot
     of the first page is enough to start)
  4. The exact steps to reproduce (e.g. 'open the
     attached file, then click on page 5')

If the crash happens in the libpoppler or libgxps
backend, the bug should be filed against the
corresponding library, not against xreader.

### Q: How do I enable / disable the 'night mode'?

Night mode is in the `View > Night Mode` menu (or
`Ctrl+Alt+N`).  It inverts the page colors to reduce
blue light.  The setting is per-document and per-window.

### Q: How do I print a document?

`File > Print` (or `Ctrl+P`).  Supports all the standard
GTK print dialog options: paper size, orientation, copies,
range, etc.

### Q: How do I fill out a PDF form?

**This feature is in development** (the B6 item in the
roadmap).  The poppler backend has the form data; the
UI to edit the data is being worked on.  Track via
the issue tracker.

In the meantime, you can use a PDF editor like
`pdfarranger` (FOSS, available in most distros) or
`masterpdfeditor` (proprietary, free for non-commercial
use) to fill out forms.

### Q: Does Xreader have a tabbed view?

Not yet.  The C3 item in the roadmap (4.14.0 target) is
a tabbed view, similar to Firefox's.  The work is
multi-PR and depends on the GAction migration (B1)
finishing first.

### Q: How do I report a security vulnerability?

See `SECURITY.md`.  Do NOT open a public issue.

## Bug reports

Before filing a bug:

  1. Reproduce on the latest version
     (see `xreader --version`).
  2. Check the existing issues to avoid duplicates:
     <https://github.com/yuri-schmaltz/xreader/issues>
  3. Check the upstream issue tracker too:
     <https://github.com/linuxmint/xreader/issues>

When you file a bug, include:

  - xreader version
  - Linux distro + version
  - The document (if sharable)
  - Steps to reproduce
  - The expected vs. actual behavior
  - The terminal output (`xreader --debug file.pdf`)

## Contributing fixes

See `CONTRIBUTING.md` and `HACKING.md`.
