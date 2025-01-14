cmake_minimum_required (VERSION 3.21)
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW) # enable MSVC_RUNTIME_LIBRARY
endif()

project ("test_exrcore_tinyexr")

add_executable (test_exrcore_tinyexr
	test_exrcore_tinyexr.cpp
	openexr/openexr-c.c
)

set_property(TARGET test_exrcore_tinyexr PROPERTY CXX_STANDARD 17)

set_property(TARGET test_exrcore_tinyexr PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_compile_definitions(test_exrcore_tinyexr PRIVATE _CRT_SECURE_NO_DEPRECATE _CRT_NONSTDC_NO_WARNINGS NOMINMAX)

#if(((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU")) AND
#	((CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64") OR (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")))
#	target_compile_options(test_exrcore_tinyexr PRIVATE -msse4.2)
#endif()
