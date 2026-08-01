#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
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
    int integer;
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
            Bencode bencode_key = parseString();
            Bencode value = parse();

            std::string key = bencode_key.string;
            dict.insert({key, value});
        }
        consume();

        return {.type = Bencode::Type::Dict, .dict = dict};
    }

  private:
    const std::string &m_str;
    size_t m_pos = 0;

    char peek() { return m_str.at(m_pos); }
    char consume() { return m_str.at(m_pos++); }
};

