#ifndef NODE_H
#define NODE_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include "../../lib/msgpack.hpp"
class Node {
public:
    virtual ~Node() = default;
    virtual std::string toJSON() const = 0;
    virtual msgpack::type::variant toMsgPack() const = 0;
    virtual size_t length() const = 0;
};

class ValueNode : public Node {
public:
    std::string value;
    ValueNode(const std::string &val) : value(val) {}
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
};

class ArrayNode : public Node {
public:
    std::vector<std::unique_ptr<Node>> values;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
};

//ObjectNode is anything and the entirety of the data contained within curly braces {}.
class ObjectNode : public Node {
public:
    std::vector<std::pair<std::string, std::unique_ptr<Node>>> children;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
};
class ConditionalNode : public Node {
public:
    std::string conditional;
    std::unique_ptr<Node> value;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
};


#endif
