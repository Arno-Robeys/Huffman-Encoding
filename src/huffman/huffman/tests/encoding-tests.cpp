#ifdef TEST_BUILD

#include "catch.hpp"
#include "encoding/eof-encoding.h"


TEST_CASE("Eof-Encoding ")
{
	io::MemoryBuffer<4> buffer;
	io::MemoryBuffer<5> buffer2;
	io::MemoryBuffer<4> buffer3;;

	buffer.data()->push_back(8);
	buffer.data()->push_back(4);
	buffer.data()->push_back(2);
	buffer.data()->push_back(16);

	const auto eofencoding = encoding::eof_encoding<4>();
	encode(buffer.source(), eofencoding, buffer2.destination());

	REQUIRE(buffer.data()->size() == buffer2.data()->size() - 1);

	const auto eofdecoding = encoding::eof_encoding<4>();
	decode(buffer2.source(), eofdecoding, buffer3.destination());
	
	REQUIRE(buffer2.data()->size() == buffer3.data()->size()+1);
	REQUIRE(buffer.data().get()->at(0) == 8);
	REQUIRE(buffer.data().get()->at(1) == 4);
	REQUIRE(buffer.data().get()->at(2) == 2);
	REQUIRE(buffer.data().get()->at(3) == 16);
}

#endif
