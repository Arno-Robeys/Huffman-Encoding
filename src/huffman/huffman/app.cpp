#ifndef TEST_BUILD

/*
*   This file is only active in non-test builds (i.e., builds where TEST_BUILD is not defined).
*/

#include <iostream>
#include "util.h"


/*
* This is the main entry point of your application.
* Note that it will only work in non-test builds.
*/
int main()
{
    bits_needed(129);
#ifdef NDEBUG
    std::cout << "You are running the release build" << std::endl;
#else
    std::cout << "You are running the debug build" << std::endl;
#endif
}

#endif