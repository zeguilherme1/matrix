#pragma once
#include "parser.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Metadata {
    std::string announce;
    std::string comment;

    uint64_t length;
    std::string name;

    uint32_t piece_length;
    std::vector<std::array<uint8_t, 20>> piece_hashes;
    std::string info_hash;

    static Metadata from_bencode(const Bencode &root);
};

void printMetadata(const Metadata &meta);