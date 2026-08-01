#include "metadata.hpp"
#include "parser.hpp"
#include <array>
#include <cstdint>

Metadata Metadata::from_bencode(const Bencode &root) {
    Metadata meta;

    auto &dict = root.dict;

    meta.announce = dict.at("announce").string;

    auto &info = dict.at("info").dict;

    meta.name = info.at("name").string;

    meta.length = info.at("length").integer;

    meta.piece_length = info.at("piece length").integer;

    auto &pieces = info.at("pieces").string;

    for (size_t i = 0; i < pieces.size(); i += 20) {
        std::array<uint8_t, 20> hash;

        std::copy(pieces.begin() + i, pieces.begin() + i + 20, hash.begin());

        meta.piece_hashes.push_back(hash);
    }
    return meta;
}