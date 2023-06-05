#ifndef SKANDIKIT_ENTRY_H
#define SKANDIKIT_ENTRY_H

#include <string>
#include <iostream>


void makeSVG(const std::string &inFile, const std::string &outFile, bool drawRef);

std::string makeGeojsonString(const std::string& input);
std::string makeSVGstring(const std::string& input);

#endif //SKANDIKIT_ENTRY_H
