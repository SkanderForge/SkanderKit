#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>
#include <stdint.h>
#include <cassert>
#include <iostream>
#include <string>


using uint = uint32_t;


template<typename T>
class coord2d
{
public:

    coord2d(const T& ai_x, const T& ai_y) :
        x(ai_x), y(ai_y)
    {}
    //conversion constructor
    template<typename U>
    coord2d(coord2d<U> ai_c) :
        x(static_cast<T>(ai_c.x)), y(static_cast<T>(ai_c.y))
    {}
    //default constructor
    coord2d() :
        x(0), y(0)
    {}
    //scalar to coord constructor
    coord2d(const T& ai_s) :
        x(ai_s), y(ai_s)
    {}


    const bool operator==(const coord2d<T>& other) const
    {
        return ((x == other.x) && (y == other.y));
    }

    const bool operator!=(const coord2d<T>& other) const
    {
        return !(*this == other);
    }

    const coord2d<T> operator+(const coord2d<T>& other) const
    {
        return coord2d<T>(x + other.x, y + other.y);
    }

    void operator+=(const coord2d<T>& other)
    {
        x += other.x;
        y += other.y;
    }


    const coord2d<T> operator-(const coord2d<T>& other) const
    {
        return coord2d<T>(x - other.x, y - other.y);
    }

    void operator-=(const coord2d<T>& other)
    {
        x -= other.x;
        y -= other.y;
    }


    const coord2d<T> operator*(const coord2d<T>& other) const
    {
        return coord2d<T>(x * other.x, y * other.y);
    }

    void operator*=(const coord2d<T>& other)
    {
        x *= other.x;
        y *= other.y;
    }



public:
    T x;
    T y;
};

template<typename T>
std::string toString(const coord2d<T>& c)
{
    return std::string(std::to_string(c.x) + " " + std::to_string(c.y));
}



using coord = coord2d<uint>;
using icoord = coord2d<int>;
using dcoord = coord2d<double>;
using fcoord = coord2d<float>;



coord toCoord(const std::string& s);




template <typename T>
class TwoDimArray
{
public:

    TwoDimArray() {};
    TwoDimArray(uint w, uint h) { v = std::vector<std::vector<T>>(w, std::vector<T>(h)); }
    TwoDimArray(uint w, uint h, T defaultValue) { v = std::vector<std::vector<T>>(w, std::vector<T>(h, defaultValue)); }


    const T operator[](const coord& c) const
    {
        return v[c.x][c.y];
    }

    T& operator[](const coord& c)
    {
        return v[c.x][c.y];
    }

    const coord size() const
    {
        if (v.size() == 0)
        {
            return coord(0, 0);
        }
        else
        {
            return coord(v.size(), v[0].size());
        }
    }

public:
    std::vector<std::vector<T>> v;
};


enum E_ORIENTATION
{
    E_HORIZONTAL,
    E_VERTICAL
};

enum E_DIRECTION
{
    E_FRONT,
    E_BACK
};

enum E_CONNEX_TYPE
{
    E_PRINCIPAL,
    E_INTERNAL
};

class PixelBorder
{
public:

    PixelBorder() : orientation(E_ORIENTATION::E_HORIZONTAL){};
    PixelBorder(coord c1, coord c2)
    {
        if (c1.x == c2.x && c2.y == c1.y + 1)
        {
            cFront = c2;
            cBack = c1;
            orientation = E_VERTICAL;
        }
        else if (c1.x == c2.x && c1.y == c2.y + 1)
        {
            cFront = c1;
            cBack = c2;
            orientation = E_VERTICAL;
        }
        else if (c1.y == c2.y && c2.x == c1.x + 1)
        {
            cFront = c2;
            cBack = c1;
            orientation = E_HORIZONTAL;
        }
        else if (c1.y == c2.y && c1.x == c2.x + 1)
        {
            cFront = c1;
            cBack = c2;
            orientation = E_HORIZONTAL;
        }
        else
        {
            assert(0 && "tried to access border defined by two non-contiguous points");
        }
    }

    PixelBorder(coord ai_cBack, E_ORIENTATION ai_orientation)
    {
        cBack = ai_cBack;
        orientation = ai_orientation;
        if (orientation == E_HORIZONTAL)
        {
            cFront = coord(cBack.x + 1, cBack.y);
        }
        else
        {
            cFront = coord(cBack.x, cBack.y + 1);
        }
    }

    coord getFront() const { return cFront; }
    coord getBack() const { return cBack; }
    coord get(E_DIRECTION direction) const {
        if (direction == E_BACK) {
            return cBack; 
        }
        else 
        {
            return cFront; 
        }
    }
    const E_ORIENTATION getOrientation() const { return orientation; }

