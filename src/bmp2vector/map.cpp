#include <vector>
#include <utility>

#include "../bmp2vector/EasyBMP.h"
#include "../bmp2vector/Map.h"
#include "../bmp2vector/types.h"


Map::Map(unsigned int ai_width, unsigned int ai_height)
{
    init_PixelVector(ai_width, ai_height);
}

void Map::init_PixelVector(unsigned int ai_width, unsigned int ai_height)
{

    m_height = ai_height;
    m_width = ai_width;

    std::cout << "in init_PixelVector:\n";
    std::cout << "ai_width " << ai_width << ", ai_height " << ai_height << "\n";

    std::vector<Pixel> line(ai_height);

    for (unsigned int w_i = 0; w_i < ai_width; w_i++)
    {
        m_Pixels.push_back(line);
    }

}

void Map::initializeWithEasyBMP(const BMP& ai_bmp)
{
    unsigned int w_height = ai_bmp.TellHeight();
    unsigned int w_width = ai_bmp.TellWidth();
    m_rgbValues.clear();

    init_PixelVector(w_width, w_height);

    std::cout << m_Pixels.size() << " " << m_Pixels[0].size() << "\n";
    std::cout << w_width << " " << w_height << "\n";

    for (unsigned int w_w = 0; w_w < w_width; w_w++)
    {
        for (unsigned int w_h = 0; w_h < w_height; w_h++)
        {
            m_Pixels[w_w][w_h].parent = nullptr;
            m_Pixels[w_w][w_h].mp_map = this;
            Rgb w_rgb(ai_bmp.GetPixel(w_w, w_h).Red, 
                      ai_bmp.GetPixel(w_w, w_h).Green,
                      ai_bmp.GetPixel(w_w, w_h).Blue);

            //std::cout << w_w << " " << w_h << "\n";
            m_Pixels[w_w][w_h].m_rgb = w_rgb;
        }
    }

}
void Map::setNeighbors() {
    /*
    //top left
    m_Pixels[0][0].neighbors.push_back(coord(1, 0));
    m_Pixels[0][0].neighbors.push_back(coord(0, 1));

    //top right
    m_Pixels[m_width - 1][0].neighbors.push_back(coord(m_width - 1, 1));
    m_Pixels[m_width - 1][0].neighbors.push_back(coord(m_width - 2, 0));

    //bottom left
    m_Pixels[0][m_height - 1].neighbors.push_back(coord(0, m_height - 2));
    m_Pixels[0][m_height - 1].neighbors.push_back(coord(1, m_height - 1));

    //bottom right
    m_Pixels[m_width - 1][m_height - 1].neighbors.push_back(coord(m_width - 1, m_height - 2));
    m_Pixels[m_width - 1][m_height - 1].neighbors.push_back(coord(m_width - 2, m_height - 1));*/




    for (int w_w = 0; w_w < m_width - 1; w_w++)
    {
        for (int w_h = 0; w_h < m_height; w_h++)
        {
            m_Pixels[w_w][w_h].neighbors.push_back(coord(w_w + 1, w_h));
        }
    }


    for (int w_w = 1; w_w < m_width ; w_w++)
    {
        for (int w_h = 0; w_h < m_height; w_h++)
        {
            m_Pixels[w_w][w_h].neighbors.push_back(coord(w_w - 1, w_h));
        }
    }


    for (int w_w = 0; w_w < m_width; w_w++)
    {
        for (int w_h = 0; w_h < m_height -1 ; w_h++)
        {
            m_Pixels[w_w][w_h].neighbors.push_back(coord(w_w, w_h + 1));
        }
    }

    for (int w_w = 0; w_w < m_width; w_w++)
    {
        for (int w_h = 1; w_h < m_height; w_h++)
        {
            m_Pixels[w_w][w_h].neighbors.push_back(coord(w_w, w_h - 1));
        }
    }
}

 
void Map::makeBlobs() {
}