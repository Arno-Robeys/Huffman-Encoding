#ifdef TEST_BUILD

#include "catch.hpp"
#include "util.h"


TEST_CASE("Bits needed")
{
   REQUIRE(bits_needed(2) == 1);
   REQUIRE(bits_needed(3) == 2);
   REQUIRE(bits_needed(4) == 2);
   REQUIRE(bits_needed(5) == 3);
   REQUIRE(bits_needed(6) == 3);
   REQUIRE(bits_needed(7) == 3);
   REQUIRE(bits_needed(8) == 3);
   REQUIRE(bits_needed(9) == 4);
   REQUIRE(bits_needed(128) == 7);
   REQUIRE(bits_needed(129) == 8);
   REQUIRE(bits_needed(1'000'000) == 20);
}

#endif
