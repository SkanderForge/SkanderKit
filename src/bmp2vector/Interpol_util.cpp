#include "../bmp2vector/Interpol_util.h"


#ifdef DEBUG_INTERPOL_UTIL
#define DEBUG_INTERPOL_UTIL_PRINT(arg) arg
#else
#define DEBUG_INTERPOL_UTIL_PRINT(arg)  
#endif

namespace Interpol
{


    itValue<double, dcoord> maxDistanceFromSegment(
        const dcoord& A, const dcoord& B,
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd)
    {
        itValue<double, dcoord> max;
        max.it = pointsBegin;
        max.value = 0;

        dcoord u = normalize(B - A);
        dcoord v = orthonormalUnitVector(B - A);

        for (auto  pointsIterator = pointsBegin; pointsIterator != pointsEnd; pointsIterator++)
        {
            //std::cout << "coords pointer by iterator: " << toString(*pointsIterator) << "\n";
            double d;
            double distanceAlongLine = abs(dotProduct(dcoord((*pointsIterator) - A), u));

            if (distanceAlongLine >= 0 && distanceAlongLine <= norm(B - A))
            {
                double distanceToLine = abs(dotProduct(dcoord((*pointsIterator) - A), v));
                d = distanceAlongLine;
                if (distanceToLine > max.value)
                {
                    max.value = distanceToLine;
                    max.it = pointsIterator;
                }
            }
            else
            {
                double distanceToOrigin = norm((*pointsIterator) - A);
                d = distanceToOrigin;
                if (distanceToOrigin > max.value)
                {
                    max.value = distanceToOrigin;
                    max.it = pointsIterator;
                }
            }
            //std::cout << "iteration distance: " << d << "\tmaxValue: "<< max.value <<"\n";
        }
        return max;

    }

    std::vector<dcoord> discretizeSegment(const dcoord A, const dcoord B, uint n)
    {
        DEBUG_INTERPOL_UTIL_PRINT(std::cout << "dS start "<< n <<"\n";)
        std::vector<dcoord> v(n);
        DEBUG_INTERPOL_UTIL_PRINT(std::cout << "size: "<< v.size() << "\n";)
        v.front() = A;
        v.back() = B;

        for (uint i = 1; i < (n - 1); i++)
        {
            double t = double(i) / n;
            v[i] = dcoord(t) * A + dcoord(1 - t) * B;
        }

        DEBUG_INTERPOL_UTIL_PRINT(std::cout << "dS end\n";)
        return v;
    }

    double l1_fit(
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd,
        const std::vector<dcoord>::const_iterator pointsRefBegin,
        const std::vector<dcoord>::const_iterator pointsRefEnd)
    {
        double fit = 0;
        uint n = pointsEnd - pointsBegin;

        for (auto i = 0; i < n; i++)
        {
            fit += norm(dcoord(*(pointsBegin + i)) - dcoord(*(pointsRefBegin + i)));
        }

        fit = fit / n;
        return fit;
    };
}