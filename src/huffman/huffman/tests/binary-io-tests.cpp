#ifdef TEST_BUILD

#include "catch.hpp"
#include "io/binary-io.h"
#include <io/memory-buffer.h>

void check(std::uint64_t n, unsigned nbits)
{
    io::MemoryBuffer<2> buffer;
    auto input = buffer.source()->create_input_stream();
    auto output = buffer.destination()->create_output_stream();
    io::write_bits(n, nbits, *output);
    auto result = io::read_bits(nbits, *input);

    REQUIRE(n == result);
}


TEST_CASE("Binary IO")
{
    check(0b1, 1);
    check(0b10, 2); 
    check(0b11, 2);
    check(0b1001010, 7);
}

#endif
