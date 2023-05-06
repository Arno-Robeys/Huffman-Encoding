#include "catch.hpp"
#include "encoding/huffman\tree-encoding.h"
#include <iostream>
#include <vector>
#include "encoding/huffman\huffman-encoding.h"

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

TEST_CASE("Copy to vector") {

	io::MemoryBuffer<6> buffer;
	auto input = buffer.source()->create_input_stream();
	auto output = buffer.destination()->create_output_stream();
	io::write_bits(4506811, 23, *output);

	std::vector<Datum> vec = copy_to_vector<Datum>(*input);
	REQUIRE(vec.size() == 23);
	for (int i = 0; i < 23; i++) {
		REQUIRE(vec[i] == buffer.data()->at(i));
	}

}