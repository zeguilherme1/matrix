#include "parser.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {

    std::ifstream file("debian.torrent", std::ios::binary);

    std::string data((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    Parser parser(data);

    Bencode torrent = parser.parse();

    // todo: implement a function to print the decoded bencode
    // printBencode(torrent);

    return 0;
}