    const bool operator==(const PixelBorder &other) const
    {
        return ((orientation == other.orientation) && (cFront == other.cFront));
    }

    const bool operator !=(const PixelBorder &other) const
    {
        return !(*this == other);
    }

private:
    coord cFront;
    coord cBack;
    E_ORIENTATION orientation;

};

template <typename T>
class PixelBorderArray
{
    //structs
public:



public:
    PixelBorderArray() {};
    // Constructeur pour un array de w*h pixels
    PixelBorderArray(uint w, uint h)
    {
        m_hBorders = TwoDimArray<T>(w, h + 1);
        m_vBorders = TwoDimArray<T>(w + 1, h);
    }
    PixelBorderArray(uint w, uint h, T defaultValue)
    {
        m_hBorders = TwoDimArray<T>(w, h + 1, defaultValue);
        m_vBorders = TwoDimArray<T>(w + 1, h, defaultValue);
    }


    T& operator[](PixelBorder v)
    {
        if (v.getOrientation() == E_HORIZONTAL)
        {
            return m_hBorders[v.getBack()];
        }
        else
        {
            return m_vBorders[v.getBack()];
        }
    }

    const T operator[](PixelBorder v) const
    {
        if (v.getOrientation() == E_HORIZONTAL)
        {
            return m_hBorders[v.getBack()];
        }
        else
        {
            return m_vBorders[v.getBack()];
        }
    }

    
    T& between(coord c1, coord c2)
    {
        return operator[](PixelBorder(c1,c2));
    }

    const T between(coord c1, coord c2) const
    {
        return operator[](PixelBorder(c1, c2));
    }

    T& from(coord c, E_ORIENTATION orientation)
    {
        return operator[](PixelBorder(c, orientation));
    }

    const T from(coord c, E_ORIENTATION orientation) const
    {
        return operator[](PixelBorder(c, orientation));
    }

    const coord hSize() const
    {
        return m_hBorders.size();
    }

    const coord vSize() const 
    {
        return m_vBorders.size();
    }

    std::vector<PixelBorder> getFrontNeighbors(const PixelBorder& v)
    {
        coord c = v.getFront();
        std::vector<PixelBorder> frontNeighbors;
        if (c.x < hSize().x - 1)
        {
            frontNeighbors.push_back(PixelBorder(c, E_HORIZONTAL));
        }
        if (c.y < vSize().y - 1)
        {
            frontNeighbors.push_back(PixelBorder(c, E_VERTICAL));
        }
        if (v.getOrientation() == E_HORIZONTAL)
        {
            if (c.y > 0)
            {
                frontNeighbors.push_back(PixelBorder(c, coord(c.x, c.y - 1)));
            }
        }
        else 
        {
            if (c.x > 0)
            {
                frontNeighbors.push_back(PixelBorder(c, coord(c.x - 1, c.y)));
            }
        }
        std::cout << "frontNeighbors:\n";
        for (auto v : frontNeighbors)
        {
            std::cout << v.getBack().x << "\t" << v.getBack().y << "\t" << v.getOrientation() << "\n";
        }

        return frontNeighbors;
    }

    std::vector<PixelBorder> getBackNeighbors(const PixelBorder& v) 
    {
        coord c = v.getBack();
        std::vector<PixelBorder> backNeighbors;
        if (c.x > 0)
        {
            backNeighbors.push_back(PixelBorder(c, coord(c.x - 1, c.y)));
        }
        if (c.y > 0)
        {
            backNeighbors.push_back(PixelBorder(c, coord(c.x, c.y - 1)));
        }
        if (v.getOrientation() == E_HORIZONTAL)
        {
            backNeighbors.push_back(PixelBorder(c, E_VERTICAL));
        }
        else
        {
            backNeighbors.push_back(PixelBorder(c, E_HORIZONTAL));
        }


        std::cout << "backNeighbors:\n";
        for (auto v : backNeighbors)
        {
            std::cout << v.getBack().x << "\t" << v.getBack().y << "\t" << v.getOrientation() << "\n";
        }
        return backNeighbors;
    }


private:
    TwoDimArray<T> m_hBorders;
    TwoDimArray<T> m_vBorders;
};


std::string toHex(uint u, uint minDigits = 0);

struct Color
{
    uint r;
    uint g;
    uint b;

    std::string hexString()
    {
        std::string hs = "#";
        hs += toHex(r, 2);
        hs += toHex(g, 2);
        hs += toHex(b, 2);

        return hs;
    }
};

#endif
