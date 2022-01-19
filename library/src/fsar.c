#include "fsar.h"
#include "decode.h"
#include "gzip.h"
#include "encode.h"
#include <stdio.h>

fsar* fsar_parse(stream* stream)
{
	u64 read;

	// Todo: Move to decode.h as decode_string_stream
	u8* magic_buffer = (u8*)malloc(4);
	fread(magic_buffer, 1, 4, stream);
	u8* magic = (u8*)decode_string(magic_buffer, 4, &read);
	free(magic_buffer);

	endianness endianness;

	// Determine endianness of the file.
	if (strcmp(FSAR_BE_MAGIC, magic) == 0)
	{
		free(magic);
		endianness = ENDIAN_BIG;
	}
	else if (strcmp(FSAR_LE_MAGIC, magic) == 0)
	{
		free(magic);
		endianness = ENDIAN_LITTLE;
	}
	else
	{
		free(magic);
		return NULL;
	}

	// Read data from the stream.
	u32 version = decode_u32_stream(stream, endianness);
	u32 _ = decode_u32_stream(stream, endianness);
	u32 entry_count = decode_u32_stream(stream, endianness);

	// Create FSAR memory structure.
	fsar* fsar = malloc(sizeof(fsar));
	fsar->endianness = endianness;
	fsar->stream = stream;
	fsar->version = version;

	// Allocate entry table.
	fsar->entry_count = entry_count;
	fsar->entries = malloc(sizeof(fsar_entry) * entry_count);

	u32 start_of_data_section = 0x20 + FSAR_ENTRY_SIZE * entry_count;

	// Read entries from stream.
	for (int i = 0; i < entry_count; ++i)
	{
		// Seek to start of FSAR entry.
		fseek(stream, 0x20 + FSAR_ENTRY_SIZE * i, SEEK_SET);

		// Get pointer to entry.
		fsar_entry* entry = &fsar->entries[i];

		// Decode path from stream.
		u8* path = decode_string_stream(stream, FSAR_MAX_PATH_LENGTH, &read);
		memcpy(&entry->path, path, FSAR_MAX_PATH_LENGTH);
		free(path);

		// Print out current read position;
		u64 current_position = ftell(stream);
		printf("%llu\n", current_position);

		// Get entry data sizes.
		u64 uncompressed_size = decode_u64_stream(stream, endianness);
		u64 compressed_size = decode_u64_stream(stream, endianness);

		// Store information.
		entry->offset = decode_u64_stream(stream, endianness);
		entry->size = uncompressed_size;

		// Attempt to read the entry data.
		entry->data = malloc(uncompressed_size);
		fseek(stream, start_of_data_section + entry->offset, SEEK_SET);
		if (compressed_size == 0 && uncompressed_size > 0) {
			// Read decompressed data from stream.
			fread(entry->data, uncompressed_size, 1, stream);
		} else if (compressed_size > 0 && uncompressed_size > 0) {
			// Read compressed data from stream.
			u8* compressed_data = malloc(compressed_size);
			fread(compressed_data, compressed_size, 1, stream);

			u64 real_uncompressed_size = uncompressed_size;

			// Decompress data.
			u8* decompressed = gzip_decompress(compressed_data, compressed_size, &real_uncompressed_size);
			memcpy(entry->data, decompressed, real_uncompressed_size);
			free(decompressed);

//			free(compressed_data);
			entry->size = real_uncompressed_size;
		}
	}

	return fsar;
}

fsar* fsar_open_file(const char* path)
{
	// Open the stream.
	stream* stream = fopen(path, "r+");

	// Parse the file.
	fsar* fsar = fsar_parse(stream);
	return fsar;
}

fsar* fsar_create(u32 version, endianness endianness)
{
	// Create FSAR memory structure.
	fsar* fsar = malloc(sizeof(fsar));
	fsar->version = version;
	fsar->endianness = endianness;
	fsar->entry_count = 0;

	return fsar;
}

u8* fsar_read(fsar* fsar, fsar_entry* entry, u64* size)
{
	if (!fsar || !entry)
		return NULL;

	if (entry->size == 0)
		return NULL;

	if (!entry->data)
		return NULL;

	*size = entry->size;
	u8* data = malloc(entry->size);
	memcpy(data, entry->data, entry->size);
	return data;
}

