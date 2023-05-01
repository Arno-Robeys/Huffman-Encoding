#include "binary-io.h"

namespace io {

    u64 read_bits(unsigned nbits, InputStream& input) {
        u64 result = 0;
		for (unsigned i = 0; i < nbits; i++) {
			result <<= 1;
			result |= input.read();
		}
		return result;


		/* Werkt ook maar enkel met write_bits van hieronder in commentaar
		for (unsigned i = 0; i < nbits; i++) {
			result |= u64(input.read()) << i;
		}
		return result;*/
    }

	void write_bits(u64 value, unsigned nbits, io::OutputStream& output) {
		
		for (int i = nbits - 1; i >= 0; i--) {
			output.write((value >> i) & 1);
		}
		/* Werkt ook maar enkel met read_bits van hierboven in commentaar
		for (unsigned i = 0; i < nbits; i++) {
			output.write(value & 1);
			value >>= 1;
		}*/
	}
	
}