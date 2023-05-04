#ifdef TEST_BUILD

#include "catch.hpp"
#include "encoding/eof-encoding.h"
#include "encoding/bit-grouper.h"
#include "encoding/encoding-combiner.h"
#include <iostream>


TEST_CASE("Eof-Encoding ")
{
	io::MemoryBuffer<17> buffer;
	io::MemoryBuffer<18> buffer2;
	io::MemoryBuffer<17> buffer3;;

	buffer.data()->push_back(8);
	buffer.data()->push_back(4);
	buffer.data()->push_back(5);
	buffer.data()->push_back(16);

	const auto eofencoding = encoding::eof_encoding<17>();
	encode(buffer.source(), eofencoding, buffer2.destination());


	REQUIRE(buffer.data()->size() == buffer2.data()->size() - 1);
	REQUIRE(buffer2.data().get()->at(0) == 8);
	REQUIRE(buffer2.data().get()->at(1) == 4);
	REQUIRE(buffer2.data().get()->at(2) == 5);
	REQUIRE(buffer2.data().get()->at(3) == 16);
	//EOF Check
	REQUIRE(buffer2.data().get()->at(4) == 17);

	const auto eofdecoding = encoding::eof_encoding<17>();
	decode(buffer2.source(), eofdecoding, buffer3.destination());

	REQUIRE(buffer2.data()->size() == buffer3.data()->size()+1);
	REQUIRE(buffer3.data().get()->at(0) == 8);
	REQUIRE(buffer3.data().get()->at(1) == 4);
	REQUIRE(buffer3.data().get()->at(2) == 5);
	REQUIRE(buffer3.data().get()->at(3) == 16);
}

TEST_CASE("Bit Grouper Encoding") {
	io::MemoryBuffer<2> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<2> buffer3;

	const auto output = buffer.destination()->create_output_stream();
	io::write_bits(70, 8, *output);
	io::write_bits(69, 8, *output);

	REQUIRE(buffer.data()->size() == 16);

	const auto bitgrouper = encoding::bit_grouper<8>();
	encode(buffer.source(), bitgrouper, buffer2.destination());

	REQUIRE(buffer2.data()->begin()[0] == 'F');
	REQUIRE(buffer2.data()->back() == 'E');

	decode(buffer2.source(), bitgrouper, buffer3.destination());

	REQUIRE(buffer.data()->begin()[0] == buffer3.data()->begin()[0]);
	REQUIRE(buffer.data()->back() == buffer3.data()->back());


}

TEST_CASE("Encodding Combinar") {
	io::MemoryBuffer<2> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<2> buffer3;
	

	const auto bitgrouper = encoding::bit_grouper<8>();
	const auto eofencoding = encoding::eof_encoding<8>();

}

#endif
