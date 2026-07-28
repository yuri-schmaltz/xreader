# fuzz/corpus -- seed corpus for fuzz-document-load

The libFuzzer harness in `fuzz/fuzz-document-load.c` exercises
`ev_document_factory_get_document()` with random bytes.
libFuzzer is a coverage-driven fuzzer: it starts from a seed
corpus and mutates each input to explore new coverage paths.

This directory contains the project's seed corpus.  Each file
is a small (a few bytes to ~25 KB) example of a document format
the harness can try to parse.  Coverage is fastest when the
corpus is small and varied -- a 25 KB file that covers a
hundred code paths is more valuable than a hundred 1 KB files
that each cover one.

## Files

* `seed-encrypt.pdf` -- the encrypted test PDF from
  `test/test-encrypt.pdf` (PDF 1.4, 1 page).  Exercises the
  PDF back-end's password prompt and the poppler error path.

* `seed-links.pdf` -- the link-heavy test PDF from
  `test/test-links.pdf` (PDF 1.4, 2 pages).  Exercises the
  link-tree parsing and the outline / dest code paths.

* `seed-page-labels.pdf` -- the page-label test PDF from
  `test/test-page-labels.pdf` (PDF 1.4, 5 pages).  Exercises
  the page-label parsing in the poppler back-end.

* `seed-djvu.bin` -- the first 16 bytes of a DjVu document
  (`AT&TFORM....DJVM`).  Not a full document, just enough
  magic for `shared-mime-info` to classify it as
  `image/vnd.djvu` and dispatch it to the djvu back-end.
  The back-end will then return an error because the
  document is truncated, but the dispatch / MIME-detection
  code path is covered.

* `seed-dvi.bin` -- the first 5 bytes of a DVI document
  (`\xf7\x02PRE\n`).  Same idea as the DjVu seed: enough
  to identify the format, not a full document.

* `seed-epub.bin` -- a minimal ZIP archive (the local file
  header) with an EPUB-style `mimetype` entry.  Not a
  full EPUB, just the magic + the format-identifying
  file entry that the EPUB back-end looks for.

* `seed-tiff-le.bin` -- TIFF header, little-endian
  (`II*\0`).

* `seed-tiff-be.bin` -- TIFF header, big-endian
  (`MM\0*`).

* `seed-zip.bin` -- 4-byte ZIP magic.  Sniffs as
  `application/zip`, which is dispatched to the XPS back-end
  (because XPS is a ZIP) and to the EPUB back-end (because
  EPUB is also a ZIP).  The back-ends will both return an
  error on the truncated input.

* `seed-rar.bin` -- RAR magic (`Rar!\x1a\x07\x00`).
  Sniffs as `application/vnd.rar` or
  `application/x-rar-compressed`; the comics back-end (CBR)
  picks it up.

## Usage

The seed corpus is consumed by libFuzzer automatically; pass
the directory as a positional argument:

```
CC=clang meson setup build-fuzz -Dfuzz=true \
    -Db_sanitize=address,undefined,fuzzer
meson compile -C build-fuzz fuzz-document-load
./build-fuzz/fuzz-document-load fuzz/corpus/ -max_len=1048576
```

libFuzzer will mutate each seed in turn, save interesting
inputs (new coverage) into a parallel corpus directory of
its choice (default: the first positional argument), and
crash on the first assertion / segfault / use-after-free
it finds.

The corpus is not built or installed by meson -- it is
checked into the tree for repeatability.  A regenerated
corpus with new crashes is uploaded as the
`fuzz-crashes` artifact in the CI fuzz job (PR #19).

## Adding new seeds

When a new backend is added (e.g. CBZ, FB2) or an existing
backend gains a new top-level code path, drop a small
representative sample of the new format into this directory
and update this README.  The seed should be:

* as small as possible (a few bytes is fine, the fuzzer
  will mutate it);
* representative of the format (the right magic bytes, or
  the right MIME type, or the right top-level structure);
* not a full document (the fuzzer mutates the seed, so a
  full document just slows down the first iteration).
