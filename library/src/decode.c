#include "decode.h"
#include "endian.h"

u16 decode_u16(const u8* data, endianness endian) {
	if (endian == ENDIAN_LITTLE) {
		return decode_u16_le(data);
	}
	if (endian == ENDIAN_BIG) {
		return decode_u16_be(data);
	}

	return 0;
}

u16 decode_u16_stream(stream* stream, endianness endian) {
	u8 data[2];
	fread(&data, 1, 2, stream);
	return decode_u16(data, endian);
}

u16 decode_u16_le(const u8* data)
{
	u16 num;
	memcpy(&num, data, sizeof(num));
	return le_to_host_16(num);
}

u16 decode_u16_be(const u8* data)
{
	u16 num;
	memcpy(&num, data, sizeof(num));
	return be_to_host_16(num);
}

u32 decode_u32(const u8* data, endianness endian) {
	if (endian == ENDIAN_LITTLE) {
		return decode_u32_le(data);
	}
	if (endian == ENDIAN_BIG) {
		return decode_u32_be(data);
	}

	return 0;
}

u32 decode_u32_stream(stream* stream, endianness endian) {
	u8 data[4];
	fread(&data, 1, 4, stream);
	return decode_u32(data, endian);
}

u32 decode_u32_le(const u8* data)
{
	u32 num;
	memcpy(&num, data, sizeof(num));
	return le_to_host_32(num);
}

u32 decode_u32_be(const u8* data)
{
	u32 num;
	memcpy(&num, data, sizeof(num));
	return be_to_host_32(num);
}

u64 decode_u64(const u8* data, endianness endian) {
	if (endian == ENDIAN_LITTLE) {
		return decode_u64_le(data);
	}
	if (endian == ENDIAN_BIG) {
		return decode_u64_be(data);
	}

	return 0;
}

u64 decode_u64_stream(stream* stream, endianness endian) {
	u8 data[8];
	fread(&data, 1, 8, stream);
	return decode_u64(data, endian);
}

u64 decode_u64_le(const u8* data)
{
	u64 num;
	memcpy(&num, data, sizeof(num));
	return le_to_host_64(num);
}

u64 decode_u64_be(const u8* data)
{
	u64 num;
	memcpy(&num, data, sizeof(num));
	return be_to_host_64(num);
}

f32 decode_f32(const u8* data, endianness endian) {
	if (endian == ENDIAN_LITTLE) {
		return decode_f32_le(data);
	}
	if (endian == ENDIAN_BIG) {
		return decode_f32_be(data);
	}

	return 0;
}

f32 decode_f32_stream(stream* stream, endianness endian) {
	u8 data[4];
	fread(&data, 1, 4, stream);
	return decode_f32(data, endian);
}

f32 decode_f32_le(const u8* data)
{
	f32 num;
	memcpy(&num, data, sizeof(num));
	return le_to_host_32(num);
}

f32 decode_f32_be(const u8* data) {
	f32 num;
	memcpy(&num, data, sizeof(num));
	return be_to_host_32(num);
}

char* decode_string_stream(stream* stream, u64 max_length, u64* length) {
	u8* buffer = (u8*)malloc(max_length);
	fread(buffer, 1, max_length, stream);
	*length = strlen((char*)buffer);
	return (char*)buffer;
}

char* decode_string(const u8* data, u64 max_length, u64* length)
{
	*length = min(strlen(data), max_length);
	char* result = calloc(*length + 1, sizeof(char));
	memcpy(result, data, *length);


	result[*length] = '\0';
	return result;
}

char* decode_string_null_terminated(const u8* data, u64* length) {
	*length = strlen(data);
	char* result = calloc(*length + 1, sizeof(char));
	memcpy(result, data, *length);

	result[*length] = '\0';
	return result;
}
