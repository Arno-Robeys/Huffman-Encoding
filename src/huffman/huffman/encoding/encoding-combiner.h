#ifndef ENCODING_COMBINAR_H
#define ENCODING_COMBINAR_H
#include "encoding.h"

namespace encoding {

	template<u64 N1, u64 N2, u64 N3>
	class EncodingCombinerImplementation : public EncodingImplementation {
	public:

		EncodingCombinerImplementation(Encoding<N1,N2> first, Encoding<N2,N3> second) : first{first}, second{second} {}
		
		void encode(io::InputStream& input, io::OutputStream& output) override {
			auto tempbuffer = io::MemoryBuffer<N1>();
			auto tempoutputstream = tempbuffer.destination()->create_output_stream();
			auto tempinputstream = tempbuffer.source()->create_input_stream();

			first->encode(input, *tempoutputstream);
			second->encode(*tempinputstream, output);
		}
		
		void decode(io::InputStream& input, io::OutputStream& output) override {
			auto tempbuffer = io::MemoryBuffer<N3>();
			auto tempoutputstream = tempbuffer.destination()->create_output_stream();
			auto tempinputstream = tempbuffer.source()->create_input_stream();

			second->decode(input, *tempoutputstream);
			first->decode(*tempinputstream, output);
		}

	private:
		Encoding<N1,N2> first;
		Encoding<N2,N3> second;
	};


	template<u64 N1, u64 N2, u64 N3>
	Encoding<N1, N3> operator|(Encoding<N1, N2> first, Encoding<N2, N3> second) {
		return Encoding<N1, N3>(std::make_shared<EncodingCombinerImplementation<N1, N2, N3>>(first, second));
	}
}

#endif
