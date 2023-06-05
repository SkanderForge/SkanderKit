#pragma once  // This prevents multiple inclusion of this file

#include <string>

std::string c2msgpack(const std::string &input);
std::string c2json(const std::string &input);
std::string bmp2geojson(const std::string &input);
std::string bmp2svg(const std::string &input);
