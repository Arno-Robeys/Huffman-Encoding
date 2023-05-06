#include "catch.hpp"
#include "encoding/huffman\tree-encoding.h"
#include <iostream>
#include <vector>
#include "encoding/huffman\huffman-encoding.h"
#include "encoding/eof-encoding.h"
#include "encoding/bit-grouper.h"
#include "encoding/encoding-combiner.h"

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

TEST_CASE("Buildtree & Buildcodes") {
	auto freqtable = data::FrequencyTable<Datum>();
	freqtable.increment(1);
	freqtable.increment(1);
	freqtable.increment(2);
	freqtable.increment(3);

	auto root = encoding::huffman::build_tree(freqtable);

	auto codes = encoding::huffman::build_codes(root);

	/*for (auto const& pair : codes) {
		std::string s = "";
		for (auto const& bit : pair.second) {
			s.append(std::to_string(bit));
		}
		
		std::cout << pair.first << " : " << s << std::endl;
	}*/

	REQUIRE(std::prev(codes.end(), 3)->first == 1);
	REQUIRE(std::prev(codes.end(), 2)->first == 2);
	REQUIRE(std::prev(codes.end(), 1)->first == 3);
	REQUIRE(std::prev(codes.end(), 3)->second.size() == 1);
	REQUIRE(std::prev(codes.end(), 2)->second.size() == 2);
	REQUIRE(std::prev(codes.end(), 1)->second.size() == 2);


}

TEST_CASE("Huffman encode") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());

}