u8* fsar_read_from_path(fsar* fsar, const char* path, u64* size)
{
	if (!fsar || !path)
	{
		return NULL;
	}

	if (fsar->entry_count == 0 || fsar->entries == NULL)
	{
		return NULL;
	}

	for (u32 i = 0; i < fsar->entry_count; i++)
	{
		if (strcmp((const char*)(&fsar->entries[i].path), path) == 0)
		{
			return fsar_read(fsar, &fsar->entries[i], size);
		}
	}

	return NULL;
}

u8* fsar_read_from_index(fsar* fsar, u32 index, u64* size)
{
	if (!fsar)
	{
		return NULL;
	}

	if (fsar->entry_count == 0 || fsar->entries == NULL)
	{
		return NULL;
	}

	if (index >= fsar->entry_count)
	{
		return NULL;
	}

	return fsar_read(fsar, &fsar->entries[index], size);
}

u8 fsar_exists(fsar* fsar, const char* path)
{
	if (!fsar || !path)
	{
		return 0;
	}

	if (fsar->entry_count == 0 || fsar->entries == NULL)
	{
		return 0;
	}

	for (u32 i = 0; i < fsar->entry_count; i++)
	{
		if (strncmp((const char*)(&fsar->entries[i].path), path, FSAR_MAX_PATH_LENGTH) == 0)
		{
			return 1;
		}
	}

	return 0;
}

u8 fsar_add(fsar* fsar, const char* path, u8* data, u64 size, u8 compress)
{
	if (!fsar || !path || !data)
	{
		return 0;
	}

	if (fsar_exists(fsar, path))
	{
		return 0;
	}

	if (fsar->entry_count == 0)
	{
		fsar->entries = malloc(sizeof(fsar_entry));
	}
	else
	{
		fsar->entries = realloc(fsar->entries, sizeof(fsar_entry) * (fsar->entry_count + 1));
	}

	if (fsar->entries == NULL)
	{
		return 0;
	}

	fsar_entry* entry = &fsar->entries[fsar->entry_count];
	fsar->entry_count++;

	u8* copied_data = NULL;

	u32 path_length = strlen(path);
	if (path_length > FSAR_MAX_PATH_LENGTH)
	{
		path_length = FSAR_MAX_PATH_LENGTH;
	}

	memcpy(&entry->path, path, path_length);
	if (path_length < FSAR_MAX_PATH_LENGTH)
	{
		memset(&entry->path[path_length], 0, FSAR_MAX_PATH_LENGTH - path_length);
	}

	entry->size = size;
	entry->should_compress = compress;

	copied_data = malloc(size);
	if (copied_data == NULL)
	{
		return 0;
	}

	memcpy(copied_data, data, size);

	fsar->entries[fsar->entry_count].data = copied_data;

	return 1;
}

u8 fsar_replace(fsar* fsar, const char* path, u8* data, u64 size, u8 compress)
{
	// Find entry with path
	for (u32 i = 0; i < fsar->entry_count; i++) {
		if (strcmp((const char*)fsar->entries[i].path, path) == 0) {
			// Todo
		}
	}

	return 0;
}

u8* fsar_delete(fsar* fsar, const char* path)
{
	// Todo
	return NULL;
}

u8* fsar_delete_all(fsar* fsar)
{
	// Todo
	return NULL;
}

struct fsar_output_entry {
	u64 length;
	u8* data;
};

