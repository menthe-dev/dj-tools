#include "gzip.h"
#include "libdeflate.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

u8* gzip_decompress(u8* in, u64 in_size, u64* decompressed_size)
{
	struct libdeflate_decompressor* decompressor = libdeflate_alloc_decompressor();
	if (!decompressor)
	{
		decompressed_size = 0;
		return NULL;
	}

	u64 out_size = *decompressed_size;
	u8* out = malloc(out_size);

	enum libdeflate_result result = libdeflate_zlib_decompress(decompressor, in, in_size, out, out_size, decompressed_size);
	if (result != LIBDEFLATE_SUCCESS)
	{
		free(out);
		out = NULL;
	}

	libdeflate_free_decompressor(decompressor);
	return out;
}

u8* gzip_compress(u8* in, u64 in_size, u64* compressed_size)
{
	struct libdeflate_compressor* compressor = libdeflate_alloc_compressor(12);
	if (!compressor)
	{
		compressed_size = 0;
		return NULL;
	}

	u64 buf_size = libdeflate_zlib_compress_bound(compressor, in_size);
	u8* data = malloc(buf_size);

	size_t real_size = libdeflate_zlib_compress(compressor, in, in_size, data, buf_size);
	*compressed_size = real_size;

	u8* out = malloc(*compressed_size);
	memcpy(out, data, *compressed_size);
	free(data);

	libdeflate_free_compressor(compressor);
	return out;
}