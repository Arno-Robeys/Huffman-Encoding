#include "bit-grouper.h"


namespace {

	class BitGrouperImplementation : public encoding::EncodingImplementation {
	private:
		const u64 group_size;

	public:
		BitGrouperImplementation(u64 group_size) : group_size{group_size} {}

		void encode(io::InputStream& input, io::OutputStream& output) override {
			while (!input.end_reached()) {
				auto value = io::read_bits(group_size, input);
				output.write(value);
			}
		}

		void decode(io::InputStream& input, io::OutputStream& output) override {
			while (!input.end_reached()) {
				auto value = input.read();
				io::write_bits(value, group_size, output);
			}
		}
	};;
}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_bit_grouper_implementation(u64 group_size)
{
	return std::make_shared<BitGrouperImplementation>(group_size);
}

