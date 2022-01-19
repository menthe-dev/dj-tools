#include "encode.h"

void encode_u8(u8* buffer, u8 value)
{
	buffer[0] = value;
}

void encode_u16(u8* buffer, u16 value, endianness endian)
{
	if (endian == ENDIAN_LITTLE) {
		encode_u16_le(buffer, value);
	}
	if (endian == ENDIAN_BIG) {
		encode_u16_be(buffer, value);
	}
}

void encode_u16_be(u8* buffer, u16 value)
{
	buffer[0] = (value >> 8) & 0xFF;
	buffer[1] = value & 0xFF;
}

void encode_u16_le(u8* buffer, u16 value)
{
	buffer[0] = value & 0xFF;
	buffer[1] = (value >> 8) & 0xFF;
}

void encode_u32(u8* buffer, u32 value, endianness endian)
{
	if (endian == ENDIAN_LITTLE) {
		encode_u32_le(buffer, value);
	}
	if (endian == ENDIAN_BIG) {
		encode_u32_be(buffer, value);
	}
}

void encode_u32_be(u8* buffer, u32 value)
{
	buffer[0] = (value >> 24) & 0xFF;
	buffer[1] = (value >> 16) & 0xFF;
	buffer[2] = (value >> 8) & 0xFF;
	buffer[3] = value & 0xFF;
}

void encode_u32_le(u8* buffer, u32 value)
{
	buffer[0] = value & 0xFF;
	buffer[1] = (value >> 8) & 0xFF;
	buffer[2] = (value >> 16) & 0xFF;
	buffer[3] = (value >> 24) & 0xFF;
}

void encode_u64(u8* buffer, u64 value, endianness endian)
{
	if (endian == ENDIAN_LITTLE) {
		encode_u64_le(buffer, value);
	}
	if (endian == ENDIAN_BIG) {
		encode_u64_be(buffer, value);
	}
}

void encode_u64_be(u8* buffer, u64 value)
{
	buffer[0] = (value >> 56) & 0xFF;
	buffer[1] = (value >> 48) & 0xFF;
	buffer[2] = (value >> 40) & 0xFF;
	buffer[3] = (value >> 32) & 0xFF;
	buffer[4] = (value >> 24) & 0xFF;
	buffer[5] = (value >> 16) & 0xFF;
	buffer[6] = (value >> 8) & 0xFF;
	buffer[7] = value & 0xFF;
}

void encode_u64_le(u8* buffer, u64 value)
{
	buffer[0] = value & 0xFF;
	buffer[1] = (value >> 8) & 0xFF;
	buffer[2] = (value >> 16) & 0xFF;
	buffer[3] = (value >> 24) & 0xFF;
	buffer[4] = (value >> 32) & 0xFF;
	buffer[5] = (value >> 40) & 0xFF;
	buffer[6] = (value >> 48) & 0xFF;
	buffer[7] = (value >> 56) & 0xFF;
}
