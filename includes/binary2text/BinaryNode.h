#ifndef BNODE_H
#define BNODE_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include "../../lib/msgpack.hpp"
class BinaryNode {
public:
    virtual ~BinaryNode() = default;
    virtual std::string toJSON() const = 0;
    virtual msgpack::type::variant toMsgPack() const = 0;
    virtual size_t length() const = 0;
    virtual std::string type() const = 0;
};

class BinaryValueNode : public BinaryNode {
public:
    std::string value;
    BinaryValueNode(const std::string &val) : value(val) {}
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
    std::string type() const override;
};

class BinaryArrayNode : public BinaryNode {
public:
    std::vector<std::unique_ptr<BinaryNode>> values;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
    std::string type() const override;
};

//ObjectNode is anything and the entirety of the data contained within curly braces {}.
class BinaryObjectNode : public BinaryNode {
public:
    std::vector<std::pair<std::string, std::unique_ptr<BinaryNode>>> children;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
    std::string type() const override;
};
class BinaryConditionalNode : public BinaryNode {
public:
    std::string conditional;
    std::unique_ptr<BinaryNode> value;
    size_t length() const override;
    std::string toJSON() const override;
    msgpack::type::variant toMsgPack() const override;
    std::string type() const override;
};


#endif
