//
// Copyright 2023 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
/// \file OpenEXRCoreUnity.h

#include "src/lib/OpenEXRCore/openexr_config.h"

#include "openexr-c.h"

#include "src/lib/OpenEXRCore/attributes.c"
#include "src/lib/OpenEXRCore/base.c"
#include "src/lib/OpenEXRCore/bytes.c"
#include "src/lib/OpenEXRCore/channel_list.c"
#include "src/lib/OpenEXRCore/chunk.c"
#include "src/lib/OpenEXRCore/coding.c"
#include "src/lib/OpenEXRCore/compression.c"
#include "src/lib/OpenEXRCore/context.c"
#include "src/lib/OpenEXRCore/debug.c"
#include "src/lib/OpenEXRCore/decoding.c"
#include "src/lib/OpenEXRCore/encoding.c"
#include "src/lib/OpenEXRCore/float_vector.c"
#include "src/lib/OpenEXRCore/internal_b44_table.c"
#include "src/lib/OpenEXRCore/internal_b44_table_init.c"
#include "src/lib/OpenEXRCore/internal_b44.c"
#include "src/lib/OpenEXRCore/internal_dwa.c"
#include "src/lib/OpenEXRCore/internal_dwa_table.c"
#include "src/lib/OpenEXRCore/internal_dwa_table_init.c"
#include "src/lib/OpenEXRCore/internal_huf.c"
#include "src/lib/OpenEXRCore/internal_piz.c"
#include "src/lib/OpenEXRCore/internal_pxr24.c"
#include "src/lib/OpenEXRCore/internal_rle.c"
#include "src/lib/OpenEXRCore/internal_structs.c"
#include "src/lib/OpenEXRCore/internal_zip.c"
#include "src/lib/OpenEXRCore/memory.c"
#include "src/lib/OpenEXRCore/opaque.c"
#include "src/lib/OpenEXRCore/pack.c"
#include "src/lib/OpenEXRCore/parse_header.c"
#include "src/lib/OpenEXRCore/part_attr.c"
#include "src/lib/OpenEXRCore/part.c"
#include "src/lib/OpenEXRCore/preview.c"
#include "src/lib/OpenEXRCore/std_attr.c"
#include "src/lib/OpenEXRCore/string_vector.c"
#include "src/lib/OpenEXRCore/string.c"
#include "src/lib/OpenEXRCore/unpack.c"
#include "src/lib/OpenEXRCore/validation.c"
#include "src/lib/OpenEXRCore/write_header.c"
