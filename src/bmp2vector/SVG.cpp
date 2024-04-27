#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "../bmp2vector/SVG.h"

namespace Geom
{
    void Svg::writeToFile(std::ofstream& ai_fileStream)
    {

        ai_fileStream << std::setprecision(5);

        writeHeader(ai_fileStream);

        writeShapes(ai_fileStream);

        writeFooter(ai_fileStream);

    }
    std::string Svg::writeToString() {
        std::stringstream buffer;
        writeHeader(buffer);
        writeShapes(buffer);

        writeFooter(buffer);
        return buffer.str();
    }
    void Svg::writeShapes(std::ofstream& ai_fileStream) {
        std::cout << "SVG::writeShapes\n";
        for (auto cp : shapes)
        {
            cp->draw(ai_fileStream);
            ai_fileStream << "\n";
        }
    }
    void Svg::writeShapes(std::stringstream &buffer) {
        std::cout << "SVG::writeShapes\n";
        for (auto cp : shapes)
        {
            cp->draw(buffer);
            buffer << "\n";
        }
    }
    void Svg::writeHeader(std::ofstream& ai_fileStream)
    {
        ai_fileStream << "<!DOCTYPE html>\n";
        ai_fileStream << "<html>\n";
        ai_fileStream << "<body>\n";
        ai_fileStream << "<svg height=\"" << dim.x << "\" width=\"" << dim.y << "\">";
        //ai_fileStream << "<svg viewBox = '0 0 " << dim.x << " " << dim.y << "' xmlns = 'http://www.w3.org/2000/svg'>";
        ai_fileStream << "\n";
    }
    void Svg::writeHeader(std::stringstream &buffer)
    {
        buffer << "<!DOCTYPE html>\n";
        buffer << "<html>\n";
        buffer << "<body>\n";
        buffer << R"(<svg xmlns="http://www.w3.org/2000/svg" height=")" << dim.x << "\" width=\"" << dim.y << "\">";
        //ai_fileStream << "<svg viewBox = '0 0 " << dim.x << " " << dim.y << "' xmlns = 'http://www.w3.org/2000/svg'>";
        buffer << "\n";
    }
    void Svg::writeFooter(std::ofstream& ai_fileStream)
    {
        ai_fileStream << "</svg>\n";
        ai_fileStream << "</body>\n";
        ai_fileStream << "</html>\n";
    }
    void Svg::writeFooter(std::stringstream &buffer)
    {
        buffer << "</svg>\n";
        buffer << "</body>\n";
        buffer << "</html>\n";
    }
}