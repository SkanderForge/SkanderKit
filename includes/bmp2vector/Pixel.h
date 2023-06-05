#ifndef PIXEL_H
#define PIXEL_H


#include <vector>

#include "Rgb.h"
#include "Map.h"
#include "types.h"

class Map;

class Pixel {
    friend Map;
public:
    Pixel() {
        neighbors.reserve(4);
    };

    Rgb m_rgb;

    unsigned int x;
    unsigned int y;
    coord m_coord;

    Pixel* parent;

    std::vector<coord> neighbors;




public:
    unsigned int red();
    unsigned int green();
    unsigned int blue();
    
private:
    Map* mp_map;
};

#endif
