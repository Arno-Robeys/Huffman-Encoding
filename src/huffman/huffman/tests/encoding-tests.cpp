#ifdef TEST_BUILD

#include "catch.hpp"
#include "encoding/eof-encoding.h"
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

	//Print the encoded buffer
	for (auto i = 0; i < buffer2.data()->size(); i++)
	{
		std::cout << "Buffer plaats " << i << ": " << buffer2.data().get()->at(i) << std::endl;
	}


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

#endif
