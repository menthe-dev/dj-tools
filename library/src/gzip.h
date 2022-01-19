#pragma once

#include "primitives.h"

u8* gzip_decompress(u8* in, u64 in_size, u64* decompressed_size);
u8* gzip_compress(u8* in, u64 in_size, u64* compressed_size);
