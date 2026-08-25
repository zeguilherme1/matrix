#include "metadata.hpp"
#include "parser.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string generate_peer_id() { return "-ZG0001-abcdefghijkl"; }

int main(int argc, char *argv[]) {
    std::ifstream file(argv[1], std::ios::binary);

    std::string data((std::istreambuf_iterator<char>(file)), {});

    Parser parser(data);

    Bencode torrent = parser.parse();

    auto info_hash = parser.getInfoHash();

    Metadata meta = Metadata::from_bencode(torrent);

    std::cout << "Info hash (URL encoded): ";
    for (int i = 0; i < 20; i++) {
        printf("%%%02X", info_hash[i]);
    }
    std::cout << std::endl;

    printMetadata(meta);

    std::ifstream file2("response", std::ios::binary);
    std::string data2((std::istreambuf_iterator<char>(file2)), {});

    Parser parser2(data2);

    Bencode response = parser2.parse();

    std::string peers = response.dict["peers"].string;

    std::cout << "Peers: " << std::endl;
    for (size_t i = 0; i < peers.size(); i += 6) {
        uint8_t a = peers[i];
        uint8_t b = peers[i + 1];
        uint8_t c = peers[i + 2];
        uint8_t d = peers[i + 3];

        uint8_t p1 = peers[i + 4];
        uint8_t p2 = peers[i + 5];

        
        uint16_t port = ((uint8_t)p1 << 8) | (uint8_t)p2;

        std::cout << int(a) << '.' << int(b) << '.' << int(c) << '.' << int(d)
                  << ':' << int(port) << std::endl;
    }
    return 0;
}