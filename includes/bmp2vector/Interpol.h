#ifndef INTERPOL_H
#define INTERPOL_H


#include <vector>

#include "types.h"
#include "Shape.h"

#include "Interpol_util.h"


namespace Interpol
{


    static uint interpolRecursionCount;
    //types
    class quadraticBezierChain {
    public:
        void append(const quadraticBezierChain& qChain);
    public:
        std::vector<dcoord> points;
        std::vector<dcoord> Qpoints;
    };

    double bezierDiff(const std::vector<dcoord>& P, const std::vector<icoord>& points);

    //dcoord bezierGrad(const dcoord& Q, const std::vector<double>& varCoeff, const std::vector<dcoord>& cFixe);
    dcoord bezierGrad( const std::vector<dcoord>& P, const std::vector<icoord>& points);

    dcoord bezier(const std::vector<icoord>& points, double& d);

    double smoothFilter(double distance);

    void line(const std::vector<icoord>& points, double& d);

    template<typename T>
    dcoord normalize(const coord2d<T>& c);

    template<typename T>
    dcoord orthonormalUnitVector(const coord2d<T>& c);

    template<typename T>
    uint maxDistanceFromLineId(const dcoord& A, const dcoord& B, const std::vector <coord2d<T>>& points);

    template<typename T>
    idValue<double> maxDistanceFromLine(const dcoord& A, const dcoord& B, const std::vector <coord2d<T>>& points);

    double smoothFactor(const uint length);
    
    Geom::Path interpolLoop(const std::vector<icoord>& points);

    Geom::Path interpol_sub(const std::vector<icoord>& points);

    Geom::Path interpol(const std::vector<icoord>& points);

    enum LoisSubStatus
    {
        E_STOP_USE_GRADIENT,
        E_CONTINUE_USE_POINTS
    };

    struct LoisSubStruct
    {
        bool status;
        dcoord M;
        std::vector<dcoord> points;
    };

    LoisSubStruct localOptimumInterpolationStrategy_sub(
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd,
        double curFit, bool isFirstCall);

    std::vector<dcoord> localOptimumInterpolationStrategy(const std::vector<dcoord>& points);
}

#endif
