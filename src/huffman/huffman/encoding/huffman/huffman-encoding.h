#ifndef HUFFMAN_ENCODING_H
#define HUFFMAN_ENCODING_H
#include "encoding/encoding.h"
#include "data/frequency-table.h"
#include "data/binary-tree.h"
#include <map>
#include "tree-encoding.h"


template<typename T>
extern std::vector<T> copy_to_vector(io::InputStream& input);

namespace encoding {
	namespace huffman {

		std::shared_ptr<EncodingImplementation> create_huffman_implementation(u64 domain_size);

		template<unsigned DOMAIN_SIZE>
		encoding::Encoding<DOMAIN_SIZE, 2>huffman_encoding()
		{
			return create_huffman_implementation(DOMAIN_SIZE);
		}

		std::unique_ptr<const data::Node<std::pair<Datum, u64>>> build_tree(data::FrequencyTable<Datum>& table);

		std::map<Datum, std::vector<Datum>> build_codes(std::unique_ptr<const data::Node<std::pair<Datum, u64>>>& tree);
		
		bool helper_functie_encode(std::string str);
	}
}

#endif
