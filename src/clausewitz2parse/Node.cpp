#include "../../includes/clausewitz2parse/Node.h"
#include <iostream>

std::string remove_slashes(std::string input)
{
    if(input[0]=='"'){
        input.erase(0,1);
    }
    if(input[input.length()-1]=='"'){
        input.erase(input.length()-1,1);
    }
    return input;
}

msgpack::type::variant ValueNode::toMsgPack() const {
    if (std::all_of(value.begin(), value.end(), ::isdigit)) {
        return msgpack::type::variant(std::stoi(value));
    }
    return msgpack::type::variant(std::move(value));
}

msgpack::type::variant ArrayNode::toMsgPack() const {
    std::vector<msgpack::type::variant> packed_values;
    for (const auto &value: values) {
        packed_values.push_back(value->toMsgPack());
    }
    return msgpack::type::variant(std::move(packed_values));
}

msgpack::type::variant ObjectNode::toMsgPack() const {
    std::map<msgpack::type::variant, msgpack::type::variant> packed_map;
    for (const auto &pair: children) {
        packed_map.insert({std::move(msgpack::type::variant(pair.first)), std::move(pair.second->toMsgPack())});
    }
    return msgpack::type::variant(std::move(packed_map));
}

msgpack::type::variant ConditionalNode::toMsgPack() const {
    std::map<std::string, msgpack::type::variant> empty_map;
    return msgpack::type::variant("");
}


std::string ArrayNode::toJSON() const {
    std::string result = "[";
    for (auto &value: values) {
        if (result.size() > 1) result += ",";
        result += value->toJSON();
    }
    result += "]";
    return result;
}

size_t ArrayNode::length() const {
    return values.size();
}

std::string ValueNode::toJSON() const {
    // If the value can be converted to a simple int. Paradox's floats, ie. 123.000
    // will be returned as a string(for now anyways).
    if (std::all_of(value.begin(), value.end(), ::isdigit)) {
        return value;
    }
    else {
        //Some strings in the game are already enclosed in quotation marks, so let's not do it twice.
        if (value.find('"') != -1) {
            return value;
        }
        return '"' + value + '"';
    }
}

size_t ValueNode::length() const {
    return 1;
}

std::string ObjectNode::toJSON() const {
    std::string result = "{";
    for (const auto &pair: children) {
        //We initialize result with length 1. If it's
        // larger than that, it means that the first element has already been added.
        if (result.size() > 1) result += ",";
        //first -> key, second -> value(<Node>)
        result += '"' + remove_slashes(pair.first) + "\":" + pair.second->toJSON();
    }
    result += "}";
    return result;
}

size_t ObjectNode::length() const {
    return children.size();
}

std::string ConditionalNode::toJSON() const {
    return "{}";
}
