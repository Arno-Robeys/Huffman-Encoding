#include "binary-io.h"
#include <cassert>
#include <limits>

namespace io {

    u64 read_bits(u64 nbits, InputStream& input) {
		
		u64 result = 0;
		for (u64 i = 0; i < nbits; i++) {
				result <<= 1;
				result |= input.end_reached() ? 0 : input.read();
		}
		return result;

    }

	void write_bits(u64 value, u64 nbits, io::OutputStream& output) {
		for (u64 i = nbits; i > 0; --i) {
			output.write((value >> (i - 1)) & 1);
		}
	}

	
}