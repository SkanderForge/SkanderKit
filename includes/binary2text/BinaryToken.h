#ifndef BC2J_TOKEN_H
#define BC2J_TOKEN_H

#include <string>
#include <cstdint>

class BinaryToken {
public:
    enum class TokenType {
        EQUAL, NUMBER, IDENTIFIER, OPEN_BRACE, ENCLOSED_OBJECT, CONDITIONAL_OBJECT, CLOSE_BRACE, END_OF_FILE,HEADER
    };
    BinaryToken(TokenType type, std::string value, std::string second_value);
    BinaryToken(TokenType type, std::string value);
    BinaryToken(TokenType type, std::string value,  uint16_t code);

    TokenType getType() const;
    std::string getValue() const;
    std::string getTypeString() const;
    std::string value;
    uint16_t code;
    std::string second_value;
    TokenType type;

    uint16_t getCode() const;
};

#endif
