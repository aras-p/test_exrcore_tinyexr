### OpenEXR


Inspiration in how to use it is from OpenUSD:
https://github.com/PixarAnimationStudios/OpenUSD/tree/262b84a7029c/pxr/imaging/hio/OpenEXR

- Take OpenEXR source repository (took v3.3.5, 2025 Jul),
	- Take only the `src/lib/OpenEXRCore` folder from it.
	- `openexr_config.h` has local changes!
- Take libdeflate source repository (took v1.23, 78051988f9, 2024 Dec),
	- Take only the `lib` folder from it.
- Take `openexr-c.c`, `openexr-c.h`, `OpenEXRCoreUnity.h` from the OpenUSD repository.

### TinyEXR

- Take tinyexr source repository (took v1.0.12, 2025 Mar).

### Observations

Xcode 16.1.

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
