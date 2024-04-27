#include "../../includes/binary2text/BinaryNode.h"
#include <iostream>


std::string BinaryValueNode::type() const {
    return "BinaryValueNode";
}

std::string BinaryArrayNode::type() const {
    return "BinaryArrayNode";
}

std::string BinaryObjectNode::type() const {
    return "BinaryObjectNode";
}

std::string BinaryConditionalNode::type() const {
    return "BinaryConditionalNode";
}
//
//msgpack::type::variant BinaryValueNode::toMsgPack() const {
//    if (std::all_of(value.begin(), value.end(), ::isdigit)) {
//        return msgpack::type::variant(std::stoi(value));
//    }
//    return msgpack::type::variant(std::move(value));
//}
//
//msgpack::type::variant BinaryArrayNode::toMsgPack() const {
//    std::vector<msgpack::type::variant> packed_values;
//    for (const auto &value: values) {
//        packed_values.push_back(value->toMsgPack());
//    }
//    return msgpack::type::variant(std::move(packed_values));
//}
//
//msgpack::type::variant BinaryObjectNode::toMsgPack() const {
//    std::map<msgpack::type::variant, msgpack::type::variant> packed_map;
//    for (const auto &pair: children) {
//        packed_map.insert({std::move(msgpack::type::variant(pair.first)), std::move(pair.second->toMsgPack())});
//    }
//    return msgpack::type::variant(std::move(packed_map));
//}
//
//msgpack::type::variant BinaryConditionalNode::toMsgPack() const {
//    std::map<std::string, msgpack::type::variant> empty_map;
//    return msgpack::type::variant("");
//}


std::string BinaryArrayNode::toJSON() const {
    std::string result = "[";
    for (auto &value: values) {
        // std::cout << "Array val " << value->length() << std::endl;

        if (value == nullptr) {
            //std::cout << "Null pointer encountered!\n";
            continue; // Or handle the error differently
        }


        if (result.size() > 1) result += ",";
        result += value->toJSON();
    }
    result += "]";
    return result;
}

size_t BinaryArrayNode::length() const {
    return values.size();
}


std::string BinaryValueNode::toJSON() const {
    // If the value can be converted to a simple int. Paradox's floats, ie. 123.000
    // will be returned as a string(for now anyways).
    if (std::all_of(value.begin(), value.end(), ::isdigit)) {
        return value;
    }
    uint8_t i = 0;
    uint8_t required = value.length();
    if(std::count(value.begin(),value.end(), '.') == 1) required--;
    if(value[0] == '-') required--;

    for (char ch: value) {
        if (isdigit(ch)) {
            i++;
        }
    }
    if(i == required){
        return value;
    }

    //Some strings in the game are already enclosed in quotation marks, so let's not do it twice.
    if (value.find('"') != std::string::npos) {
        return value;
    }
    return '"' + value + '"';
}

size_t BinaryValueNode::length() const {
    return 1;
}

std::string BinaryObjectNode::toJSON() const {
    std::string result = "{";
    for (const auto &pair: children) {
        //We initialize result with length 1. If it's
        // larger than that, it means that the first element has already been added.
        if (result.size() > 1) result += ",";
        //first -> key, second -> value(<Node>)
        result += '"' + pair.first + "\":" + pair.second->toJSON();
    }
    result += "}";
    return result;
}

size_t BinaryObjectNode::length() const {
    return children.size();
}

std::string BinaryConditionalNode::toJSON() const {
    return "{}";
}
