#ifndef HUFFMAN_TREE_ENCODING_H
#define HUFFMAN_TREE_ENCODING_H

#include "../encoding.h"
#include "./data/binary-tree.h"

namespace encoding {
	namespace huffman {

		void encode_tree(const data::Node<Datum>& tree, int numberOfBitsPerDatum, io::OutputStream& output);

		std::unique_ptr <data::Node<Datum>> decode_tree(int numberOfBitsPerDatum, io::InputStream& input);

	}
}

#endif