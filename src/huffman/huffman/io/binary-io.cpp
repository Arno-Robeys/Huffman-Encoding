#include "binary-io.h"

namespace io {

    u64 read_bits(unsigned nbits, InputStream& input) {
		
		u64 result = 0;
		for (unsigned i = 0; i < nbits; i++) {
				result <<= 1;
				result |= input.end_reached() ? 0 : input.read();
		}
		return result;

    }

	void write_bits(u64 value, unsigned nbits, io::OutputStream& output) {
		
		for (int i = nbits - 1; i >= 0; i--) {
			output.write((value >> i) & 1);
		}
	}
	
}