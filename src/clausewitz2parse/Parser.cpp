#include <utility>
#include <iostream>
#include <unordered_set>
#include "../../includes/clausewitz2parse/Lexer.h"

#include "../../includes/clausewitz2parse/Parser.h"
#include "../../includes/clausewitz2parse/BinaryLexer.h"
#include "../../includes/clausewitz2parse/PlainTextLexer.h"

Parser::Parser(const std::string &input) : lexer(std::make_unique<PlainTextLexer>("\n{\n" + input + "\n}\n")), currentToken(lexer->getNextToken()) {}
Parser::Parser(std::stringstream &s, const std::string &game) : lexer(std::make_unique<BinaryLexer>(s,game)), currentToken(lexer->getNextToken()) {}

std::unique_ptr<Node> Parser::parse() {
    return parseObject();
}


std::unique_ptr<ObjectNode> Parser::parseMap() {
    std::unique_ptr<ObjectNode> object = std::make_unique<ObjectNode>();
    std::unordered_set<std::string> existingKeys;
    std::unique_ptr<Node> value;
    std::string key = "key";
    std::unique_ptr<ArrayNode> array = std::make_unique<ArrayNode>();
    while (currentToken.type != Token::TokenType::CLOSE_BRACE && currentToken.type != Token::TokenType::END_OF_FILE) {
//        auto start = std::chrono::high_resolution_clock::now();

        if (currentToken.type == Token::TokenType::IDENTIFIER) {
            key = currentToken.value;
        }
        getNextToken("Start of the loop");

        if (currentToken.type == Token::TokenType::EQUAL) {

            expect(Token::TokenType::EQUAL);
            if (currentToken.type == Token::TokenType::IDENTIFIER) {
                value = std::make_unique<ValueNode>(currentToken.value);
            } else if (currentToken.type == Token::TokenType::OPEN_BRACE) {
                value = parseObject();
            } else if (currentToken.type == Token::TokenType::ENCLOSED_OBJECT) {
                Parser parser(currentToken.value);
                value = parser.parse();
            } else {
                throw std::runtime_error("Expected a value, got " + currentToken.value);
            }
        } else {
            array = parseArray();
            array->values.insert(array->values.begin(), std::make_unique<ValueNode>(key));
        }
        bool exists = existingKeys.find(key) != existingKeys.end();
        //The element DOES exist already
        if (exists) {
            auto found_element = std::find_if(object->children.begin(), object->children.end(),
                                              [&key](const std::pair<std::string, std::unique_ptr<Node>> &lookup) {
                                                  return lookup.first == key;
                                              });

            //<ArrayNode>> derives from <Node>. This cast will only be successful if found_element is already an array node.
            auto *arrayNode = dynamic_cast<ArrayNode *>(found_element->second.get());
            if (!arrayNode) {
                //If the element is NOT an array node, we create it
                //and push the first element into it
                std::unique_ptr<ArrayNode> newArrayNode = std::make_unique<ArrayNode>();
                newArrayNode->values.push_back(std::move(found_element->second));
                found_element->second = std::move(newArrayNode);
                arrayNode = dynamic_cast<ArrayNode *>(found_element->second.get());
            }
            arrayNode->values.push_back(std::move(value));
        } else {
            if (!array->length()) {
                existingKeys.insert(key);
                object->children.emplace_back(key, std::move(value));
            } else {
                object->children.emplace_back("list", std::move(array));
                array = std::make_unique<ArrayNode>();
            }
        }
//        auto end = std::chrono::high_resolution_clock::now();
//        auto duration = duration_cast<std::chrono::microseconds>(end - start);
//        std::cout << "Execution time: " << duration.count() << std::endl;

        getNextToken("Finished while loop");
    }
    return object;
};

//Array is a {} object that doesn't contain any key-value pairs. For instance: color = { 1 1 1 }
std::unique_ptr<ArrayNode> Parser::parseArray() {

    std::unique_ptr<ArrayNode> array = std::make_unique<ArrayNode>();
    while (currentToken.type == Token::TokenType::IDENTIFIER) {
        std::unique_ptr<ValueNode> value = std::make_unique<ValueNode>(currentToken.value);
        if (currentToken.type == Token::TokenType::CLOSE_BRACE) return array;
        expect(Token::TokenType::IDENTIFIER);
        array->values.push_back(std::move(value));
    }
    return array;
}

//Object is anything between curly {} braces. Later on we distinguish between an array { a, b, c} and map {key = value}
std::unique_ptr<Node> Parser::parseObject() {
    auto start = std::chrono::high_resolution_clock::now();
    expect(Token::TokenType::OPEN_BRACE);
    //Looking up 2 tokens ahead allows us to determine what type of object we are dealing with.
    size_t current_position = lexer->getPosition();
    Token lookup = lexer->getNextToken();
    Token lookup_second = lexer->getNextToken();
    lexer->setPosition(current_position);

    if (currentToken.type == Token::TokenType::IDENTIFIER && lookup.type == Token::TokenType::EQUAL) {
        //Normal map, ie. { a = b }
        std::unique_ptr<ObjectNode> obj = std::make_unique<ObjectNode>();
        obj = parseMap();
        return obj;
    } else if (currentToken.type == Token::TokenType::OPEN_BRACE) {
        //Nested array of objects, ie. { { a = b } { b = a } }
        auto node = std::make_unique<ArrayNode>();
        node->values.push_back(parseObject());
        getNextToken("Nested start");
        while (currentToken.type == Token::TokenType::OPEN_BRACE) {
            node->values.push_back(parseObject());
            getNextToken("Nested in");
        }
        return node;
    } else {
        //Array, ie. { 0 1 2 3 }
        std::unique_ptr<Node> array = std::make_unique<ArrayNode>();
        array = parseArray();
        return array;
    }
}

void Parser::expect(Token::TokenType type) {
    if (currentToken.type == type) {
        getNextToken("Expecter");
    } else {
        std::cout << "\n\n\n\n";
        std::exit(1);
    }
}

void Parser::getNextToken(const std::string &caller) {
    currentToken = lexer->getNextToken();
}