#ifndef INTERPOL_UTIL_H
#define INTERPOL_UTIL_H

#include <vector>
#include <iterator>
#include <math.h>

#include "types.h"


namespace Interpol
{
    //functions

    template<typename T>
    inline T dotProduct(const coord2d<T>& lhs, const coord2d<T>& rhs)
    {
        return (rhs.x * lhs.x) + (rhs.y * lhs.y);
    };

    template<typename T>
    inline T squareNorm(const coord2d<T>& c)
    {
        return dotProduct(c, c);
    };

    template<typename T>
    inline double norm(const coord2d<T>& c)
    {
        return sqrt(squareNorm(c));
    };

    template<typename T>
    dcoord normalize(const coord2d<T>& c)
    {
        return dcoord(c) * dcoord(double(1) / norm(c));
    }

    template<typename T>
    dcoord orthonormalUnitVector(const coord2d<T>& c)
    {
        dcoord normalized = normalize(c);
        return dcoord(-normalized.y, normalized.x);
    }



    template<typename T>
    struct idValue
    {
        uint id;
        T value;
    };

    template<typename T, typename U>
    struct itValue
    {
        typename std::vector<U>::const_iterator it;
        T value;
    };

    /* Return max distance from line (B-A) in points
    */
    template<typename T>
    idValue<double> maxDistanceFromLine(const dcoord& A, const dcoord& B, const std::vector<coord2d<T>>& points)
    {
        idValue<double> max;
        max.id = 0;
        max.value = 0;

        dcoord u = normalize(B - A);
        dcoord v = orthonormalUnitVector(B - A);

        for (uint i = 0; i < points.size(); i++)
        {
            double distanceAlongLine = abs(dotProduct(dcoord(points[i] - A), u));

            if (distanceAlongLine >= 0 && distanceAlongLine <= norm(B - A))
            {
                double distanceToLine = abs(dotProduct(dcoord(points[i] - A), v));
                if (distanceToLine > max.value)
                {
                    max.value = distanceToLine;
                    max.id = i;
                }
            }
            else
            {
                double distanceToOrigin = norm(points[i] - A);
                if (distanceToOrigin > max.value)
                {
                    max.value = distanceToOrigin;
                    max.id = i;
                }
            }
        }
        return max;
    };


    itValue<double, dcoord> maxDistanceFromSegment(
        const dcoord& A, const dcoord& B,
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd);
    
    std::vector<dcoord> discretizeSegment(const dcoord A, const dcoord B, uint n);

    double l1_fit(
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd,
        const std::vector<dcoord>::const_iterator pointsRefBegin,
        const std::vector<dcoord>::const_iterator pointsRefEnd);
}

#endif
