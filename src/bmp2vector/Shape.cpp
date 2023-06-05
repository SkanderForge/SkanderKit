#include <iomanip>

#include "bmp2vector/Shape.h"
#include <sstream>

#ifdef DEBUG_SHAPE
#define DEBUG_SHAPE_PRINT(arg) arg
#else
#define DEBUG_SHAPE_PRINT(arg)  
#endif

namespace Geom
{
    void Path::draw(std::ofstream& ai_fileStream)
    {
        std::string s;

        ai_fileStream<< "<path d=\"";
        for (auto c : commands)
        {
            ai_fileStream << c->draw();
            ai_fileStream << " ";
        }
        ai_fileStream << "\" stroke = \"rgb("<< strokeColor.r <<","<<strokeColor.g<<","<<strokeColor.b<<")\" stroke-width = \"" << std::setprecision(3) << strokeWidth << 
            "\" fill = \"rgb(" << fillColor.r << "," << fillColor.g << "," << fillColor.b << ")\" />";
        ai_fileStream << "\n";

    }
    void Path::draw(std::stringstream &buffer)
    {
        std::string s;
        buffer<< "<path d=\"";
        for (auto c : commands)
        {
            buffer << c->draw();
            buffer << " ";
        }
        buffer << "\" stroke = \"rgb("<< strokeColor.r <<","<<strokeColor.g<<","<<strokeColor.b<<")\" stroke-width = \"" << std::setprecision(3) << strokeWidth <<
                      "\" fill = \"rgb(" << fillColor.r << "," << fillColor.g << "," << fillColor.b << ")\" />";
        buffer << "\n";

    }

    void Path::addCommand(std::shared_ptr<Command> p_command)
    {
        commands.push_back(p_command);
    }



    void Path::append(const Path& p)
    {
        uint n = p.commands.size();
        for (uint i = 0; i < n; i++)
        {
            addCommand(p.commands[i]);
        }
    }

    void Path::appendFrom(const Path& p, uint min)
    {
        uint n = p.commands.size();
        for (uint i = min; i < n; i++)
        {
            addCommand(p.commands[i]);
        }
    }

    void Path::addLines(const std::vector<dcoord>& coords)
    {
        if (coords.size() != 0)
        {
            if (commands.size() == 0)
            {
                commands.push_back(std::make_shared<MoveTo>(MoveTo(dcoord(coords[0]))));
            }
            else
            {
                commands.push_back(std::make_shared<LineTo>(LineTo(dcoord(coords[0]))));
            }
            for (uint i = 1; i < coords.size(); i++)
            {
                commands.push_back(std::make_shared<LineTo>(LineTo(dcoord(coords[i]))));
                
            }
        }
    }

    void Path::addLines(const std::vector<coord>& coords)
    {
        if (coords.size() != 0)
        {
            if (commands.size() == 0)
            {
                commands.push_back(std::make_shared<MoveTo>(MoveTo(dcoord(coords[0]))));
            }
            else
            {
                commands.push_back(std::make_shared<LineTo>(LineTo(dcoord(coords[0]))));
            }
            for (uint i = 1; i < coords.size(); i++)
            {
                commands.push_back(std::make_shared<LineTo>(LineTo(dcoord(coords[i]))));

            }
        }
    }


    void Path::addLinesQ(const std::vector<coord>& coords, const std::vector<dcoord>& Qs)
    {
        //hyp: coords.size() = Qs.size() + 1
        //     coords.size() >=2
        if (commands.size() == 0)
        {
            commands.push_back(std::make_shared<MoveTo>(MoveTo(dcoord(coords[0]))));
        }
        else
        {
            commands.push_back(std::make_shared<LineTo>(LineTo(dcoord(coords[0]))));
        }
        for (uint i = 1; i < coords.size(); i++)
        {
            commands.push_back(std::make_shared<QuadraticBezierTo>(QuadraticBezierTo(dcoord(coords[i]), dcoord(Qs[i-1]))));

        }
    }




    void Path::addMoveTo(const dcoord& M)
    {
        commands.push_back(std::make_shared<MoveTo>(M));
    }

    void Path::addQ(const dcoord& B, const dcoord& Q)
    {
        commands.push_back(std::make_shared<QuadraticBezierTo>(B, Q));
    }


    void Path::addLineTo(const dcoord& B)
    {
        commands.push_back(std::make_shared<LineTo>(B));
    }

    Path Path::reverse()
    {
        DEBUG_SHAPE_PRINT(std::cout << "reverse BEGIN, size: "<< commands.size()<<"\n";)
        Path reversePath;
        reversePath.commands.push_back(commands[0]->reverseTo(commands[commands.size() - 1]));
        for (uint i = commands.size() - 1; i > 0; i--)
        {
            reversePath.commands.push_back(commands[i]->reverseTo(commands[i - 1]));
        }
        DEBUG_SHAPE_PRINT(std::cout << "reverse END\n";)
        return reversePath;
    }
}