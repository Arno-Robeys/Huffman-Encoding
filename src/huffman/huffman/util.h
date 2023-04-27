#include <cstdint>
#ifndef UTIL_H
#define UTIL_H
    typedef uint64_t u64;
    typedef uint64_t Datum;

     constexpr unsigned bits_needed(u64 domain_size) {
         u64 size = domain_size - 1;
         unsigned bits = 0;
         while (size > 0) {
             ++bits;
             size >>= 1;
         }
         return bits;
    }

     constexpr unsigned bytes_needed(u64 domain_size) {
		return (bits_needed(domain_size) + 7) / 8;
	 }

#endif
