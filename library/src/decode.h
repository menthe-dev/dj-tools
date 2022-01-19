#pragma once

#include "primitives.h"
#include "endian.h"

u16 decode_u16(const u8* data, endianness endian);
u16 decode_u16_stream(stream* stream, endianness endian);
u16 decode_u16_le(const u8* data);
u16 decode_u16_be(const u8* data);

u32 decode_u32(const u8* data, endianness endian);
u32 decode_u32_stream(stream* stream, endianness endian);
u32 decode_u32_le(const u8* data);
u32 decode_u32_be(const u8* data);

u64 decode_u64(const u8* data, endianness endian);
u64 decode_u64_stream(stream* stream, endianness endian);
u64 decode_u64_le(const u8* data);
u64 decode_u64_be(const u8* data);

f32 decode_f32(const u8* data, endianness endian);
f32 decode_f32_stream(stream* stream, endianness endian);
f32 decode_f32_le(const u8* data);
f32 decode_f32_be(const u8* data);

char* decode_string_stream(stream* stream, u64 max_length, u64* length);
char* decode_string(const u8* data, u64 max_length, u64* length);
char* decode_string_null_terminated(const u8* data, u64* length);
