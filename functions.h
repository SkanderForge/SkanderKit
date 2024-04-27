#ifndef FUNCTIONS_H // Use a unique identifier for each header file
#define FUNCTIONS_H


#include <string>
#include "../clausewitz2parse//Node.h"
#include "../binary2text//BinaryNode.h"


std::string c2msgpack(const std::string &input);
std::string c2json(const std::string &input);
std::string cfile2json(const std::string &input);


std::shared_ptr<Node> c2ast(const std::string &input);

std::shared_ptr<BinaryNode> b2ast(const std::string &input, const std::string &game);


std::string bmp2geojson(const std::string &input);
std::string bmp2svg(const char* &input);
std::string binary2txt(const std::string &input);

#endif
