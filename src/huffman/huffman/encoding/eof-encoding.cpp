#include "eof-encoding.h"

namespace {

	class EofEncodingImplementation : public encoding::EncodingImplementation {

	private:
		const u64 domain_size;

	public:
		EofEncodingImplementation(u64 domain_size) : domain_size{domain_size} {}

		void encode(io::InputStream& input, io::OutputStream& output) override {
			while (!input.end_reached()) {
				auto value = io::read_bits(domain_size, input);
				io::write_bits(value, domain_size, output);
			}
			io::write_bits(domain_size+1, 1, output);
			
		}
		void decode(io::InputStream& input, io::OutputStream& output) override {
			while (!input.end_reached()) {
				auto value = io::read_bits(domain_size+1, input);
				if (value == domain_size+1) {
					break;
				}
				io::write_bits(value, domain_size, output);
			}
		}
	};

}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_eof_implementation(u64 domain_size)
{
	return std::make_shared<EofEncodingImplementation>(domain_size);
}
