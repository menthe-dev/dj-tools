#pragma once

#include "primitives.h"
#include "endian.h"

#define FSAR_ENTRY_SIZE 0x120

#define FSAR_MAX_PATH_LENGTH 0x100
#define FSAR_BE_MAGIC "FSAR"
#define FSAR_LE_MAGIC "RASF"

struct fsar_entry_size_s {
	/**
	 * The compressed size.
	 * 0 if the file isn't compressed.
	 */
	u64 compressed;

	/**
	 * The uncompressed size.
	 */
	 u64 uncompressed;

};
typedef struct fsar_entry_size_s fsar_entry_size;

struct fsar_entry_s {
	/**
	 * The stream where the data of the entry is stored.
	 * This can either be a file or a memory buffer.
	 *
	 * Null if the file hasn't been read yet.
	 */
	u8* data;

	/**
	 * The path of the entry.
	 */
	u8 path[FSAR_MAX_PATH_LENGTH];

	/**
	 * The size information.
	 */
	u64 size;

	/**
	 * The flag indicating its compression status.
	 */
	u8 should_compress;

	/**
	 * The offset of the entry.
	 */
	u64 offset;
};
typedef struct fsar_entry_s fsar_entry;

struct fsar_s {
	/**
	 * The stream where the data of the file is stored.
	 * This can either be a real file or a memory buffer.
	 */
	stream *stream;

	endianness endianness;
	u32 version;

	u32 entry_count;
	fsar_entry* entries;
};

typedef struct fsar_s fsar;

fsar* fsar_open_file(const char* path);
fsar* fsar_create(u32 version, endianness endianness);

u8* fsar_read_from_path(fsar* fsar, const char* path, u64* size);
u8* fsar_read_from_index(fsar* fsar, u32 index, u64* size);

u8 fsar_add(fsar* fsar, const char* path, u8* data, u64 size, u8 compress);
u8 fsar_replace(fsar* fsar, const char* path, u8* data, u64 size, u8 compress);
u8* fsar_delete(fsar* fsar, const char* path);
u8* fsar_delete_all(fsar* fsar);

u8* fsar_build(fsar* fsar, u64* size);
u8 fsar_write_to_file(fsar* fsar, const char* path);

u8 fsar_close(fsar* fsar);