#include "catch.hpp"
#include "encoding/huffman\tree-encoding.h"
#include <iostream>

TEST_CASE("Tree Encoding") {

	int nr = 4506811;

	io::MemoryBuffer<6> buffer;
	auto input = buffer.source()->create_input_stream();
	auto output = buffer.destination()->create_output_stream();

	io::write_bits(nr, 23, *output);

	auto root = encoding::huffman::decode_tree(6, *input);
	encoding::huffman::encode_tree(*root, 6, *output);
	auto result = io::read_bits(23, *input);

	REQUIRE(result == nr);

}