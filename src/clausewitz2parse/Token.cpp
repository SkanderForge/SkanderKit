#include "../../includes/clausewitz2parse/Token.h"

Token::Token(TokenType type, std::string value) : type(type), value(std::move(value)) {}
Token::Token(TokenType type, std::string value,  std::string second_value) : type(type), value(std::move(value)), second_value(std::move(second_value)) {}
Token::Token(TokenType type, std::string value,  uint16_t code) : type(type), value(std::move(value)), code(code) {}

Token::TokenType Token::getType() const {
    return type;
}

uint16_t Token::getCode() const {
    return code;
}

//For debug purposes
std::string Token::getTypeString() const {
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
std::string Token::getValue() const {
    return value;
}