u8* fsar_build(fsar* fsar, u64* size)
{
	// Steps:
	//  - Calculate header size (fixed 0x20)
	//  - Calculate size of entry table ( entry_count * 0x120 )
	//  - Create final data for each entry
	//  - Calculate size of data struct ( add each final size)

	//  - Build header
	//  - Build entry table
	// 	- Build data

	u64 start_of_data = 0x20 + fsar->entry_count * FSAR_ENTRY_SIZE;
	struct fsar_output_entry* output_files = calloc(fsar->entry_count, sizeof(struct fsar_output_entry));

	u64 total_size = 0x20 + fsar->entry_count * FSAR_ENTRY_SIZE;

	// Create final data for each entry.
	for (u32 i = 0; i < fsar->entry_count; i++) {
		// Shorthands.
		fsar_entry entry = fsar->entries[i];
		struct fsar_output_entry out = output_files[i];

		// Compression switch.
		if (entry.should_compress) {
			// Write the compressed data to the output.
			out.data = gzip_compress(entry.data, entry.size, &out.length);
		} else {
			// Copy the uncompressed data to the output.
			out.length = entry.size;
			out.data = malloc(out.length);
			memcpy(out.data, entry.data, out.length);
		}

		total_size += out.length;
	}

	// Allocate stream.
	u8* final = malloc(total_size);
	u64 current_offset = 0;

	// Populate header.

	// Write magic + indicate endianness.
	if (fsar->endianness == ENDIAN_BIG) {
		memcpy(final + current_offset, FSAR_BE_MAGIC, sizeof(FSAR_BE_MAGIC));
	} else {
		memcpy(final + current_offset, FSAR_LE_MAGIC, sizeof(FSAR_LE_MAGIC));
	}
	current_offset += 4;

	// Write version.
	u8 version_buffer[sizeof(u32)] = {0};
	encode_u32(version_buffer, fsar->version, fsar->endianness);
	memcpy(final + current_offset, version_buffer, sizeof(u32));
	current_offset += sizeof(u32);

	// Write start of data section.
	u8 start_of_data_buffer[sizeof(u32)] = {0};
	encode_u32(start_of_data_buffer, start_of_data, fsar->endianness);
	memcpy(final + current_offset, start_of_data_buffer, sizeof(u32));
	current_offset += sizeof(u32);

	// Write entry count.
	u8 entry_count_buffer[sizeof(u32)] = {0};
	encode_u32(entry_count_buffer, fsar->entry_count, fsar->endianness);
	memcpy(final + current_offset, entry_count_buffer, sizeof(u32));
	current_offset += sizeof(u32);

	// Filler.
	memset(final + current_offset, 'x', 0x10);
	current_offset += 0x10;

	u64 file_offset = 0;

	// Table of Contents entries
	for (u32 i = 0; i < fsar->entry_count; i++) {
		u64 offset = 0x20 + i * FSAR_ENTRY_SIZE;

		// Shorthands.
		fsar_entry* entry = &fsar->entries[i];
		struct fsar_output_entry *out = &output_files[i];

		// Write path.
		memcpy(final + offset, entry->path, FSAR_MAX_PATH_LENGTH);
		offset += FSAR_MAX_PATH_LENGTH;

		// Write compressed size.
		u8 compressed_size_buffer[sizeof(u64)] = {0};
		if (entry->should_compress) {
			encode_u64(compressed_size_buffer, out->length, fsar->endianness);
		}
		memcpy(final + offset, compressed_size_buffer, sizeof(u64));
		offset += sizeof(u64);

		// Write size.
		u8 size_buffer[sizeof(u64)] = {0};
		encode_u64(size_buffer, entry->size, fsar->endianness);
		memcpy(out->data + current_offset, size_buffer, sizeof(u64));
		current_offset += sizeof(u64);

		u8 file_offset_buffer[sizeof(u64)] = {0};
		encode_u64(file_offset_buffer, file_offset, fsar->endianness);
		memcpy(final + offset, file_offset_buffer, sizeof(u64));
		offset += sizeof(u64);

		// Write compression flag.
		u8 compression_flag_buffer[sizeof(u32)] = {0};
		encode_u32(compression_flag_buffer, entry->should_compress, fsar->endianness);
		memcpy(out->data + current_offset, compression_flag_buffer, sizeof(u32));
		current_offset += sizeof(u8);

		current_offset += FSAR_ENTRY_SIZE;
		file_offset += out->length;
	}

	// Write data.
	for (u32 i = 0; i < fsar->entry_count; i++) {
		// Shorthands.
		fsar_entry *entry = &fsar->entries[i];
		struct fsar_output_entry *out = &output_files[i];

		memcpy(final + current_offset, out->data, out->length);
		current_offset += out->length;
	}

	*size = total_size;
	return final;
}