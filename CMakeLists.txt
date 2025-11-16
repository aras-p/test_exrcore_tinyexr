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
	list(APPEND DEFINITIONS USE_TINYEXR)
endif ()

add_executable (test_exrcore_tinyexr ${SOURCES})

target_include_directories(test_exrcore_tinyexr PRIVATE
	openexr/external/deflate
	openexr
	tinyexr/miniz
)

set_property(TARGET test_exrcore_tinyexr PROPERTY CXX_STANDARD 17)

set_property(TARGET test_exrcore_tinyexr PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_definitions(test_exrcore_tinyexr PRIVATE ${DEFINITIONS})

#if(((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU")) AND
#	((CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")))
#	target_compile_options(test_exrcore_tinyexr PRIVATE -msse4.2)
#endif()
