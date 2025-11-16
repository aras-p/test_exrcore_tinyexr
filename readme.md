### OpenEXR


Inspiration in how to use it is from OpenUSD:
https://github.com/PixarAnimationStudios/OpenUSD/tree/262b84a7029c/pxr/imaging/hio/OpenEXR

- Take OpenEXR source repository (took post-v3.4.3, 2025 Nov 14, 90b6738),
	- Take only the `src/lib/OpenEXRCore` and `external/deflate` folders from it.
	- `openexr_config.h` and `compression.c` have local changes! Look for `LOCAL CHANGE` comments.
	- The new in v3.4 HTJ2K compression is disabled for now.
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

**OpenEXR post-3.4.3 (2025 Nov 14, 90b6738) with disabled HTJ2K; vendored libdeflate (1.23 -> 1.24), tinyexr v1.0.12 (2025 Mar)**

- Mac Release application with both OpenEXR and TinyEXR: 583104 bytes.
	- Just OpenEXR: 371616 bytes. source (w/ deflate): 1757350 bytes.
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
