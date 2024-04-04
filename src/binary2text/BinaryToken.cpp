#include "../../includes/binary2text/BinaryParser.h"
#include "BinaryToken.h"
#include <cstdint>

BinaryToken::BinaryToken(TokenType type, std::string value) : type(type), value(std::move(value)) {}
BinaryToken::BinaryToken(TokenType type, std::string value,  std::string second_value) : type(type), value(std::move(value)), second_value(std::move(second_value)) {}

BinaryToken::TokenType BinaryToken::getType() const {
    return type;
}
uint16_t BinaryToken::getCode() const {
    return code;
}

//For debug purposes
std::string BinaryToken::getTypeString() const {
    switch (type) {
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::OPEN_BRACE:
            return "OPEN_BRACE";
        case TokenType::CLOSE_BRACE:
            return "CLOSE_BRACE";
        case TokenType::END_OF_FILE:
            return "END_OF_FILE";
        case TokenType::HEADER:
            return "HEADER";
        case TokenType::ENCLOSED_OBJECT:
            return "ENCLOSED_OBJECT";
        case TokenType::CONDITIONAL_OBJECT:
            return "CONDITIONAL_OBJECT";
    }
    return "UNDEFINED";
}
std::string BinaryToken::getValue() const {
    return value;
}

BinaryToken::BinaryToken(TokenType type, std::string value,  uint16_t code) : type(type), value(std::move(value)), code(code) {}


