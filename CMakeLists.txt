cmake_minimum_required (VERSION 3.21)
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # enable MSVC_RUNTIME_LIBRARY
endif()

project ("test_exrcore_tinyexr")

set(USE_OPENEXR ON)
set(USE_TINYEXR ON)

set(SOURCES test_exrcore_tinyexr.cpp)
set(DEFINITIONS CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_WARNINGS NOMINMAX)

if (USE_OPENEXR)
	list(APPEND SOURCES
		openexr/openexr-c.c
		openexr/openexr-c.h
        openexr/OpenEXRCoreUnity.h
        
        openexr/src/lib/OpenEXRCore/internal_ht_common.cpp
        openexr/src/lib/OpenEXRCore/internal_ht.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codeblock_fun.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codeblock.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_avx.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_avx2.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_gen.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_local.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_sse.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream_sse2.cpp
        #openexr/external/OpenJPH/src/core/codestream/ojph_codestream_wasm.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_codestream.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_params.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_precinct.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_resolution.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_subband.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_tile_comp.cpp
        openexr/external/OpenJPH/src/core/codestream/ojph_tile.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_common.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_decoder_avx2.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_decoder_ssse3.cpp
        #openexr/external/OpenJPH/src/core/coding/ojph_block_decoder_wasm.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_decoder32.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_decoder64.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_encoder_avx2.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_encoder_avx512.cpp
        openexr/external/OpenJPH/src/core/coding/ojph_block_encoder.cpp
        openexr/external/OpenJPH/src/core/others/ojph_arch.cpp
        openexr/external/OpenJPH/src/core/others/ojph_file.cpp
        openexr/external/OpenJPH/src/core/others/ojph_mem.c
        openexr/external/OpenJPH/src/core/others/ojph_mem.cpp
        openexr/external/OpenJPH/src/core/others/ojph_message.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_colour_avx.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_colour_avx2.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_colour_sse.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_colour_sse2.cpp
        #openexr/external/OpenJPH/src/core/transform/ojph_colour_wasm.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_colour.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform_avx.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform_avx2.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform_avx512.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform_sse.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform_sse2.cpp
        #openexr/external/OpenJPH/src/core/transform/ojph_transform_wasm.cpp
        openexr/external/OpenJPH/src/core/transform/ojph_transform.cpp
	)
	list(APPEND DEFINITIONS USE_OPENEXR)
endif ()
if (USE_TINYEXR)
	list(APPEND SOURCES
		tinyexr/tinyexr.cc
		tinyexr/tinyexr.h
		tinyexr/miniz/miniz.c
		tinyexr/miniz/miniz.h
	)
	list(APPEND DEFINITIONS USE_TINYEXR TINYEXR_USE_THREAD=1)
endif ()

add_executable (test_exrcore_tinyexr ${SOURCES})

target_include_directories(test_exrcore_tinyexr PRIVATE
	openexr
    openexr/external/OpenJPH/src/core/common
	tinyexr/miniz
)

set_property(TARGET test_exrcore_tinyexr PROPERTY CXX_STANDARD 17)

set_property(TARGET test_exrcore_tinyexr PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_definitions(test_exrcore_tinyexr PRIVATE ${DEFINITIONS})

# Enable debug symbols (RelWithDebInfo is not only that; it also turns on
# incremental linking, disables some inlining, etc. etc.)
set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
set(CMAKE_XCODE_ATTRIBUTE_COPY_PHASE_STRIP "NO")
set(CMAKE_XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT "NO")
# note: this does not work; have to pass -g instead
#set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	if (MSVC)
		target_compile_options(test_exrcore_tinyexr PRIVATE /Zi)
		target_link_options(test_exrcore_tinyexr PRIVATE /DEBUG /OPT:ICF /OPT:REF)
	endif()
	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		target_compile_options(test_exrcore_tinyexr PRIVATE -g)
		target_link_options(test_exrcore_tinyexr PRIVATE -g)
	endif()
endif ()


#if(((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU")) AND
#	((CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")))
#	target_compile_options(test_exrcore_tinyexr PRIVATE -msse4.2)
#endif()
