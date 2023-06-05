#ifndef SVG_H
#define SVG_H

#include <vector>
#include <string>
#include <memory>
#include <fstream>

#include "types.h"
#include "Shape.h"
#include "Command.h"

namespace Geom
{
    class Shape;

    class Svg
    {
    public:
        Svg(coord ai_dim) :
            dim(ai_dim)
        {}


        void writeToFile(std::ofstream& ai_fileStream);
        std::string writeToString();


        void writeShapes(std::ofstream& ai_fileStream);
        void writeHeader(std::ofstream& ai_fileStream);
        void writeFooter(std::ofstream& ai_fileStream);
        void writeShapes(std::stringstream &buffer);
        void writeHeader(std::stringstream &buffer);
        void writeFooter(std::stringstream &buffer);

    public:
        std::vector <std::shared_ptr<Shape>> shapes;
        coord dim;
    };
}

#endif
