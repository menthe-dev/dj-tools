#pragma once

#include "endian.h"

void encode_u8(u8* buffer, u8 value);

void encode_u16(u8* buffer, u16 value, endianness endian);
void encode_u16_be(u8* buffer, u16 value);
void encode_u16_le(u8* buffer, u16 value);

void encode_u32(u8* buffer, u32 value, endianness endian);
void encode_u32_be(u8* buffer, u32 value);
void encode_u32_le(u8* buffer, u32 value);

void encode_u64(u8* buffer, u64 value, endianness endian);
void encode_u64_be(u8* buffer, u64 value);
void encode_u64_le(u8* buffer, u64 value);