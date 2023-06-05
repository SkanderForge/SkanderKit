#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <string>

#include "types.h"
#include "SVG.h"


namespace Geom
{
    class Command
    {
    public:
        Command(const dcoord& c) :
            to(c)
        {}

        virtual std::string draw() = 0;
        virtual std::shared_ptr<Command> reverseTo(std::shared_ptr<Command> p_c) = 0;
        virtual ~Command(){};

    public:
        icoord to; // was dcoord
    };

    class MoveTo : public Command
    {
    public:
        MoveTo(const dcoord& c) :
            Command(c)
        {}
        std::string draw() { return "M " + std::to_string(to.x) + " " + std::to_string(to.y);}
        std::shared_ptr<Command>  reverseTo(std::shared_ptr<Command> p_c);
    };

    class LineTo : public Command
    {
    public:
        LineTo(const dcoord& c) :
            Command(c)
        {}
        std::string draw() { return "L " + std::to_string(to.x) + " " + std::to_string(to.y); }
        std::shared_ptr<Command>  reverseTo(std::shared_ptr<Command> p_c);
    };

    class QuadraticBezierTo : public Command
    {
    public:
        QuadraticBezierTo(const dcoord& ai_to, const dcoord& ai_Q) :
            Command(ai_to), Q(ai_Q)
        {}
        std::string draw() { return "Q " + std::to_string(Q.x) + " " + std::to_string(Q.y) + " " + std::to_string(to.x) + " " + std::to_string(to.y); }
        std::shared_ptr<Command>  reverseTo(std::shared_ptr<Command> p_c);
    public:
        dcoord Q;
    };

    using pathGeometry = std::vector<std::shared_ptr<Command>>;
}

#endif
