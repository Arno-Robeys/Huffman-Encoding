#include "encoding/huffman/huffman-encoding.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

namespace {
	class HuffmanEncodingImplementation : public encoding::EncodingImplementation {

		virtual void encode(io::InputStream& input, io::OutputStream& output) override {
			std::vector<Datum> data = copy_to_vector<Datum>(input);
			data::FrequencyTable<Datum> frequency_table = data::count_frequencies(data);
			std::unique_ptr<const data::Node<std::pair<Datum, u64>>> tree = encoding::huffman::build_tree(frequency_table);
			std::map<Datum, std::vector<Datum>> codes = encoding::huffman::build_codes(tree);
			
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

unsigned long int weight(data::Node<std::pair<Datum, u64>>& node) {
	if (dynamic_cast<data::Leaf<std::pair<Datum, u64>>*>(&node) != nullptr) {
		return dynamic_cast<const data::Leaf<std::pair<Datum, u64>>*>(&node)->get_value().second;
	}
	else if(dynamic_cast<data::Branch<std::pair<Datum, u64>>*>(&node) != nullptr) {
		auto right = dynamic_cast<data::Branch<std::pair<Datum, u64>>*>(&node)->get_right_child();
		auto left = dynamic_cast<data::Branch<std::pair<Datum, u64>>*>(&node)->get_left_child();
		return weight(right) + weight(left);
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
