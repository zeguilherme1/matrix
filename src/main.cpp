#include "metadata.hpp"
#include "parser.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {

    std::ifstream file(argv[1], std::ios::binary);

    std::string data((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    Parser parser(data);

    Bencode torrent = parser.parse();

    std::string test = parser.formatBencode(torrent);

    std::cout << test << std::endl;

    return 0;
}