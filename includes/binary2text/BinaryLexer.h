#ifndef BC2J_LEXER_H
#define BC2J_LEXER_H

#include <string>
#include "BinaryToken.h"
#include <chrono>
#include <unordered_map>
using namespace std::chrono;

class BinaryLexer {
public:
    explicit BinaryLexer(std::stringstream s, const std::string& game);

    BinaryToken getNextToken();

    std::streampos getPosition();
    void pushToParents(uint16_t code);
    void popParent();

    void setPosition(std::streampos pos);
    std::unordered_map<std::string, std::string> dict;
    std::string debug_string;
private:
    int counter;
    std::stringstream source;
    size_t position;
    typedef std::chrono::duration<float> float_seconds;
    std::vector<uint16_t> parents;
    bool isString(unsigned char ch, bool in_quotations = false);
    std::string readAsHex(int length = 1);

    void skipWhitespace();
    BinaryToken getIdentifierToken();
    BinaryToken getValueToken();
    BinaryToken readString();

};

#endif
