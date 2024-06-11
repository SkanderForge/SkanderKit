#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "Token.h"
#include <chrono>
using namespace std::chrono;

class Lexer {
public:
    virtual ~Lexer() = default;
    virtual Token getNextToken() = 0; // Pure virtual
    virtual size_t getPosition() = 0;  // Pure virtual

    virtual void setPosition(size_t pos) = 0;
private:
};

#endif
