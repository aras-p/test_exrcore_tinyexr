### OpenEXR


Inspiration in how to use it is from OpenUSD:
https://github.com/PixarAnimationStudios/OpenUSD/tree/262b84a7029c/pxr/imaging/hio/OpenEXR

- Take OpenEXR source repository (took post-3.3.2, main branch 8bc3fae, 2025 Jan),
	- Take only the `src/lib/OpenEXRCore` folder from it.
- Take libdeflate source repository (took v1.23, 78051988f9, 2024 Dec),
	- Take only the `lib` folder from it.
- Take `openexr-c.c`, `openexr-c.h`, `OpenEXRCoreUnity.h` from the OpenUSD repository.

### TinyEXR

- Take tinyexr source repository (took post-v1.0.9, release branch 756f7d3, 2025 Jan).

### Observations

- Mac Release application with both OpenEXR and TinyEXR: 1110928 bytes.
	- Just OpenEXR: 899024 bytes.
	- Just TinyEXR: 266160 bytes.
