#ifndef C2J_TOKEN_H
#define C2J_TOKEN_H

#include <string>
#include <cstdint>


class Token {
public:
    enum class TokenType {
        EQUAL, NUMBER, IDENTIFIER, OPEN_BRACE, ENCLOSED_OBJECT, CONDITIONAL_OBJECT, CLOSE_BRACE, END_OF_FILE,HEADER
    };
    Token(TokenType type, std::string value, std::string second_value);
    Token(TokenType type, std::string value);
    Token(TokenType type, std::string value,  uint16_t code);

    TokenType getType() const;
    std::string getValue() const;
    std::string getTypeString() const;
    uint16_t code;
    std::string value;
    std::string second_value;
    TokenType type;
    uint16_t getCode() const;
};

#endif
