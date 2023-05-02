#include "binary-io.h"

namespace io {

    u64 read_bits(unsigned nbits, InputStream& input) {
		
		u64 result = 0;
		for (unsigned i = 0; i < nbits; i++) {
			result <<= 1;
			result |= input.read();
		}
		return result;

		/*if (nbits == 1) {
			return input.read();
		} else {
			if (input.end_reached() && nbits > 0) {
				return 0;
			}else {
				auto x = input.read() << (nbits - 1);
				auto y = read_bits(nbits - 1, input);
				return x | y;
			}
		}*/

    }

	void write_bits(u64 value, unsigned nbits, io::OutputStream& output) {
		
		for (int i = nbits - 1; i >= 0; i--) {
			output.write((value >> i) & 1);
		}
	}
	
}