#ifndef C2J_PARSER_H
#define C2J_PARSER_H

#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "Node.h"
#include "Token.h"
class Parser {
public:
    explicit Parser(const std::string& input);
    std::unique_ptr<Node> parse();

private:
    Lexer lexer;
    Token currentToken;

    std::unique_ptr<ObjectNode> parseMap();

    std::unique_ptr<ArrayNode> parseArray();

    std::unique_ptr<Node> parseObject();

    void expect(Token::TokenType type);

    void getNextToken(const std::string& caller = "");
};

#endif
