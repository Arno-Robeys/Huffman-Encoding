#include "eof-encoding.h"
#include "io/memory-buffer.h"


namespace {

	class EofEncodingImplementation : public encoding::EncodingImplementation {

	private:
		const u64 domain_size;

	public:
		EofEncodingImplementation(u64 domain_size) : domain_size{domain_size} {}

		void encode(io::InputStream& input, io::OutputStream& output) override {
			while (!input.end_reached()) {
				auto value = input.read();
				output.write(value);
			}
			output.write(domain_size);
		}

		void decode(io::InputStream& input, io::OutputStream& output) override {
			while (true) {
				auto value = input.read();
				if (value == domain_size) {
					break;
				}
				output.write(value);
			}
		}
	};

}

std::shared_ptr<encoding::EncodingImplementation> encoding::create_eof_implementation(u64 domain_size)
{
	return std::make_shared<EofEncodingImplementation>(domain_size);
}
