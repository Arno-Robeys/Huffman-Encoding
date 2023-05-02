#ifndef ENCODING_H
#define ENCODING_H
#include "io/streams.h"
#include "util.h"
#include <io/memory-buffer.h>

namespace encoding {

	class EncodingImplementation {
	public:
		virtual ~EncodingImplementation() {}

		virtual void encode(io::InputStream& input, io::OutputStream& output) {};

		virtual void decode(io::InputStream& input, io::OutputStream& output) {};
	};

	template<u64 IN, u64 OUT>
	class Encoding {
	public:
		Encoding(std::shared_ptr<EncodingImplementation> impl) : impl{impl} {}

		EncodingImplementation* operator->() {
			return impl.get();
		}

		const EncodingImplementation* operator->() const {
			return impl.get();
		}

	private:
		std::shared_ptr<EncodingImplementation> impl;

	};
}

template<u64 IN, u64 OUT>
void encode(io::DataSource<IN> source, encoding::Encoding<IN, OUT> encoding, io::DataDestination<OUT> destination) {
	auto input_stream = source->create_input_stream();
	auto output_stream = destination->create_output_stream();

	encoding->encode(*input_stream, *output_stream);
};

template<u64 IN, u64 OUT>
void decode(io::DataSource<OUT> source, encoding::Encoding<IN, OUT> encoding, io::DataDestination<IN> destination) {
	auto input_stream = source->create_input_stream();
	auto output_stream = destination->create_output_stream();

	encoding->decode(*input_stream, *output_stream);
};

#endif
