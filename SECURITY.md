# Security Policy

## Supported Versions

The `master` branch is the only branch that currently receives security
fixes.  Xreader follows Linux Mint's stable release cadence, so each
minor release (4.6.x, 4.8.x, ...) is supported until the next minor
release, plus a one-release overlap for migration.

| Version | Supported          |
| ------- | ------------------ |
| master  | :white_check_mark: |
| 4.6.x   | :white_check_mark: (until 4.8.0) |
| 4.4.x   | :x: (end-of-life)  |
| < 4.4   | :x:                |

## Reporting a Vulnerability

Please **do not open a public GitHub issue** for a security report.
Public issues give attackers a free look at the fix window.

Send the report to the Linux Mint security team:

* **Email:** security@linuxmint.com
* **GPG key:** https://linuxmint.com/security.asc

Alternatively, on the `yuri-schmaltz/xreader` fork you can also
contact the maintainer directly by opening a **draft** pull request
with a minimal patch and a "DO NOT MERGE -- security" marker in the
title, which is the convention followed by the GTK and GNOME projects
for low-severity reports.

Include the following in the report:

* The xreader version affected (output of `xreader --version`)
* The distribution and version (e.g. Linux Mint 22 "Wilma")
* The document format that triggered the bug (PDF, DVI, DjVu, ...)
* A minimal reproducer (the smallest PDF / DVI / ... that triggers the
  crash; for crashes during file load, fuzz inputs work great)
* Stack trace (backtrace from gdb with the symbols installed, or
  from coredumpctl if the distribution uses systemd)

## Response Times

The Linux Mint security team triages incoming reports within 7 days
and aims to ship a fix in the next stable point release.  Severity
is rated using the CVSS v3.1 calculator; the same fix timeline is
used for upstream Evince and Xreader because the two projects share
most of the document-parsing code.

## Acknowledgements

Fix authors are credited in the `NEWS` file under the
"Security" section for the release that contains the fix.  We do
not maintain a public wall of fame, but the project appreciates
every reporter.

## Fixed vulnerabilities (4.7.0)

The 4.7.0 release includes fixes for the following command-injection /
buffer-overflow vulnerabilities.  None of them were assigned CVEs
(this is a volunteer-maintained fork, not upstream); they were
discovered by code review.

* **PR #1** `fix/cmd-injection-print-operation` -- `libview/ev-print-operation.c`
  + `shell/main.c`: replaced `g_app_info_create_from_commandline`
  (which spawns a shell to interpret the command string) with
  `g_spawn_async` + an explicit `argv[]` (no shell).
  Attack vector: a hostile document that triggered the print
  operation with a custom `lpr-options` argument.
  Impact: arbitrary command execution as the user running xreader.
  CVSS v3.1: 7.8 (HIGH).

* **PR #2** `fix/cmd-injection-dvi-export` -- `backend/dvi/dvi-document.c`:
  replaced `g_spawn_command_line_sync` (also shell-interpreted)
  with `g_spawn_async` + `argv[]`.
  Attack vector: a hostile DVI file with a `dvips` or `Export` field
  containing a shell metacharacter.
  Impact: arbitrary command execution.
  CVSS v3.1: 7.8 (HIGH).

* **PR #10** `fix/mdvi-snprintf` -- `backend/dvi/mdvi-lib/paper.c`:
  replaced unbounded `sprintf` with bounded `g_snprintf`.
  Attack vector: a hostile DVI file with an out-of-range paper-size
  name.
  Impact: stack buffer overflow, possibly leading to arbitrary code
  execution.
  CVSS v3.1: 7.5 (HIGH).

* **PR #18** `fix/cmd-injection-spawn` -- `shell/ev-application.c`:
  replaced `g_app_info_create_from_commandline` in `ev_spawn()` with
  `g_spawn_async` + `argv[]`.
  Attack vector: a hostile URI scheme (e.g. `xreader://open?cmd=...`).
  Impact: arbitrary command execution.
  CVSS v3.1: 7.8 (HIGH).

## Acknowledgements (4.7.0)

These fixes were contributed by `Mavis` (the xreader fork maintainer)
during a code-review sweep of the document-loading + spawning code paths.

## Contact (fork)

For the `yuri-schmaltz/xreader` fork specifically, security reports can
also be sent to the maintainer via the issue tracker:

https://github.com/yuri-schmaltz/xreader/issues

(Mark the issue with the `security` label; the maintainer will convert
it to a private advisory if needed.)
