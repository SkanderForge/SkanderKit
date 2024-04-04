#ifndef BC2J_PARSER_H
#define BC2J_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "BinaryLexer.h"
#include "BinaryNode.h"
#include "BinaryToken.h"
class BinaryParser {
public:
    explicit BinaryParser(const std::string& input, const std::string& game);
    std::unique_ptr<BinaryNode> parse();

private:
    BinaryLexer lexer;
    BinaryToken currentToken;

    std::unique_ptr<BinaryObjectNode> parseMap();

    std::unique_ptr<BinaryArrayNode> parseArray();

    std::unique_ptr<BinaryNode> parseObject();

    void expect(BinaryToken::TokenType type);

    void getNextToken(const std::string& caller = "");
};

#endif
