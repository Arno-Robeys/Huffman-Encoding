#include "encoding/huffman/huffman-encoding.h"
#include <vector>
#include <algorithm>
#include <memory>


namespace {

	std::unique_ptr<data::Node<Datum>> build_writeable_tree(const data::Node<std::pair<Datum, u64>>& node) {
		typedef std::unique_ptr<data::Node<Datum>> NodePtr;

		if (auto leaf = dynamic_cast<const data::Leaf<std::pair<Datum, u64>>*>(&node)) {
			return std::make_unique<data::Leaf<Datum>>(leaf->get_value().first);
		}
		else if (auto branch = dynamic_cast<const data::Branch<std::pair<Datum, u64>>*>(&node)) {
			NodePtr left = build_writeable_tree(branch->get_left_child());
			NodePtr right = build_writeable_tree(branch->get_right_child());
			return std::make_unique<data::Branch<Datum>>(std::move(left), std::move(right));
		}
		return nullptr;
	}

	Datum decode_single_datum(io::InputStream& input, const data::Node<Datum>& node) {
		if (auto leaf = dynamic_cast<const data::Leaf<Datum>*>(&node)) {
			return leaf->get_value();
		}
		else if (auto branch = dynamic_cast<const data::Branch<Datum>*>(&node)) {
			if (!input.end_reached()) {
				auto bit = input.read();
				if (bit == 0) {
					return decode_single_datum(input, branch->get_left_child());
				}
				else {
					return decode_single_datum(input, branch->get_right_child());
				}
			}
		}
		return 0;
	}

	void decode_bits(io::InputStream& input, const data::Node<Datum>& node, io::OutputStream& output) {
		while (!input.end_reached()) {
			auto datum = decode_single_datum(input, node);
			output.write(datum);
		}
	}

	class HuffmanEncodingImplementation : public encoding::EncodingImplementation {

	private: 	
		Datum domain_size;

	public:
		HuffmanEncodingImplementation(Datum domain_size) : domain_size{ domain_size } {
			assert(domain_size > 0);
		}

		virtual void encode(io::InputStream& input, io::OutputStream& output) override {
			std::vector<Datum> data = copy_to_vector<Datum>(input);
			data::FrequencyTable<Datum> frequency_table = data::count_frequencies(data);
			std::unique_ptr<const data::Node<std::pair<Datum, u64>>> tree = encoding::huffman::build_tree(frequency_table);
			std::map<Datum, std::vector<Datum>> codes = encoding::huffman::build_codes(tree);

			auto domain_bits = bits_needed(domain_size);
			auto tree_write = build_writeable_tree(*tree);
			
			encoding::huffman::encode_tree(*tree_write, domain_bits, output);

			for(auto& datum : data) {
				auto code = codes[datum];
				for (auto& bit : code) {
					output.write(bit);
				}
			}
		}

		virtual void decode(io::InputStream& input, io::OutputStream& output) override {
			auto domain_bits = bits_needed(domain_size);
			auto tree = encoding::huffman::decode_tree(domain_bits, input);
			decode_bits(input, *tree, output);
		}
	};

}

std::shared_ptr<encoding::EncodingImplementation> encoding::huffman::create_huffman_implementation(u64 domain_size)
{
	return std::make_shared<HuffmanEncodingImplementation>(domain_size);
}


template<typename T>
std::vector<T> copy_to_vector(io::InputStream& input) {
	std::vector<T> data;
	while (!input.end_reached()) {
		data.push_back(input.read());
	}
	return data;
}

Datum weight(data::Node<std::pair<Datum, u64>>& node) {
	if (auto leaf = dynamic_cast<data::Leaf<std::pair<Datum, u64>>*>(&node)) {
		return leaf->get_value().second;
	}
	else if (auto branch = dynamic_cast<data::Branch<std::pair<Datum, u64>>*>(&node)) {
		auto right = &branch->get_right_child();
		auto left = &branch->get_left_child();
		return weight(*right) + weight(*left);
	}
}

