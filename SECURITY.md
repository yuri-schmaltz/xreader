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
