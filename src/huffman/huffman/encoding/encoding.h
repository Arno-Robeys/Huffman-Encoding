#ifndef ENCODING_H
#define ENCODING_H
#include "io/streams.h"
#include "util.h"

namespace encoding {

	class EncodingImplementation {
	public:
		virtual ~EncodingImplementation() {}

		virtual void encode(io::InputStream& input, io::OutputStream& output) {};

		virtual void decode(io::InputStream& input, io::OutputStream& output) {};
	};

	template<typename u64 IN, typename u64 OUT>
	class Encoding {
	public:
		Encoding(EncodingImplementation* impl) : impl{impl} {}

	private:
		std::unique_ptr<EncodingImplementation> impl;

	};
}

#endif
