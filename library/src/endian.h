#pragma once

#include "primitives.h"

enum endianness_e {
	ENDIAN_BIG,
	ENDIAN_LITTLE,
};
typedef enum endianness_e endianness;

#ifdef _MSC_VER

#include <stdlib.h>
#include <Windows.h>

#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
# define ENDIANNESS ENDIAN_LITTLE
# define le_to_host_32(VAL) VAL
# define be_to_host_32(VAL) _byteswap_ulong(VAL)
# define le_to_host_16(VAL) VAL
# define be_to_host_16(VAL) _byteswap_ushort(VAL)
# define le_to_host_64(VAL) VAL
# define be_to_host_64(VAL) _byteswap_uint64(VAL)
#else
# define ENDIANNESS ENDIAN_BIG
# define le_to_host_32(VAL) _byteswap_ulong(VAL)
# define be_to_host_32(VAL) VAL
# define le_to_host_16(VAL) _byteswap_ushort(VAL)
# define be_to_host_16(VAL) VAL
# define le_to_host_64(VAL) _byteswap_uint64(VAL)
# define be_to_host_64(VAL) VAL
#endif

#endif

// Todo: Add support for other platforms like GCC