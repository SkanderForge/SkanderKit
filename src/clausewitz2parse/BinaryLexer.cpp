#include <stdexcept>
#include "../../includes/clausewitz2parse/BinaryLexer.h"
#include <iostream>
#include <algorithm>
#include <Dictionaries.h>
#include <utility>
#include <sstream>


std::string string_to_hex(const std::string &input) {
    static const char hex_digits[] = "0123456789abcdef";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c: input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

BinaryLexer::BinaryLexer(std::stringstream &s, const std::string &game) : source(std::move(s)), position(0), counter(0),
                                                                         debug_string("") {
    if (game == "vic3") {
        dict = vic3Dict;
    }
    if (game == "ck3") {
        dict = vic3Dict;
    }
    if (game == "eu4") {
        dict = eu4Dict;
    }

    codes = {14327, 10317, 10379, 10416, 10420, 10432, 10468, 10469, 10498, 10549, 10570, 10571, 10652, 10798, 11409,
             11480, 11482, 11523, 11525, 11529, 11586, 11601, 11612, 11706, 11816, 11817, 11824, 11825, 11870, 11987,
             12032, 12441, 12543, 12544, 12643, 12653, 12661, 12771, 12842, 12869, 12926, 12960, 12997, 13025, 13039,
             13044, 13065, 13066, 13171, 13184, 13271, 13437, 13594, 13787, 13998, 14152, 14218, 14264, 13239};


}

struct Date {
    int year;
    int month;
    int day;
};

const long long zeroDate = 43800000;

// Function to convert the Clausewitz date number to a Date struct
Date numberToDate(long long n) {
    Date date{};

    if (n < zeroDate) {
        if (n == 43791240) {
            date.year = -1;
            date.month = 1;
            date.day = 1;
        } else {
            date.year = 0;
            date.month = 0;
            date.day = 0;
        }
        return date;
    }

    n -= zeroDate;
    n /= 24;

    date.year = 0;
    date.month = 1;
    date.day = 1;

    int daysInMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
    for (int i = 365; i <= n; i += 365) {
        date.year += 1;
    }

    n -= date.year * 365;

    for (int i = 0; i < 11; i++) {
        if (n < daysInMonths[i]) {
            break;
        }
        n -= daysInMonths[i];
        date.month += 1;
    }

    date.day += n;

    return date;
}

std::string formatDate(const Date &date) {
    return std::to_string(date.year) + "." + std::to_string(date.month) + "." + std::to_string(date.day);
}

size_t BinaryLexer::getPosition() {
    size_t ret = source.tellg();
    return ret;
}

void BinaryLexer::setPosition(size_t pos) {
    source.seekg(pos);
}

Token BinaryLexer::getNextToken() {
    uint16_t code;
    char bytes_[2];
    source.read(bytes_, 2);
    code = *reinterpret_cast<uint16_t *>(bytes_);
    // uint8_t val;
    // source.read(reinterpret_cast<char *>(&val), sizeof(val));




    switch (code) {
        case 0: {
            return {Token(Token::TokenType::END_OF_FILE, "")};
            break;
        }
        case 14065: {

            std::string current_str = source.str();
            std::string new_str = current_str;
            auto old_pos = source.tellg();
            auto pos_for_insert = old_pos.operator+(0);

            new_str.insert(pos_for_insert, 1, char(0));

            new_str.insert(pos_for_insert, 1, char(1));

            source.str("");
            source.clear(); // Clear any error flags
            source << new_str;
            source.seekg(old_pos);
        }
        case 3: {
            return {Token(Token::TokenType::OPEN_BRACE, "{")};
            break;
        }
        case 4: {
            return {Token(Token::TokenType::CLOSE_BRACE, "}")};
            break;
        }
        case 1: {
            return {Token(Token::TokenType::EQUAL, "=")};
            break;
        }
        case 12: {
            int32_t val; // Assuming 4-byte integer (32 bits)
            source.read(reinterpret_cast<char *>(&val), sizeof(val));

            for (uint16_t datecode: codes) {
                if (std::find(parents.begin(), parents.end(), datecode) != parents.end()) {
                    //std::cout << formatDate(numberToDate(val)) << std::endl;
                    return {Token(Token::TokenType::IDENTIFIER, formatDate(numberToDate(val)))};
                }
            }

            std::string output = std::to_string(val);
            return {Token(Token::TokenType::IDENTIFIER, output)};
            break;
        }
        case 13: {
            int32_t val; // Assuming 4-byte integer (32 bits)
            source.read(reinterpret_cast<char *>(&val), sizeof(val));
            val /= 1000;
            std::string output = std::to_string(val);
            return {Token(Token::TokenType::IDENTIFIER, output)};
            break;
        }
        case 14: {
            bool val;
            source.read(reinterpret_cast<char *>(&val), 1);
            std::string output;
            val ? output += "true" : output += "false";
            return {Token(Token::TokenType::IDENTIFIER, output)};
            break;
        }
        case 15: {
            uint16_t length;
            source.read(reinterpret_cast<char *>(&length), sizeof(length));
            char buffer[length + 1];
            source.read(buffer, length);
            buffer[length] = '\0';
            std::string val(buffer);
            if (!val.empty() && val[val.length() - 1] == '\n') {
                val.erase(val.length() - 1);
            }
            return {Token(Token::TokenType::IDENTIFIER, val)};
            break;
        }
        case 20: {
            int32_t val; // Assuming 4-byte integer (32 bits)
            source.read(reinterpret_cast<char *>(&val), sizeof(val));
            std::string output = std::to_string(val);
            return {Token(Token::TokenType::IDENTIFIER, output)};

            break;
        }
        case 23: {
            uint16_t length;
            source.read(reinterpret_cast<char *>(&length), sizeof(length));
            char buffer[length + 1];
            source.read(buffer, length);
            buffer[length] = '\0';
            std::string val(buffer);
            return {Token(Token::TokenType::IDENTIFIER, val)};

            break;
        }
        case 359: {
            int64_t val;
            source.read(reinterpret_cast<char *>(&val), sizeof(val));
            auto floatVal = static_cast<float>(val);
            floatVal /= 100000;
            std::string output = std::to_string(floatVal);
            return {Token(Token::TokenType::IDENTIFIER, output)};
            break;
        }
        case 668: {
            int64_t val;
            source.read(reinterpret_cast<char *>(&val), sizeof(val));
            auto floatVal = static_cast<float>(val);
            floatVal /= 100000;
            std::string output = std::to_string(floatVal);
            return {Token(Token::TokenType::IDENTIFIER, output)};
            break;
        }
        default: {
            if (dict.find(std::to_string(code)) != dict.end()) {
                //std::cout << code << dict[std::to_string(code)] << std::endl;
                return {Token(Token::TokenType::IDENTIFIER, dict[std::to_string(code)], code)};
            }
            return {Token(Token::TokenType::IDENTIFIER, "UNK_" + std::to_string(code), code)};

            break;

        }

    }

}

void BinaryLexer::pushToParents(uint16_t code) {
    parents.push_back(code);
}

void BinaryLexer::popParent() {
    parents.pop_back();
}
