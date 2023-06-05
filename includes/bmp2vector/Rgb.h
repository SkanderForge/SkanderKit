#ifndef RGB_H
#define RGB_H

class Rgb
{
public:
    unsigned int m_red;
    unsigned int m_green;
    unsigned int m_blue;

    unsigned int value;

    Rgb(unsigned int ai_red, unsigned int ai_green, unsigned int ai_blue);

    Rgb() {};
};

const bool operator==(const Rgb &lhs, const Rgb &rhs);

#endif
