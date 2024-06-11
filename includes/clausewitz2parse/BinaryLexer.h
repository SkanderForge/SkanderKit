#ifndef BC2J_LEXER_H
#define BC2J_LEXER_H

#include <string>
#include <chrono>
#include <unordered_map>
#include <sstream>
#include <vector>
#include "Lexer.h"
#include "Token.h"

using namespace std::chrono;

class BinaryLexer: public Lexer {
public:
    explicit BinaryLexer(std::stringstream &s, const std::string &game);

    Token getNextToken() override;

    size_t getPosition() override;
    void pushToParents(uint16_t code);
    void popParent();

    void setPosition(size_t pos) override;
    std::unordered_map<std::string, std::string> dict;
    std::string debug_string;
private:
    int counter;
    std::stringstream source;
    size_t position;
    typedef std::chrono::duration<float> float_seconds;
    std::vector<uint16_t> parents;
    std::vector<uint16_t> codes;
};

#endif
