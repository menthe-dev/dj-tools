#include "gtest/gtest.h"

extern "C" {
#include "../decode.h"
#include "../primitives.h"
}

auto str = (const u8*)"Hello World\0\0";

TEST(Decoder, StringWithNullPadding)
{
	u64 length;
	char* decoded = decode_string_null_terminated(str, &length);

	EXPECT_EQ(length, 11);
	EXPECT_STREQ(decoded, "Hello World");
}

TEST(Decoder, MaxStringLength)
{
	u64 length;
	char* decoded = decode_string(str, 5, &length);

	EXPECT_EQ(length, 5);
	EXPECT_STREQ(decoded, "Hello");
}