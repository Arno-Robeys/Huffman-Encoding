#ifdef TEST_BUILD

#include "catch.hpp"
#include "data/frequency-table.h"


TEST_CASE("Frequency Table")
{
    data::FrequencyTable<char> table;
    table.increment('a');
    table.increment('a');
    table.increment('b');
    table.increment('c');

    REQUIRE(table['a'] == 2);
    REQUIRE(table['b'] == 1);
    REQUIRE(table['c'] == 1);
    REQUIRE(table['d'] == 0);

    std::vector<int> values = {1 , 2, 1 , 1 , 2, 3, 3, 1};
    data::FrequencyTable<int> table2 = data::count_frequencies(values);
    REQUIRE(table2[1] == 4);
    REQUIRE(table2[2] == 2);
    REQUIRE(table2[3] == 2);
    REQUIRE(table2[4] == 0);


}
/*
TEST_CASE("Failing test")
{
    //REQUIRE(1 == 2);
}*/

#endif
