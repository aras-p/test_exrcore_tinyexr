//
// Copyright 2023 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
/// \file OpenEXRCoreUnity.h

#include "OpenEXRCore/openexr_config.h"

#include "libdeflate/lib/lib_common.h"
#include "libdeflate/common_defs.h"
#include "libdeflate/lib/utils.c"
#include "libdeflate/lib/arm/cpu_features.c"
#include "libdeflate/lib/x86/cpu_features.c"
#include "libdeflate/lib/deflate_compress.c"
#undef BITBUF_NBITS
#include "libdeflate/lib/deflate_decompress.c"
#include "libdeflate/lib/adler32.c"
#include "libdeflate/lib/zlib_compress.c"
#include "libdeflate/lib/zlib_decompress.c"

#include "openexr-c.h"

#include "OpenEXRCore/attributes.c"
#include "OpenEXRCore/base.c"
#include "OpenEXRCore/bytes.c"
#include "OpenEXRCore/channel_list.c"
#include "OpenEXRCore/chunk.c"
#include "OpenEXRCore/coding.c"
#include "OpenEXRCore/compression.c"
#include "OpenEXRCore/context.c"
#include "OpenEXRCore/debug.c"
#include "OpenEXRCore/decoding.c"
#include "OpenEXRCore/encoding.c"
#include "OpenEXRCore/float_vector.c"
#include "OpenEXRCore/internal_b44_table.c"
#include "OpenEXRCore/internal_b44.c"
#include "OpenEXRCore/internal_dwa.c"
#include "OpenEXRCore/internal_huf.c"
#include "OpenEXRCore/internal_piz.c"
#include "OpenEXRCore/internal_pxr24.c"
#include "OpenEXRCore/internal_rle.c"
#include "OpenEXRCore/internal_structs.c"
#include "OpenEXRCore/internal_zip.c"
#include "OpenEXRCore/memory.c"
#include "OpenEXRCore/opaque.c"
#include "OpenEXRCore/pack.c"
#include "OpenEXRCore/parse_header.c"
#include "OpenEXRCore/part_attr.c"
#include "OpenEXRCore/part.c"
#include "OpenEXRCore/preview.c"
#include "OpenEXRCore/std_attr.c"
#include "OpenEXRCore/string_vector.c"
#include "OpenEXRCore/string.c"
#include "OpenEXRCore/unpack.c"
#include "OpenEXRCore/validation.c"
#include "OpenEXRCore/write_header.c"
