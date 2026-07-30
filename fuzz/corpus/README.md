# fuzz/corpus -- seed corpus for fuzz-document-load + fuzz-document-load-all

The libFuzzer harnesses in `fuzz/fuzz-document-load.c` (PR #13)
and `fuzz/fuzz-document-load-all.c` (PR #42) exercise
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

* `test-encrypt.pdf` -- the encrypted test PDF from
  `test/test-encrypt.pdf` (PDF 1.4, 1 page, 26 KB).  Exercises
  the PDF back-end's password prompt and the poppler error path.
* `test-links.pdf` -- the test PDF from `test/test-links.pdf`
  (PDF 1.4, 5 pages with internal + external links, 15 KB).
  Exercises the poppler link-extraction code path.
* `test-page-labels.pdf` -- the test PDF from
  `test/test-page-labels.pdf` (PDF 1.4, 3 pages with custom
  page labels, 10 KB).  Exercises the poppler page-labels API.
* `test-mime.bin` -- the test MIME-bytes from
  `test/test-mime.bin` (1 KB).  Exercises the MIME detection
  path via the `ev_file_get_mime_type()` helper.
* `seed-djvu.bin` -- DjVu magic bytes (`AT&TFORM...DJVM`, 16
  bytes).  Exercises the djvu back-end's initial load.
* `seed-dvi.bin` -- DVI magic bytes (`F7 02 ...`, 8 bytes).
  Exercises the dvi back-end's initial load.
* `seed-epub.bin` -- EPUB zip header (PK, mimetype entry, 64
  bytes).  Exercises the epub back-end's initial load.
* `seed-rar.bin` -- RAR magic bytes (`Rar!\x1a\x07\x00`, 7
  bytes).  Exercises the comics back-end's RAR detection path.
* `seed-tiff-be.bin` -- TIFF big-endian magic bytes
  (`MM\x00\x2a\x00\x00\x00\x08`, 8 bytes).  Exercises the
  tiff back-end's BE parse path.
* `seed-tiff-le.bin` -- TIFF little-endian magic bytes
  (`II\x2a\x00\x08\x00\x00\x00\x00`, 10 bytes).  Exercises
  the tiff back-end's LE parse path.
* `seed-zip.bin` -- ZIP magic bytes (`PK\x03\x04`, 4 bytes).
  Exercises the comics + epub back-ends' ZIP detection.

## How to use

```sh
CC=clang meson setup build-fuzz -Dfuzz=true \
    -Db_sanitize=address,undefined,fuzzer
meson compile -C build-fuzz fuzz-document-load fuzz-document-load-all
./build-fuzz/fuzz-document-load      fuzz/corpus/ -max_len=1048576
./build-fuzz/fuzz-document-load-all  fuzz/corpus/ -max_len=1048576
```

libFuzzer will mutate each seed in turn, save interesting
inputs (new coverage) into a parallel corpus directory of
its choice (default: the first positional argument), and
crash on the first assertion / segfault / use-after-free
it finds.

The corpus is not built or installed by meson -- it is
checked into the tree for repeatability.  A regenerated
corpus with new crashes is uploaded as the `fuzz-crashes`
artifact in the CI fuzz job (PR #19).

## Adding new seeds

When a new backend is added (e.g. FB2) or an existing
backend gains a new top-level code path, drop a small
representative sample of the new format into this directory
and update this README.  The seed should be:

* as small as possible (a few bytes is fine, the fuzzer
  will mutate it);
* representative of the format (the right magic bytes, or
  the right MIME type, or the right top-level structure);
* not a full document (the fuzzer mutates the seed, so a
  full document just slows down the first iteration).

## History

* PR #29 (2026-07-28): Initial seed corpus -- 8 files.
* This PR: Expand to 11 files by adding the 3 real test
  PDFs from `test/` (replacing the synthetic 1-page
  seed-encrypt / seed-links / seed-page-labels) plus
  `test-mime.bin` for the MIME detection path.
