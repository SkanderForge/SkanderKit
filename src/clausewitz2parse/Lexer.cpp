#include <stdexcept>
#include "../../includes/clausewitz2parse/Lexer.h"
#include <iostream>
#include <algorithm>

Lexer::Lexer(std::string s) : source(std::move(s)), position(0), counter(0), debug_string("") {}

size_t Lexer::getPosition() const { return position; }

void Lexer::setPosition(size_t pos) {
    position = pos;
}

Token Lexer::getNextToken() {
    while (position < source.length() && (std::isspace(source[position]) || source[position] == 0)) {
        position++;
    }
    // If we're at the end of the input, return an END_OF_FILE token
    if (position >= source.length()) {
        return {Token::TokenType::END_OF_FILE, ""};
    }
    unsigned char current = source[position];
    //Skip comments
    if (current == '#') {
        do {
            position++;
            current = source[position];
        } while (current != '\r' && current != '\n' && position < source.length());
        position++;
        return getNextToken();
    }
    if (current == '\\') {
        position++;
        current = source[position];
    }
    //  debug_string.push_back(current);
    if (current == '=') {
        position++;
        return {Token::TokenType::EQUAL, "="};
    } else if (current == '{') {
        position++;
        return {Token::TokenType::OPEN_BRACE, "{"};
    } else if (current == '}') {
        position++;
        return {Token::TokenType::CLOSE_BRACE, "}"};
    } else if (Lexer::isString(current)) {
        return getIdentifierToken();
    }
    // Unrecognized character, throw an error
    throw std::runtime_error("Unrecognized character: " + std::to_string(current) + debug_string);
}


void Lexer::skipWhitespace() {
    while (position < source.length() && (std::isspace(source[position]) || source[position] == 0)) {
        position++;
    }
}

Token Lexer::getIdentifierToken() {
    size_t start = position;
    bool in_quotations = false;
    bool ignore_quotations = false;
    while (position < source.length() && Lexer::isString(source[position], in_quotations)) {
        if (!ignore_quotations) {
            if (source[position] == '"') {
                if (!in_quotations) {
                    in_quotations = true;
                } else {
                    in_quotations = false;
                }
            }
        } else {
            ignore_quotations = false;
        }
        if (source[position] == '\\') ignore_quotations = true;

        position++;
    }
    std::string identifier = source.substr(start, position - start);
    //Let's hope we don't end up exploding something because of it.
    std::replace(identifier.begin(), identifier.end(), '\\', '/');
    if (identifier == "EU4txt") return getNextToken();
    //Dirty hack for dealing with objects that miss an equal sign, like map_area_data{}
    //If someone can figure out something smarter please let me know
    if(source[position]=='{'){
        source[position] = '=';
        source[position+1] = '{';
    }

    //EU4 has a rather hacky syntax, where in some situations value of a "" string should be parsed as an object.
    if (identifier.find('{') != -1 || identifier.find('=') != -1) {
        identifier.erase(0, 1);
        identifier.erase(identifier.length() - 1, 1);
        return {Token::TokenType::ENCLOSED_OBJECT, identifier};
    }

    //EU4 has weird conditional statements, like [[variable] { code }]
    if (identifier[0] == '[') {
        identifier.erase(0, 1);
        size_t c_start = identifier.find('[');
        size_t c_end = identifier.find(']');
        std::string conditional = identifier.substr(c_start + 1, c_end - 1);
        std::string object;
        position++;
        while (source[position] != ']') {
            object.push_back(source[position]);
            position++;
        }
        position++;
        return {Token::TokenType::CONDITIONAL_OBJECT, conditional, object};
    }

    return {Token::TokenType::IDENTIFIER, identifier};
}

bool Lexer::isString(unsigned char ch, bool in_quotations) {

    char underscore = '_';
    char equals = ':';
    char hyphen = '-';
    char quote = '\'';
    char dot = '.';
    char quotations = '"';
    bool isString = in_quotations || (((ch >= 34 && ch <= 122) || (ch >= 126 && ch <= 255)) && ch != 61 && ch != 93);
    //std::iswalnum(ch) || in_quotations || ch == quote || ch==equals|| ch == underscore || ch == dot || ch == quotations || ch == hyphen;
    return isString;
}

