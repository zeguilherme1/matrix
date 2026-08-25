#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <map>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>

/*
    Bencode Parser

    Done - Integers are encoded as i<base10 integer>e.

    Done - Byte Strings are encoded as <length>:<contents>.

    Done - Lists are encoded as l<elements>e.

    Done - Dictionaries are encoded as d<pairs>e.
*/

struct Bencode {
    enum class Type { Integer, String, List, Dict };

    Type type;
    int64_t integer;
    std::string string;
    std::vector<Bencode> list;
    std::map<std::string, Bencode> dict;
};

class Parser {
  public:
    explicit Parser(const std::string &str) : m_str(str) {}

    Bencode parse() {
        switch (peek()) {
        case 'i':
            return parseInt();
            break;
        case 'l':
            return parseList();
            break;
        case 'd':
            return parseDict();
            break;
        default:
            return parseString();
            break;
        }
    }

    Bencode parseInt() {
        consume();
        std::string int_str;

        // if is a negative number
        if (peek() == '-') {
            consume();
            int_str += '-';
        }

        while (isdigit(peek())) {
            int_str += consume();

            if (peek() == 'e') {
                consume();
                break;
            }
        }

        // todo: reimplement the leading zeros remove logic
        // if (str.at(i) == 'e') {
        //     break;
        // }
        // remove all leading zeros
        // while (str.at(m_pos) == '0') {
        //    int_string.erase(m_pos, 1);
        //}

        int parsedInt = stoi(int_str);

        return {.type = Bencode::Type::Integer, .integer = parsedInt};
    }

    Bencode parseString() {
        std::string tmp_str_size;
        size_t str_size; // if our string size is N

        while (isdigit(peek())) {
            tmp_str_size += consume();

            if (peek() == ':') {
                consume();
                break;
            }
        }
        str_size = stoi(tmp_str_size);

        std::string parsed_string;
        // reads exactly N bytes
        while (str_size > 0) {
            parsed_string += consume();
            str_size--;
        }

        return {.type = Bencode::Type::String, .string = parsed_string};
    }

    Bencode parseList() {
        consume();

        std::vector<Bencode> values;

        while (peek() != 'e') {
            values.push_back(parse());
        }
        consume();

        return {.type = Bencode::Type::List, .list = values};
    }

    Bencode parseDict() {
        consume();

        std::map<std::string, Bencode> dict;

        while (peek() != 'e') {
            Bencode key = parseString();

            if (key.string == "info") {
                m_info_start = m_pos;

                Bencode value = parse();

                m_info_end = m_pos;

                dict.insert({key.string, value});
            } else {
                Bencode value = parse();
                dict.insert({key.string, value});
            }
        }
        consume();

        return {.type = Bencode::Type::Dict, .dict = dict};
    }

    std::string formatBencode(const Bencode &value, int depth = 0) {
        std::string indent(depth * 2, ' ');
        std::string formatted_bencode;

        switch (value.type) {
        case Bencode::Type::Integer:
            formatted_bencode +=
                (indent + std::to_string(value.integer) + "\n");
            break;
        case Bencode::Type::String:
            formatted_bencode += indent + "\"" + value.string + "\"\n";
            break;
        case Bencode::Type::Dict: {
            formatted_bencode += indent + "{\n";

            bool first = true;

            for (const auto &[key, val] : value.dict) {
                if (!first)
                    formatted_bencode += ",\n";

                first = false;

                formatted_bencode += indent + "  \"" + key + "\": ";
                formatted_bencode += formatBencode(val, depth + 1);
            }

            formatted_bencode += "\n" + indent + "}";
            break;
        }
        case Bencode::Type::List:
            formatted_bencode += (indent + "[\n");
            bool first = true;
            for (const auto &item : value.list) {
                if (!first)
                    formatted_bencode += ",\n";
                first = false;
                formatted_bencode += formatBencode(item, depth + 1);
            }
            formatted_bencode += (indent + "]\n");
            break;
        }

        return formatted_bencode;
    }

    std::string getRawInfo() {
        return (m_str.substr(m_info_start, m_info_end - m_info_start));
    }

    std::array<uint8_t, 20> getInfoHash() {
        std::string raw_info = getRawInfo();
        std::array<uint8_t, 20> info_hash{};

        SHA1(reinterpret_cast<const unsigned char *>(raw_info.data()),
             raw_info.size(), info_hash.data());

        return info_hash;
    }

  private:
    const std::string &m_str;
    size_t m_pos = 0;
    size_t m_info_start;
    size_t m_info_end;

    uint8_t peek() { return static_cast<uint8_t>(m_str.at(m_pos)); }
    uint8_t consume() { return static_cast<uint8_t>(m_str.at(m_pos++)); }
};
