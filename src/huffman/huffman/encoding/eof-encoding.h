#ifndef EOF_ENCODING_H
#define EOF_ENCODING_H
#include "encoding.h"
#include "io/binary-io.h"
#include "util.h"

namespace encoding {

	std::shared_ptr<EncodingImplementation> create_eof_implementation(u64 domain_size);

	template<u64 N>
	Encoding<N, N+1> eof_encoding() {
		return Encoding<N, N+1>(create_eof_implementation(N));
	}

}
#endif
