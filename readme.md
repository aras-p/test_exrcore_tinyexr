## How tiny is tinyexr, and how big is OpenEXR?

[tinyexr](TODO LINK) is an excellent simple library for loading and saving OpenEXR
files. It has one big advantage, in that it is _very_ simple to start using: just one
source file to compile and include! However, it also has some downsides, namely that
not all features of OpenEXR are supported (for example, it can't do PXR24, B44/B44A,
DWAA/DWAB, HTJ2K compression modes), and performance might be behind the official
library. It probably can't do some of more exotic EXR features either (e.g. "deep" images),
but I'll ignore those for now.

But how _large_ and how _complex to use_ is the "official" OpenEXR library, anyways?

I do remember that a decade ago it was quite painful to build it, especially on anything
that is not Linux. However these days (2025), that seems to be much simpler: it
uses a CMake build system, and either directly vendors or automatically fetches whatever
dependencies it needs, unless you really ask it to "please don't do this".

It is not exactly a "one source file" library though. However, I noticed that OpenUSD
vendors OpenEXR "Core" library, builds it as a single C source file, and uses their
own "nanoexr" wrapper around the API; see
[OpenUSD/pxr/imaging/hio/OpenEXR](https://github.com/PixarAnimationStudios/OpenUSD/tree/262b84a7029c/pxr/imaging/hio/OpenEXR).
So I took that, adapted it to more recent OpenEXR versions (theirs uses 3.2.x, I updated to 3.4.4).

And then I wrote a tiny app that reads an EXR file, and writes it back as downsampled EXR
(so this includes both reading & writing parts of an EXR library). And compared how large
is the binary size between `tinyexr` and `OpenEXR`, as well as their respective
source code sizes and EXR read performance.

Notes:
- This compares both tinyexr and OpenEXR in fully single-threaded mode. Tinyexr has threading
  capabilities, but it spins up and shuts down a whole thread pool for each processed image,
  which is a bit "meh"; and while OpenEXRCore can be threaded (and using full high level
  OpenEXR library does use it that way), the "nanoexr" wrapper I took from USD codebase
  does not do any threading.
- Hardware is Apple M4 Max, compiler Xcode 16.1, Release build.


Inspiration in how to use it is from OpenUSD:
https://github.com/PixarAnimationStudios/OpenUSD/tree/262b84a7029c/pxr/imaging/hio/OpenEXR

- Take OpenEXR source repository (took post-v3.4.3, 2025 Nov 17, 59289eb),
	- Take only the `src/lib/OpenEXRCore` and `external/deflate` folders from it.
	- `openexr_config.h`, `compression.c`, `internal_ht.cpp` have local changes!
	  Look for `LOCAL CHANGE` comments.
- Take OpenJPH (https://github.com/aous72/OpenJPH) source code, used 0.25.3 (2025 Nov),
	put under `external/OpenJPH`.
- Take `openexr-c.c`, `openexr-c.h`, `OpenEXRCoreUnity.h` from the OpenUSD repository.
  They were for OpenEXR v3.2, and needed some adaptations for later versions.


### TinyEXR

- Take tinyexr source repository (took v1.0.12, 2025 Mar).

### Observations

Xcode 16.1.

tinyexr can't do: B44, DWAA, HTJ2K, PXR24 compression modes.

**OpenEXR 3.2.4 (2024 Mar)**

- Mac Release application with both OpenEXR and TinyEXR: 2547280 bytes.
	- Just OpenEXR: 2335472 bytes. source (w/ deflate): 8760984 bytes.
	- Just TinyEXR: 266256 bytes. source (w/ miniz): 739744 bytes.
	- None: 52208 bytes.

```
Reading images/Blender281junkshop-exr.exr:
- open 3840x2160 90.1ms data y 0..2160 ch 4 type 1
- tiny 3840x2160 198.0ms
Reading images/ph_brown_photostudio_02_8k.exr:
- open 8192x4096 889.0ms data y 0..4096 ch 4 type 2
- tiny 8192x4096 1007.8ms
```

**OpenEXR 3.3.5 (2025 Jul), tinyexr v1.0.12 (2025 Mar)**

- Mac Release application with both OpenEXR and TinyEXR: 1110928 bytes.
	- Just OpenEXR: 899136 bytes. source (w/ deflate): 3923354 bytes.
	- Just TinyEXR: 266256 bytes. source (w/ miniz): 743240 bytes.
	- None: 52208 bytes.

```
Reading images/Blender281junkshop-exr.exr:
- open 3840x2160 91.0ms data y 0..2160 ch 4 type 1
- tiny 3840x2160 198.6.0ms
Reading images/ph_brown_photostudio_02_8k.exr:
- open 8192x4096 581.9ms data y 0..4096 ch 4 type 2
- tiny 8192x4096 1011.5ms
```

**OpenEXR 3.4.3 (2025 Nov) with disabled HTJ2K, tinyexr v1.0.12 (2025 Mar)**

- Mac Release application with both OpenEXR and TinyEXR: 1111232 bytes.
	- Just OpenEXR: 899424 bytes. source (w/ deflate): 3965260 bytes.
	- Just TinyEXR: 266256 bytes. source (w/ miniz): 743240 bytes.
	- None: 52208 bytes.

```
Reading images/Blender281junkshop-exr.exr:
- open 3840x2160 90.9ms data y 0..2160 ch 4 type 1
- tiny 3840x2160 198.6.0ms
Reading images/ph_brown_photostudio_02_8k.exr:
- open 8192x4096 589.7ms data y 0..4096 ch 4 type 2
- tiny 8192x4096 1011.5ms
```

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


**OpenEXR post-3.4.3 (2025 Nov 17, 59289eb) with disabled HTJ2K; vendored libdeflate (1.23 -> 1.24), tinyexr v1.0.12 (2025 Mar)**

- Mac Release application with both OpenEXR and TinyEXR: 583104 bytes.
	- Just OpenEXR: 371328 bytes. source (w/ deflate): 1757350 bytes.
		- +disable DWA: 336832 bytes.
		- +disable B44: 319520 bytes.
		- +disable PXR24: 319344 bytes.
	- Just OpenEXR with HTJ2K: 678480 bytes (add: 307152 bytes). source: 3294203 bytes (add: 1536853 bytes).
	- Just TinyEXR: 266256 bytes. source (w/ miniz): 743240 bytes.
	- None: 52208 bytes.

```
Reading images/Blender281junkshop-exr.exr:
- open 3840x2160 91.2ms data y 0..2160 ch 4 type 1
- tiny 3840x2160 198.6.0ms
Reading images/ph_brown_photostudio_02_8k.exr:
- open 8192x4096 585.3ms data y 0..4096 ch 4 type 2
- tiny 8192x4096 1011.5ms
```
