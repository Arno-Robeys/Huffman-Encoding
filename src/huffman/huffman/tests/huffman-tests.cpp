#ifdef TEST_BUILD
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

	REQUIRE(std::prev(codes.end(), 3)->first == 1);
	REQUIRE(std::prev(codes.end(), 2)->first == 2);
	REQUIRE(std::prev(codes.end(), 1)->first == 3);
	REQUIRE(std::prev(codes.end(), 3)->second.size() == 1);
	REQUIRE(std::prev(codes.end(), 2)->second.size() == 2);
	REQUIRE(std::prev(codes.end(), 1)->second.size() == 2);


}

TEST_CASE("Huffman encode 2") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('T');
	buffer.data()->push_back('E');
	buffer.data()->push_back('S');
	buffer.data()->push_back('T');
	buffer.data()->push_back('H');
	buffer.data()->push_back('U');
	buffer.data()->push_back('F');
	buffer.data()->push_back('F');
	buffer.data()->push_back('M');
	buffer.data()->push_back('A');
	buffer.data()->push_back('N');
	buffer.data()->push_back('C');
	buffer.data()->push_back('O');
	buffer.data()->push_back('D');
	buffer.data()->push_back('I');
	buffer.data()->push_back('N');
	buffer.data()->push_back('G');

	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 2: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 3") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('T');
	buffer.data()->push_back('E');
	buffer.data()->push_back('S');
	buffer.data()->push_back('T');

	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 3: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 4") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 4: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 5") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 5: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 6") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 6: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 7") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');
	buffer.data()->push_back('B');


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 7: " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 8") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 8 (Integers only): " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 9") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');
	buffer.data()->push_back('B');
	buffer.data()->push_back('T');
	buffer.data()->push_back('E');
	buffer.data()->push_back('S');
	buffer.data()->push_back('T');
	buffer.data()->push_back('T');
	buffer.data()->push_back('Y');
	buffer.data()->push_back('Z');
	buffer.data()->push_back('U');
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());
	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 9 (Combi Char & Integer): " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman encode 10") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	buffer.data()->push_back('A');
	buffer.data()->push_back('B');
	buffer.data()->push_back('T');
	buffer.data()->push_back('E');
	buffer.data()->push_back('S');
	buffer.data()->push_back('T');
	buffer.data()->push_back('T');
	buffer.data()->push_back('Y');
	buffer.data()->push_back('Z');
	buffer.data()->push_back('U');
	io::write_bits(255, 8, *buffer.destination()->create_output_stream());


	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());
	std::cout << "Size of encode Huffman Encode 10 (Char + 8 bits integer): " << buffer2.data()->size() << " and Size before encode: " << buffer.data()->size() << std::endl;
	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	for (int i = 0; i < buffer.data()->size(); i++) {
		REQUIRE(buffer.data()->at(i) == buffer3.data()->at(i));
	}
}

TEST_CASE("Huffman Encode Helper Function") {
	auto huffmanHelper = encoding::huffman::helper_functie_encode("ABBBBBBTTESTTYZU");

	if (huffmanHelper) {
		std::cout << "The string is the same after decode (Only Huffman)" << std::endl;
	}
	else {
		std::cout << "The string is not the same after decode (Only Huffman)" << std::endl;
	}

	auto huffmanHelper2 = encoding::huffman::helper_functie_encode("ABABABABABABABABABABABABABABABABABAB");

	if (huffmanHelper2) {
		std::cout << "The string is the same after decode (Only Huffman)" << std::endl;
	}
	else {
		std::cout << "The string is not the same after decode (Only Huffman)" << std::endl;
	}

	auto huffmanHelper3 = encoding::huffman::helper_functie_encode("ABABABABABAB");

	if (huffmanHelper3) {
		std::cout << "The string is the same after decode (Only Huffman)" << std::endl;
	}
	else {
		std::cout << "The string is not the same after decode (Only Huffman)" << std::endl;
	}

}

TEST_CASE("Huffman encode") {
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;

	io::write_bits(4506811, 23, *buffer.destination()->create_output_stream());

	auto eof = encoding::eof_encoding<256>();
	auto huffman = encoding::huffman::huffman_encoding<257>();
	auto group = encoding::bit_grouper<8>();
	auto combiner = eof | huffman | group;

	encode(buffer.source(), combiner, buffer2.destination());

	REQUIRE(buffer.data()->size() > buffer2.data()->size());

	decode(buffer2.source(), combiner, buffer3.destination());

	REQUIRE(buffer.data()->size() == buffer3.data()->size());

	auto result = io::read_bits(23, *buffer3.source()->create_input_stream());
	REQUIRE(result == 4506811);

}

#endif