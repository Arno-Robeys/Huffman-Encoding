#include <cstdint>
#ifndef UTIL_H
#define UTIL_H
    typedef uint64_t u64;
    typedef uint64_t Datum;

     constexpr unsigned bits_needed(u64 domain_size) {
        unsigned bits = 0;
        while (domain_size > 0) {
			domain_size >> 1;
			++bits;
		}
		return bits;
    }
#endif
