#include <stdexcept>
#include "../../includes/binary2text/BinaryLexer.h"
#include <iostream>
#include <algorithm>
#include <Dictionaries.h>
#include <utility>


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

BinaryLexer::BinaryLexer(std::stringstream s, const std::string &game) : source(std::move(s)), position(0), counter(0),
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
        // Special case (consider handling -1.1.1 differently if needed)
        if (n == 43791240) {
            date.year = -1;
            date.month = 1;
            date.day = 1;
        } else {
            // Here, we might want more robust error handling instead of simply returning
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

// Function to format the Date into a string
std::string formatDate(const Date &date) {
    return std::to_string(date.year) + "." + std::to_string(date.month) + "." + std::to_string(date.day);
}


std::streampos BinaryLexer::getPosition() {
    auto ret = source.tellg();
    return ret;
}

void BinaryLexer::setPosition(std::streampos pos) {
    source.seekg(pos);
}

BinaryToken BinaryLexer::getNextToken() {
    uint16_t code;
    char bytes_[2];
    source.read(bytes_, 2);
    code = *reinterpret_cast<uint16_t *>(bytes_);
    // uint8_t val;
    // source.read(reinterpret_cast<char *>(&val), sizeof(val));
    if (code == 0) {
        return {BinaryToken(BinaryToken::TokenType::END_OF_FILE, "")};
    }

    //  std::cout << code << std::endl; // Output: AB

    if (code == 14065) {
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

        //  return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, "UNK_" + std::to_string(code))};
    }


    if (code == 3) return {BinaryToken(BinaryToken::TokenType::OPEN_BRACE, "{")};
    if (code == 4) return {BinaryToken(BinaryToken::TokenType::CLOSE_BRACE, "}")};
    if (code == 1) return {BinaryToken(BinaryToken::TokenType::EQUAL, "=")};

    std::vector<uint16_t> codes = {14327, 10317, 10379, 10416, 10420, 10432, 10468, 10469, 10498, 10549, 10570, 10571,
                                   10652, 10798, 11409, 11480, 11482, 11523, 11525, 11529, 11586, 11601, 11612, 11706,
                                   11816, 11817, 11824, 11825, 11870, 11987, 12032, 12441, 12543, 12544, 12643, 12653,
                                   12661, 12771, 12842, 12869, 12926, 12960, 12997, 13025, 13039, 13044, 13065, 13066,
                                   13171, 13184, 13271, 13437, 13594, 13787, 13998, 14152, 14218, 14264};


    if (code == 12) {
        int32_t val; // Assuming 4-byte integer (32 bits)
        source.read(reinterpret_cast<char *>(&val), sizeof(val));

        for (uint16_t datecode: codes) {
            if (std::find(parents.begin(), parents.end(), datecode) != parents.end()) {
                //std::cout << formatDate(numberToDate(val)) << std::endl;
                return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, formatDate(numberToDate(val)))};
            }
        }

        std::string output = std::to_string(val);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }

    if (code == 13) {
        int32_t val; // Assuming 4-byte integer (32 bits)
        source.read(reinterpret_cast<char *>(&val), sizeof(val));
        val /= 1000;
        std::string output = std::to_string(val);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }

    if (code == 14) {
        bool val;
        source.read(reinterpret_cast<char *>(&val), 1);
        std::string output;
        val ? output += "true" : output += "false";
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }

    if (code == 15) {
        uint16_t length; // Unsigned short (2 bytes)
        source.read(reinterpret_cast<char *>(&length), sizeof(length));


        char buffer[length + 1]; // +1 for null terminator
        source.read(buffer, length);
        buffer[length] = '\0'; // Null-terminate the string
        std::string val(buffer);
        if (!val.empty() && val[val.length() - 1] == '\n') {
            val.erase(val.length() - 1);
        }


        //  std::cout << "String " << val << std::endl;
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, val)};
    }

    if (code == 20) {
        int32_t val; // Assuming 4-byte integer (32 bits)
        source.read(reinterpret_cast<char *>(&val), sizeof(val));
        std::string output = std::to_string(val);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }
    if (code == 23) {
        uint16_t length;
        source.read(reinterpret_cast<char *>(&length), sizeof(length));
        char buffer[length + 1];
        source.read(buffer, length);
        buffer[length] = '\0';
        std::string val(buffer);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, val)};
    }
    if (code == 359) {
        int64_t val;
        source.read(reinterpret_cast<char *>(&val), sizeof(val));
        auto floatVal = static_cast<float>(val);
        floatVal /= 100000;
        std::string output = std::to_string(floatVal);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }
    if (code == 668) {
        int64_t val;
        source.read(reinterpret_cast<char *>(&val), sizeof(val));
        auto floatVal = static_cast<float>(val);
        floatVal /= 100000;
        std::string output = std::to_string(floatVal);
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, output)};
    }
    // std::cout << "Missed a code " << code << std::endl;



    if (dict.find(std::to_string(code)) != dict.end()) {
        //std::cout << code << dict[std::to_string(code)] << std::endl;
        return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, dict[std::to_string(code)], code)};
    }


    return {BinaryToken(BinaryToken::TokenType::IDENTIFIER, "UNK_" + std::to_string(code), code)};
}

void BinaryLexer::pushToParents(uint16_t code) {
    parents.push_back(code);
}

void BinaryLexer::popParent() {
    parents.pop_back();
}
