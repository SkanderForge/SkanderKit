#ifndef C2J_PLAINTEXT_LEXER_H
#define C2J_PLAINTEXT_LEXER_H

#include <string>
#include "Token.h"
#include <chrono>
#include "Lexer.h"
using namespace std::chrono;

class PlainTextLexer: public Lexer {
public:
    explicit PlainTextLexer(std::string s);

    Token getNextToken() override;

    size_t getPosition() override;

    void setPosition(size_t pos) override;

    std::string debug_string;
private:
    int counter;
    std::string source;
    size_t position;
    typedef std::chrono::duration<float> float_seconds;

    bool isString(unsigned char ch, bool in_quotations = false);

    void skipWhitespace();
    Token getIdentifierToken();
};

#endif
