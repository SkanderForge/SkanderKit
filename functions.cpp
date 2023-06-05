#include "includes/clausewitz2parse/Lexer.h"
#include "includes/clausewitz2parse/Token.h"
#include "includes/clausewitz2parse/Parser.h"
#include <fstream>
#include "includes/bmp2vector/Entry.h"
#include "functions.h"

std::string c2msgpack(const std::string &input) {
    Lexer lexer("\n{" + input + "\n}");
    Parser parser(lexer);
    std::unique_ptr<Node> ast = parser.parse();
    auto msg = ast->toMsgPack();
    std::stringstream buffer;
    msgpack::pack(buffer, msg);
    return buffer.str();
}
std::string c2json(const std::string &input) {
    Lexer lexer("\n{" + input + "\n}");
    Parser parser(lexer);
    std::unique_ptr<Node> ast = parser.parse();
    std::string json = ast->toJSON();
    return json;
}
std::string bmp2geojson(const std::string &input){
    return makeGeojsonString(input);
}
std::string bmp2svg(const std::string &input){
    return makeSVGstring(input);
}