#include "tree-encoding.h"

namespace encoding  {
	namespace huffman {


			void encode_tree(const data::Node<Datum>& tree, int numberOfBitsPerDatum, io::OutputStream& output) {

				if (const data::Leaf<Datum>* leaf = dynamic_cast<const data::Leaf<Datum>*>(&tree)) {
					//If tree is a leaf, write 1 and then the value of the leaf
					output.write(0);
					io::write_bits(leaf->get_value(), numberOfBitsPerDatum, output);
				}
				else if (const data::Branch<Datum>* branch = dynamic_cast<const data::Branch<Datum>*>(&tree)) {
					//If tree is a branch, write 0 and then recursively encode its children
					output.write(1);
					encode_tree(branch->get_left_child(), numberOfBitsPerDatum, output);
					encode_tree(branch->get_right_child(), numberOfBitsPerDatum, output);
				}

			}

			std::unique_ptr<data::Node<Datum>> decode_tree(int numberOfBitsPerDatum, io::InputStream& input) {
				
				if (!input.end_reached()) {
					//Read the first bit
					auto bit = input.read();
					if (bit == 0) {
						//If the bit is 0, read the value of the leaf and return a new leaf with that value
						Datum value = io::read_bits(numberOfBitsPerDatum, input);
						return std::make_unique<data::Leaf<Datum>>(value);
					}
					else if (bit == 1) {
						//If the bit is 1, recursively decode the children and return a new branch with the results
						auto left = decode_tree(numberOfBitsPerDatum, input);
						auto right = decode_tree(numberOfBitsPerDatum, input);

						return std::make_unique<data::Branch<Datum>>(std::move(left), std::move(right));
					}
				}
			}
	}

}
