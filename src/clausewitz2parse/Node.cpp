#include "../../includes/clausewitz2parse/Node.h"
#include <iostream>
#include <cstdlib>

std::string remove_slashes(std::string input) {
    if (input[0] == '"') {
        input.erase(0, 1);
    }
    if (input[input.length() - 1] == '"') {
        input.erase(input.length() - 1, 1);
    }
    return input;
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

    bool isNumeric = true;

    for (char c : value) {
        if (!std::isdigit(c) && c != '-' && c != '.') {
            isNumeric = false;
        }
    }
    if(isNumeric) return value;

    //Some strings in the game are already enclosed in quotation marks, so let's not do it twice.
    if (value.find('"') != std::string::npos) {
        return value;
    }
    return '"' + value + '"';
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

rapidjson::Value ObjectNode::toJSONValue(rapidjson::Document &d) const {
    rapidjson::Value v(rapidjson::kObjectType);
    for (const auto &pair: children) {
        const char* key = pair.first.c_str();
        rapidjson::Value keyValue;
        keyValue.SetString(key, pair.first.length());
        v.AddMember(keyValue,pair.second->toJSONValue(d),d.GetAllocator());
    }
    return v;
}



size_t ObjectNode::length() const {
    return children.size();
}

void ObjectNode::toJSON(rapidjson::Document &d) const {

}
void ValueNode::toJSON(rapidjson::Document &d) const {

}

rapidjson::Value ValueNode::toJSONValue(rapidjson::Document &d) const {
    bool isNumeric = true;
    rapidjson::Value v;

    for (char c : value) {
        if (!std::isdigit(c) && c != '-' && c != '.') {
            isNumeric = false;
        }
    }

    if(isNumeric){
        if(value[value.length()-3] == '.'){
            double temp = strtod(value.c_str(), nullptr);
            v.SetDouble(temp);
            return v;
        }
        long int temp = strtol(value.c_str(),nullptr,10);
        v.SetInt(temp);
        return v;
    }
    std::basic_string<char> withoutQuotes = remove_slashes(value);
    v.SetString(withoutQuotes.c_str(), withoutQuotes.length(), d.GetAllocator());
    return v;
}



void ArrayNode::toJSON(rapidjson::Document &d) const {
    rapidjson::Value arr(rapidjson::kArrayType);
    for (const auto& node : values) {
        rapidjson::Value child(node->toJSONValue(d));
        arr.PushBack(child, d.GetAllocator());
    }
}

rapidjson::Value ArrayNode::toJSONValue(rapidjson::Document &d) const {
    rapidjson::Value v(rapidjson::kArrayType);
    for (const auto& node : values) {
        if(node != nullptr) {
            rapidjson::Value child(node->toJSONValue(d));
            v.PushBack(child, d.GetAllocator());
        }
    }
    return v;
}

std::string ConditionalNode::toJSON() const {
    return "{}";
}
