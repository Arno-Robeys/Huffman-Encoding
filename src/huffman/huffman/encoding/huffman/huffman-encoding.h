#ifndef HUFFMAN_ENCODING_H
#define HUFFMAN_ENCODING_H
#include "encoding/encoding.h"
#include "data/frequency-table.h"
#include "data/binary-tree.h"
#include <map>


template<typename T>
extern std::vector<T> copy_to_vector(io::InputStream& input);

namespace encoding {
	namespace huffman {

		std::unique_ptr<const data::Node<std::pair<Datum, u64>>> build_tree(data::FrequencyTable<Datum>& table);

		std::map<Datum, std::vector<Datum>> build_codes(std::unique_ptr<const data::Node<std::pair<Datum, u64>>>& tree);
	}
}

#endif
