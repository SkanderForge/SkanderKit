#ifndef SHAPE_H
#define SHAPE_H


#include <string>
#include <vector>
#include <memory>
#include <fstream>


#include "SVG.h"
#include "Command.h"
#include "types.h"
//#include "Interpol.h"

namespace Geom 
{
    class Command;

    class Shape
    {
    public:
        virtual void draw(std::ofstream& ai_fileStream) = 0;
        virtual void draw(std::stringstream &buffer) = 0;
    };

    class Path : public Shape
    {
    public:
        void draw(std::ofstream& ai_fileStream) override;
        void draw(std::stringstream &buffer) override;

        void addCommand(std::shared_ptr<Command> p_command);

        void append(const Path& p);

        void appendFrom(const Path& p, uint min);

        void addLines(const std::vector<dcoord>& coords);

        void addLines(const std::vector<coord>& coords);

        void addLinesQ(const std::vector<coord>& coords, const std::vector<dcoord>& Qs);

        void addMoveTo(const dcoord& M);

        void addQ(const dcoord& B, const dcoord& Q);

        void addLineTo(const dcoord& B);

        Path reverse();

        //void addQChain(const Interpol::quadraticBezierChain& qChain);

    public:
        std::vector<std::shared_ptr<Command>> commands;
        float strokeWidth;
        Color strokeColor;
        Color fillColor;
    };
}

#endif
