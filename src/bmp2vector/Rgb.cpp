#include <math.h>
#include "bmp2vector/Rgb.h"
#include "bmp2vector/types.h"

Rgb::Rgb(uint ai_red, uint ai_green, uint ai_blue)
{
    value = pow(2, 16) * ai_red + pow(2, 8) * ai_green + ai_blue;
}

const bool operator==(const Rgb &lhs, const Rgb &rhs)
{
    return (lhs.value == rhs.value);
}
