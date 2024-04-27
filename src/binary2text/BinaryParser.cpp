#include <utility>
#include <iostream>
#include "../../includes/binary2text/BinaryParser.h"
#include <unordered_set>
#include <unordered_map>

BinaryParser::BinaryParser(const std::string &input, const std::string &game) : lexer(std::stringstream(input), game),
                                                                                currentToken(lexer.getNextToken()) {
}

std::unique_ptr<BinaryNode> BinaryParser::parse() {
    return parseObject();
}


std::unique_ptr<BinaryObjectNode> BinaryParser::parseMap() {
    std::unique_ptr<BinaryObjectNode> object = std::make_unique<BinaryObjectNode>();
    std::unique_ptr<BinaryNode> value;
    std::string key = "key";
    uint16_t keyCode;
    std::unordered_set<std::string> existingKeys;
    std::unordered_map<std::string, int> key_usage_count;
    std::unique_ptr<BinaryArrayNode> array = std::make_unique<BinaryArrayNode>();
    //std::cout << currentToken.getTypeString() << std::endl;

    while (currentToken.type != BinaryToken::TokenType::CLOSE_BRACE &&
           currentToken.type != BinaryToken::TokenType::END_OF_FILE) {
        if (currentToken.type == BinaryToken::TokenType::IDENTIFIER) {
            key = currentToken.value;
            keyCode = currentToken.code;
        }
        getNextToken("Start of the loop");

        if (currentToken.type == BinaryToken::TokenType::EQUAL) {
            lexer.pushToParents(keyCode);
            expect(BinaryToken::TokenType::EQUAL);
            if (currentToken.type == BinaryToken::TokenType::IDENTIFIER) {
                value = std::make_unique<BinaryValueNode>(currentToken.value);
                lexer.popParent();
            } else if (currentToken.type == BinaryToken::TokenType::OPEN_BRACE) {
                value = parseObject();
                lexer.popParent();
            } else if (currentToken.type == BinaryToken::TokenType::ENCLOSED_OBJECT) {
            } else {
                throw std::runtime_error("Expected a value, got " + currentToken.value);
            }
        } else {
            lexer.pushToParents(keyCode);
            array = parseArray();
            lexer.popParent();
            array->values.insert(array->values.begin(), std::make_unique<BinaryValueNode>(key));
        }

        bool exists = existingKeys.find(key) != existingKeys.end();



        //The element DOES exist already
        if (exists) {
            //<ArrayNode> derives from <Node>. This cast will only be successful if found_element is already an array node.

            auto found_element = std::find_if(object->children.begin(), object->children.end(),
                                              [&key](const std::pair<std::string, std::unique_ptr<BinaryNode>> &lookup) {
                                                  return lookup.first == key;
                                              });

            auto *arrayNode = dynamic_cast<BinaryArrayNode *>(found_element->second.get());
            if (!arrayNode) {
                //If the element is NOT an array node, we create it
                //and push the first element into it
                std::unique_ptr<BinaryArrayNode> newArrayNode = std::make_unique<BinaryArrayNode>();
                newArrayNode->values.push_back(std::move(found_element->second));
                found_element->second = std::move(newArrayNode);
                arrayNode = dynamic_cast<BinaryArrayNode *>(found_element->second.get());
            }
            arrayNode->values.push_back(std::move(value));
        } else {
            if (!array->length()) {
                object->children.emplace_back(key, std::move(value));
            } else {
                object->children.emplace_back("list", std::move(array));
                array = std::make_unique<BinaryArrayNode>();
            }
        }
        getNextToken("Finished while loop");
    }
    return object;
};

//Array is a {} object that doesn't contain any key-value pairs. For instance: color = { 1 1 1 }
std::unique_ptr<BinaryArrayNode> BinaryParser::parseArray() {
    std::unique_ptr<BinaryArrayNode> array = std::make_unique<BinaryArrayNode>();
    while (currentToken.type == BinaryToken::TokenType::IDENTIFIER) {
        std::unique_ptr<BinaryValueNode> value = std::make_unique<BinaryValueNode>(currentToken.value);
        if (currentToken.type == BinaryToken::TokenType::CLOSE_BRACE) return array;
        expect(BinaryToken::TokenType::IDENTIFIER);
        array->values.push_back(std::move(value));
    }
    return array;
}

//Object is anything between curly {} braces. Later on we distinguish between an array { a, b, c} and map {key = value}
std::unique_ptr<BinaryNode> BinaryParser::parseObject() {
    expect(BinaryToken::TokenType::OPEN_BRACE);
    //Looking up 2 tokens ahead allows us to determine what type of object we are dealing with.
    auto current_position = lexer.getPosition();
    BinaryToken lookup = lexer.getNextToken();
    BinaryToken lookup_second = lexer.getNextToken();
    lexer.setPosition(current_position);

//    std::cout << currentToken.getTypeString() << " " << lookup.getTypeString() << " " << lookup_second.getTypeString()<< std::endl;


    std::unique_ptr<BinaryObjectNode> obj = std::make_unique<BinaryObjectNode>();

    if (currentToken.type == BinaryToken::TokenType::IDENTIFIER && lookup.type == BinaryToken::TokenType::EQUAL) {
        //Normal map, ie. { a = b }
        obj = parseMap();
        return obj;
    }
    if (currentToken.type == BinaryToken::TokenType::OPEN_BRACE) {
        //Nested array of objects, ie. { { a = b } { b = a } }
        auto node = std::make_unique<BinaryArrayNode>();
        node->values.push_back(parseObject());
        getNextToken("Nested start");
        while (currentToken.type == BinaryToken::TokenType::OPEN_BRACE) {
            node->values.push_back(parseObject());
            getNextToken("Nested in");
        }
        return node;
    }

    //Array, ie. { 0 1 2 3 }
    std::unique_ptr<BinaryNode> array = std::make_unique<BinaryArrayNode>();
    array = parseArray();
    return array;
}

void BinaryParser::expect(BinaryToken::TokenType type) {
    if (currentToken.type == type) {
        getNextToken("Expecter");
    } else {
        std::cout << "\n\n\n\n" << lexer.debug_string;
        std::exit(1);
    }
}

void BinaryParser::getNextToken(const std::string &caller) {
    //   std::cout << caller << std::endl;
    currentToken = lexer.getNextToken();
    //   std::cout << "New token " <<currentToken.getTypeString() << " " << currentToken.getValue() << std::endl;
}
