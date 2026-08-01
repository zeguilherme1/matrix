#include "metadata.hpp"
#include "parser.hpp"
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
    std::ifstream file(argv[1], std::ios::binary);

    std::string data((std::istreambuf_iterator<char>(file)), {});

    Parser parser(data);

    Bencode torrent = parser.parse();

    Metadata meta = Metadata::from_bencode(torrent);

    std::cout << meta.announce << std::endl;
    
    return 0;
}