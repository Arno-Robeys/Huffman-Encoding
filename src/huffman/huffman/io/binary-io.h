#ifndef BINARY_IO_H
#define BINARY_IO_H

#include "util.h"
#include "streams.h"

namespace io {

	u64 read_bits(unsigned nbits, io::InputStream& input);

	void write_bits(u64 value, unsigned nbits, io::OutputStream& output);
}

#endif