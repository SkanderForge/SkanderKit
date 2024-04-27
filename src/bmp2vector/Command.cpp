#include <memory>

#include "../bmp2vector/Command.h"


namespace Geom
{
    std::shared_ptr<Command> MoveTo::reverseTo(std::shared_ptr<Command> c_p)
    {
        return std::shared_ptr<Command>(std::make_shared<MoveTo>(c_p->to));
    }


    std::shared_ptr<Command> LineTo::reverseTo(std::shared_ptr<Command> c_p)
    {
        return std::shared_ptr<Command>(std::make_shared<LineTo>(c_p->to));
    }


    std::shared_ptr<Command> QuadraticBezierTo::reverseTo(std::shared_ptr<Command> c_p)
    {
        return std::shared_ptr<Command>(std::make_shared<QuadraticBezierTo>(c_p->to, Q));
    }
}