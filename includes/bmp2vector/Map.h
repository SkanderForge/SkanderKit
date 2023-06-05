#ifndef MAP_H
#define MAP_H

#include <vector>


#include "EasyBMP.h"

#include "Pixel.h"
#include "Rgb.h"

class MapInitializerEasyBMP;


class Pixel;

class Map {
    friend MapInitializerEasyBMP;
    friend Pixel;


public:
    //Map() {};
    Map(unsigned int ai_height, unsigned int ai_width);

    void initializeWithEasyBMP(const BMP& ai_bmp);

    void setNeighbors();
    void makeBlobs();

private:
    std::vector<std::vector<Pixel>> m_Pixels;
    std::vector<Rgb> m_rgbValues;

    void init_PixelVector(unsigned int ai_width, unsigned int ai_height);

    unsigned int m_height;
    unsigned int m_width;


};

#endif
