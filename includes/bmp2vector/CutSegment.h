#ifndef CUT_SEGMENT_H
#define CUT_SEGMENT_H

#include <vector>

#include "types.h"

namespace Interpol
{
    class CutSegment
    {

    public:
        CutSegment(const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end);

        dcoord optimize(const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end);
        const std::vector<dcoord>& getPoints() const;
        const std::vector<dcoord>::const_iterator begin() const
        {
            return _points.begin();
        };
        const std::vector<dcoord>::const_iterator end() const
        {
            return _points.end();
        };
        const dcoord getM() const;

    private:
        dcoord _gradient(const dcoord M, const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end);
        std::vector<dcoord> _points;
        dcoord _M;
    };

}

#endif
