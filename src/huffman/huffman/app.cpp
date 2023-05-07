#ifndef TEST_BUILD

/*
*   This file is only active in non-test builds (i.e., builds where TEST_BUILD is not defined).
*/

#include <iostream>
#include "encoding/huffman\huffman-encoding.h"
#include "encoding/eof-encoding.h"
#include "encoding/bit-grouper.h"
#include "encoding/encoding-combiner.h"
#include "io/files.h"



/*
* This is the main entry point of your application.
* Note that it will only work in non-test builds.
*/
int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "Usage: huffman <mode> <input file> <output file>" << std::endl;
		std::cout << "Mode can be either 'compress' or 'decompress'" << std::endl;
		return 1;
    } 

    std::string mode = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    if (mode == "compress") {
        std::cout << "Compressing " << input_file << " to " << output_file << std::endl;
		auto input = io::create_file_data_source(input_file);
        auto output = io::create_file_data_destination(output_file);
        auto eof = encoding::eof_encoding<256>();
        auto huffman = encoding::huffman::huffman_encoding<257>();
        auto group = encoding::bit_grouper<8>();
        auto combiner = eof | huffman | group;

        encode(input, combiner, output);
    }
    else if (mode == "decompress") {
        std::cout << "Decompressing " << input_file << " to " << output_file << std::endl;
        auto input = io::create_file_data_source(input_file);
        auto output = io::create_file_data_destination(output_file);
        auto eof = encoding::eof_encoding<256>();
        auto huffman = encoding::huffman::huffman_encoding<257>();
        auto group = encoding::bit_grouper<8>();
        auto combiner = eof | huffman | group;

        decode(input, combiner, output);
    }
    else {
		std::cout << "Invalid mode: " << mode << std::endl;
		return 1;
	}

/*#ifdef NDEBUG
    std::cout << "You are running the release build" << std::endl;
#else
    std::cout << "You are running the debug build" << std::endl;
#endif*/

}

#endif