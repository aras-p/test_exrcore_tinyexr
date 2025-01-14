// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) Contributors to the OpenEXR Project.

// Manually assembled with just what we need (in full OpenEXR this is generated
// during build configure time)

#ifndef INCLUDED_OPENEXR_INTERNAL_CONFIG_H
#define INCLUDED_OPENEXR_INTERNAL_CONFIG_H 1

#pragma once

// Define if we can support GCC style inline asm with AVX instructions
//#define OPENEXR_IMF_HAVE_GCC_INLINE_ASM_AVX 1

// Define if we need to shim in our own implementation of vld1q_f32_x2 for
// older compilers that are missing x2 Neon intrinsics on aarch64
//#define OPENEXR_MISSING_ARM_VLD1 0

#endif // INCLUDED_OPENEXR_INTERNAL_CONFIG_H
