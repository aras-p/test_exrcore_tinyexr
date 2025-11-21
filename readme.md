## How tiny is tinyexr, and how big is OpenEXR?

[tinyexr](https://github.com/syoyo/tinyexr) is an excellent simple library for loading and saving OpenEXR
files. It has one big advantage, in that it is _very_ simple to start using: just one
source file to compile and include! However, it also has some downsides, namely that
not all features of OpenEXR are supported (for example, it can't do PXR24, B44/B44A,
DWAA/DWAB, HTJ2K compression modes), and performance might be behind the official
library. It probably can't do some of more exotic EXR features either (e.g. "deep" images),
but I'll ignore those for now.

But how _large_ and how _complex to use_ is the "official" [OpenEXR library](https://github.com/AcademySoftwareFoundation/openexr),
anyways?

I do remember that a decade ago it was quite painful to build it, especially on anything
that is not Linux. However these days (2025), that seems to be much simpler: it
uses a CMake build system, and either directly vendors or automatically fetches whatever
dependencies it needs, unless you really ask it to "please don't do this".

It is not exactly a "one source file" library though. However, I noticed that [OpenUSD](https://github.com/PixarAnimationStudios/OpenUSD)
vendors OpenEXR "Core" library, builds it as a single C source file, and uses their
own "nanoexr" wrapper around the API; see
[pxr/imaging/plugin/hioOpenEXR/OpenEXR](https://github.com/PixarAnimationStudios/OpenUSD/tree/8d2d14db0/pxr/imaging/plugin/hioOpenEXR/OpenEXR).
So I took that, adapted it to more recent OpenEXR versions (theirs uses 3.2.x, I updated to 3.4.4).

And then I wrote a tiny app that reads an EXR file, and writes it back as downsampled EXR
(so this includes both reading & writing parts of an EXR library). And compared how large
is the binary size between `tinyexr` and `OpenEXR`, as well as their respective
source code sizes and EXR read performance.

Actual process was:
- Take OpenEXR source repository (v3.4.4, 2025 Nov),
	- Take only the `src/lib/OpenEXRCore` and `external/deflate` folders from it.
	- `openexr_config.h`, `compression.c`, `internal_ht.cpp` have local changes!
	  Look for `LOCAL CHANGE` comments.
- Take OpenJPH (https://github.com/aous72/OpenJPH) source code, used 0.25.3 (2025 Nov),
	put under `external/OpenJPH`.
- Take `openexr-c.c`, `openexr-c.h`, `OpenEXRCoreUnity.h` from the OpenUSD repository.
  They were for OpenEXR v3.2, and needed some adaptations for later versions.
- Take `tinyexr` source repository (v1.0.12, 2025 Mar).

## Results

| Library | Binary size, KB | Source size, KB | EXR (ZIP) read time, ms | Notes |
|----|---:|---:|---:|---|
|tinyexr 1.0.12       |  260 |  726 | 1008 | |
|OpenEXR 3.2.4        | 2281 | 8556 |  889 | |
|OpenEXR 3.3.5        |  878 | 3831 |  582 | Removed giant DWAA/DWAB lookup tables. |
|OpenEXR 3.4.3        | 1178 | 5373 |  587 | Added HTJ2K compression (via OpenJPH). |
|OpenEXR 3.4.4        |  662 | 3216 |  585 | Removed more B44/DWA lookup tables. |
| + no HTJ2K          |  363 | 1716 |      | Above, with HTJ2K/OpenJPH compiled out. |
| + no DWA            |  329 |      |      | Above, and with DWAA/DWAB compiled out. |
| + no B44            |  312 |      |      | Above, and with B44/B44A compiled out. |
| + no PXR24          |  312 |      |      | Above, and with PXR24 compiled out. |

Notes:
- This compares both tinyexr and OpenEXR in fully single-threaded mode. Tinyexr has threading
  capabilities, but it spins up and shuts down a whole thread pool for each processed image,
  which is a bit "meh"; and while OpenEXRCore can be threaded (and using full high level
  OpenEXR library does use it that way), the "nanoexr" wrapper I took from USD codebase
  does not do any threading.
- Hardware is Apple M4 Max, compiler Xcode 16.1, Release build.

## Takeaways

- OpenEXR has been mostly getting smaller over time, nice! Two large size reductions have been
  where Kimball Thurston has removed very large DWA compression lookup tables (for v3.3,
  PR [#1948](https://github.com/AcademySoftwareFoundation/openexr/pull/1948), 1.4MB size reduction),
  and I have just removed some more tables (for v3.4.4, PRs [#2126](https://github.com/AcademySoftwareFoundation/openexr/pull/2126),
  [#2174](https://github.com/AcademySoftwareFoundation/openexr/pull/2174), 0.5MB size reduction).
- However, addition of the new HTJ2K compression codec in v3.4 has increased the OpenEXR library
  size by 0.3MB.
- Tinyexr is smaller, but not _that much_ smaller than OpenEXR. And, again, it does not
  understand some of the compression modes (PXR24, B44, DWA, HTJ2K) and thus can't load
  some EXR files. Also seems to be about 2x slower than OpenEXR library.