bool weight_comperator(const std::unique_ptr<data::Node<std::pair<Datum, u64>>>& left, const std::unique_ptr<data::Node<std::pair<Datum, u64>>>& right) {
	return weight(*left) > weight(*right);
}

std::unique_ptr<const data::Node<std::pair<Datum, u64>>> encoding::huffman::build_tree(data::FrequencyTable<Datum>& table) {
	std::vector<std::unique_ptr<data::Node<std::pair<Datum, u64>>>> nodes;
	for (Datum value : table.values()) {
		auto pair = std::make_pair(value, table[value]);
		nodes.push_back(std::make_unique<data::Leaf<std::pair<Datum, u64>>>(pair));
	}

	while (nodes.size() > 1) {
		std::sort(nodes.begin(), nodes.end(), weight_comperator);

		auto right = std::move(nodes.back());
		auto left = std::move(nodes.at(nodes.size()-2));
		nodes.pop_back();
		nodes.pop_back();

		auto node = std::make_unique<data::Branch<std::pair<Datum, u64>>>(std::move(left), std::move(right));
		nodes.push_back(std::move(node));
	}
	return std::move(nodes.front());
}

void build_codes_helper(const data::Node<std::pair<Datum, u64>>& node, std::vector<Datum>& prefix, std::map<Datum, std::vector<Datum>>& codes)
{
	if (auto leaf = dynamic_cast<const data::Leaf<std::pair<Datum, u64>>*>(&node))
	{
		codes[leaf->get_value().first] = prefix;
	}
	else if (auto branch = dynamic_cast<const data::Branch<std::pair<Datum, u64>>*>(&node))
	{
		prefix.push_back(0);
		build_codes_helper(branch->get_left_child(), prefix, codes);
		prefix.pop_back();
		prefix.push_back(1);
		build_codes_helper(branch->get_right_child(), prefix, codes);
		prefix.pop_back();
	}
}

std::map<Datum, std::vector<Datum>> encoding::huffman::build_codes(std::unique_ptr<const data::Node<std::pair<Datum, u64>>>& tree)
{
	std::map<Datum, std::vector<Datum>> codes;
	std::vector<Datum> prefix;
	build_codes_helper(*tree, prefix, codes);
	return codes;
}

bool encoding::huffman::helper_functie_encode(std::string str) {

	std::cout << "-------------------------------------" << std::endl;
	std::cout << "Original String: " << str << std::endl;

	//Needs buffer for writing, input and output stream
	io::MemoryBuffer<256> buffer;
	io::MemoryBuffer<256> buffer2;
	io::MemoryBuffer<256> buffer3;
	
	//Insert string into buffer
	for (auto& c : str) {
		buffer.data()->push_back(c);
	}

	std::cout << "Original Size: " << buffer.data()->size() << std::endl;

	//Encode the string
	auto huffman = encoding::huffman::create_huffman_implementation(256);
	huffman->encode(*buffer.source()->create_input_stream(), *buffer2.destination()->create_output_stream());

	std::cout << "Encoded Size: " << buffer2.data()->size() << std::endl;

	//Decode the string
	huffman->decode(*buffer2.source()->create_input_stream(), *buffer3.destination()->create_output_stream());

	std::cout << "Decoded Size: " << buffer3.data()->size() << std::endl;

	//Add the decoded string to a std::string
	std::string decoded_string;
	for (auto& c : *buffer3.data()) {
		decoded_string += c;
	}
	std::cout << "Decoded String: " << decoded_string << std::endl;
	std::cout << "-------------------------------------" << std::endl;

	//Check if the string is the same as the decoded string
	if (buffer.data()->size() == buffer3.data()->size()) {
		for (int i = 0; i < buffer.data()->size(); i++) {
			if (buffer.data()->at(i) != buffer3.data()->at(i)) {
				return false;
			}
		}
	}else {
		return false;
	}

}
