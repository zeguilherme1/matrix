#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct Metadata {
    std::string announce;
    std::string comment;

    u_int64_t length;
    std::string name;

    u_int32_t piece_length;
    std::vector<std::array<u_int8_t, 20>> piece_hashes;
};