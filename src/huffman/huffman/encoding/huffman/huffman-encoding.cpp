#include "encoding/huffman/huffman-encoding.h"
#include <vector>
#include "data/frequency-table.h"

namespace {
	class HuffmanEncodingImplementation : public encoding::EncodingImplementation {

		virtual void encode(io::InputStream& input, io::OutputStream& output) override {
			std::vector<Datum> data = copy_to_vector<Datum>(input);
			data::FrequencyTable<Datum> frequency_table = data::count_frequencies(data);
			
		}

		virtual void decode(io::InputStream& input, io::OutputStream& output) override {}
	};

}

template<typename T>
std::vector<T> copy_to_vector(io::InputStream& input) {
	std::vector<T> data;
	while (!input.end_reached()) {
		data.push_back(input.read());
	}
	return data;
}

