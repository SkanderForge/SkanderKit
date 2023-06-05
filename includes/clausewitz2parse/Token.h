#ifndef C2J_TOKEN_H
#define C2J_TOKEN_H

#include <string>


class Token {
public:
    enum class TokenType {
        EQUAL, NUMBER, IDENTIFIER, OPEN_BRACE, ENCLOSED_OBJECT, CONDITIONAL_OBJECT, CLOSE_BRACE, END_OF_FILE,HEADER
    };
    Token(TokenType type, std::string value, std::string second_value);
    Token(TokenType type, std::string value);

    TokenType getType() const;
    std::string getValue() const;
    std::string getTypeString() const;
    std::string value;
    std::string second_value;
    TokenType type;
};

#endif
