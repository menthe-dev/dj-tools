#include "gtest/gtest.h"

extern "C" {
#include "../decode.h"
#include "../primitives.h"
#include "../gzip.h"
#include "../fsar.h"
}

TEST(FSAR, Parse)
{
	char path[255];
	getcwd(path, 255);

	auto test = "Hello World";
	auto test_len = strlen(test);

	u64 compressed_length = 0;
	auto compressed = gzip_compress((u8*)test, test_len, &compressed_length);


	// Check read position of data again.
	// It seems to be reading part of the path instead...


	// Nvm data is just malformed, correct num to 2.

	auto fsar = fsar_open_file("data/demo.far");
	EXPECT_EQ(fsar->entry_count, 2);
	EXPECT_STREQ(reinterpret_cast<const char*>(fsar->entries[0].path), "art\\test\\uncompressed.txt");
	EXPECT_STREQ(reinterpret_cast<const char*>(fsar->entries[1].path), "art\\test\\compressed.txt");
	fsar_close(fsar);
}

TEST(FSAR, Export)
{
	char path[255];
	getcwd(path, 255);
	printf("Current working dir: %s\n", path);

	auto test = "Hello World";
	auto test_len = strlen(test);

	u64 compressed_length = 0;
	auto compressed = gzip_compress((u8*)test, test_len, &compressed_length);
	printf("%a", compressed);

	auto fsar = fsar_open_file("data/demo.far");

	u64 output_length = 0;
	auto output = fsar_build(fsar, &output_length);

	EXPECT_EQ(output_length, 0x27F);
	// Todo: more checks.

	fsar_close(fsar);
}