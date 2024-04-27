#include <math.h>
#include <algorithm>
#include <stdlib.h>

#include "../bmp2vector/Interpol.h"
#include "../bmp2vector/Interpol_util.h"
#include "../bmp2vector/Shape.h"
#include "../bmp2vector/CutSegment.h"

namespace Interpol
{



    void quadraticBezierChain::append(const quadraticBezierChain& qChain)
    {
        for (auto& c : qChain.points)
        {
            points.push_back(c);
        }
        for (auto& c : qChain.Qpoints)
        {
            Qpoints.push_back(c);
        }
    }


    double bezierDiff(const std::vector<dcoord>& P, const std::vector<icoord>& points)
    {
        double d = 0;
        
        uint n = points.size();

        for (uint i = 0; i < n; i++)
        {
            double deltaNorm = norm(P[i] - dcoord(points[i]));
            d += pow(deltaNorm,2);

        }
        return d;
    }

    dcoord bezierGrad(const std::vector<dcoord>& P, const std::vector<icoord>& points)
    {
        uint n = P.size();
        dcoord gradientQ(0);

        for (uint i = 0; i < n; i++)
        {
            gradientQ += dcoord(2) * ((P[i] - points[i]));
        }
        return gradientQ;

    }

    dcoord bezier(const std::vector<icoord>& points, double& d)
    {

        uint n = points.size();

        dcoord A = points[0];
        dcoord B = points[points.size() - 1];

        //init de Q, � voir si on tente autre chose 
        dcoord Q = (A + B) * dcoord(0.5);

        if (n <= 2)
        {
            d = 0;
            return Q;
        }
        
        if (points[0] == points[points.size() - 1])
        {
            d = 0;
            return Q;
        }

        std::vector<double> t(n);
        std::vector<double> aCoeff(n);
        std::vector<double> bCoeff(n);
        std::vector<double> varCoeff(n);

        std::vector<dcoord> P(n);
        std::vector<dcoord> Pfixe(n);
        std::vector<dcoord> Pvar(n);
        std::vector<dcoord> cFixe(n);

        //Valeurs � calculer une seule fois


        for (uint i = 0; i < n; i++)
        {
            t[i] = static_cast<double>(i) / (n - 1);
            aCoeff[i] = pow(1 - t[i], 2);
            bCoeff[i] = pow(t[i], 2);
            varCoeff[i] = 2 * t[i] * (1 - t[i]);

            Pfixe[i] = (dcoord(aCoeff[i]) * A) + (dcoord(bCoeff[i]) * B);

            cFixe[i] = points[i] - Pfixe[i];
        }

        double gradThreshold = 0.0001;
        dcoord gradientQ;

        do
        {

            for (uint i = 0; i < n; i++)
            {
                Pvar[i] = dcoord(varCoeff[i]) * Q;
                P[i] = Pvar[i] + Pfixe[i];


            }

            gradientQ = bezierGrad(P, points);
            Q -= (gradientQ * dcoord(double(1) / (n - 2)));


            d = bezierDiff(P, points) / (n-2);

        } 
        while (norm(gradientQ) > gradThreshold);
        return Q;

    }

    double smoothFilter(double distance)
    {
        double d;
        d = std::max(double(0), distance - 0.4);
        return d * d * d;
    }

    void line(const std::vector<icoord>& points, double& d)
    {

        dcoord A(points[0]);
        dcoord B(points.back());
        auto maxDistanceStruct = maxDistanceFromLine(A, B, points);
        dcoord u = normalize(B - A);
        dcoord v = orthonormalUnitVector(B - A);
        double distanceToLineSum = 0;


        for (uint i = 0; i < points.size(); i++)
        {
            double distanceAlongLine = abs(dotProduct(dcoord(points[i] - A), u));

            if (distanceAlongLine >= 0 && distanceAlongLine <= norm(B - A))
            {
                double distanceToLine = abs(dotProduct(dcoord(points[i] - A), v));
                distanceToLineSum += smoothFilter(distanceToLine);
            }
            else
            {
                double distanceToOrigin = norm(points[i] - A);
                distanceToLineSum += smoothFilter(distanceToOrigin);
            }
        }

        d = distanceToLineSum/points.size();
    }




    template<typename T>
    uint maxDistanceFromLineId(const dcoord& A, const dcoord& B, const std::vector<coord2d<T>>& points)
    {
        double maxDistance = 0;
        uint maxDistanceId = 0;
        dcoord u = orthonormalUnitVector(B-A);

        for (uint i = 0; i < points.size(); i++)
        {
            double distanceToLine = abs(dotProduct(dcoord(points[i] - A), u));
            if (distanceToLine > maxDistance)
            {
                maxDistance = distanceToLine;
                maxDistanceId = i;
            }
        }
        return maxDistanceId;

    }






    double smoothFactor(const uint length)
    {
        if (length <= 10)
        {
            return 1;
        }
        else if (length <= 40)
        {
            return ((double(length) - 10) / 10);
        }
        else
        {
            return 4;
        }
    }
    
    
    Geom::Path interpolLoop(const std::vector<icoord>& points)
    {
        Geom::Path qChain;

        uint maxDistance = 0;
        uint maxDistanceId = 0;

        for (uint i = 1; i < points.size() - 1; i++)
        {
            double n = norm(points[0] - points[i]);
            if (maxDistance < n )
            {
                maxDistance = n;
                maxDistanceId = i;
            }
        }

        dcoord A = points[0];
        dcoord B = points[maxDistanceId];

        // point furthest away from [AB]
        auto d = maxDistanceFromLine(A, B, points);
        uint p1, p2;
        if (d.id < maxDistanceId)
        {
            p1 = d.id;
            p2 = maxDistanceId;
        }
        else
        {
            p1 = maxDistanceId;
            p2 = d.id;
        }


        std::vector<icoord> v1;
        for (uint i = 0; i <= p1; i++)
        {
            v1.emplace_back(points[i]);
        }

        std::vector<icoord> v2;
        for (uint i = p1; i <= p2; i++)
        {
            v2.emplace_back(points[i]);
        }

        std::vector<icoord> v3;
        for (uint i = p2; i <= points.size()-1; i++)
        {
            v3.emplace_back(points[i]);
        }
        

        qChain.append(interpol_sub(v1));
        qChain.append(interpol_sub(v2));
        qChain.append(interpol_sub(v3));


        return qChain;
    }



    Geom::Path interpol_sub(const std::vector<icoord>& points)
    {
        interpolRecursionCount++;
        if (interpolRecursionCount > 20)
        {
            std::cout << "INTERPOLATION MAX\t"<<interpolRecursionCount<<"\n";
        }

        Geom::Path p;

        if (points.size() > 2 && points[0] == points[points.size() - 1])
        {
            p.append(interpolLoop(points));
        }
        else
        {

            
            //system("PAUSE");
            bool accepted = 1;
            std::shared_ptr<Geom::Command> newCommand;
            double redoThresholdLine = 0.08 * smoothFactor(points.size());
            double redoThresholdBezier = 0.3;
            double overwriteThresholdBezier = 0.1;
            double overwriteThresholdLine = 0.05;

            double dLine, dBezier; 


            //dcoord Q = bezier(points, d);
            line(points, dLine);
            dcoord Q = bezier(points, dBezier);

            if(dLine <= redoThresholdLine)
            {
                newCommand = std::make_shared<Geom::LineTo>(points.back());
            }
            else
            {
                accepted = 0;
            }



            if (!accepted)
            {
                //search point furthest away from the line
                dcoord A(points[0]);
                dcoord B(points[points.size() - 1]);
                uint maxDistanceId = maxDistanceFromLineId(A, B, points);


                std::vector<icoord> points1;
                for (uint i = 0; i <= maxDistanceId; i++) { points1.push_back(points[i]); }
                std::vector<icoord> points2;
                for (uint i = maxDistanceId; i < points.size(); i++) { points2.push_back(points[i]); }

                Geom::Path p1 = interpol_sub(points1);
                Geom::Path p2 = interpol_sub(points2);

                p.append(p1);
                p.append(p2);

            }
            else
            {
                //p.addQ(points[points.size() - 1], Q);
                p.addCommand(newCommand);
            }

        }
        interpolRecursionCount--;
        return p;
    }


    Geom::Path interpol(const std::vector<icoord>& points)
    {
        Geom::Path p;

        if (points.size() > 0)
        {
            p.addMoveTo(points[0]);
        }

        p.append(interpol_sub(points));

        return p;
    }


    LoisSubStruct localOptimumInterpolationStrategy_sub(
        const std::vector<dcoord>::const_iterator pointsBegin,
        const std::vector<dcoord>::const_iterator pointsEnd,
        double curFit, bool isFirstCall)
    {
        static uint subDepth = 0;
        subDepth++;
        std::cout << "localOptimumInterpolationStrategy_sub BEGIN"<< subDepth<<"\n";
        static const double S_MIN_FIT_DELTA = 0.1;
        LoisSubStruct l;

        uint n = pointsEnd - pointsBegin;

        std::cout << "size of subsection: " << n << "\n";
        if (n < 2)
        {
            std::cout << "too small\n";
            l.status = false;
        }
        else
        {

        

            CutSegment cs(pointsBegin, pointsEnd);

            dcoord A = *pointsBegin;
            dcoord B = *(pointsEnd - 1);

            std::vector<dcoord> discretizedSegment = discretizeSegment(A, B, n);
            std::cout << "discretizeSegment OK\n";
            double newFit = l1_fit(
                discretizedSegment.begin(), discretizedSegment.end(),
                pointsBegin, pointsEnd);

            std::cout << "l1_fit OK\n";

            if (curFit - newFit < S_MIN_FIT_DELTA)
            {
                // it is worth going forward
                std::cout << "it is worth going forward\n";
                l.status = true;
                l.points.push_back(A);

                itValue<double, dcoord> segmentCutParams = maxDistanceFromSegment(
                    A, B,
                    pointsBegin, pointsEnd);
                std::cout << "maxDistance: " << segmentCutParams.value << "\n";

                if (pointsBegin == segmentCutParams.it || segmentCutParams.it == pointsEnd)
                {
                    std::cout << "most distant point to segment is either start or end, return true + M\n"; //NB shouldn't be possible with fitness criteria
                    l.points.push_back(cs.getM());
                }
                else
                {

                    // call sub with first half
                    std::cout << "call sub with first half\n";
                    auto r1 = localOptimumInterpolationStrategy_sub(pointsBegin, segmentCutParams.it, newFit, false);
                    std::cout << "call sub with first half DONE\n";

                    // call sub with second half
                    std::cout << "call sub with second half\n";
                    auto r2 = localOptimumInterpolationStrategy_sub(segmentCutParams.it, pointsEnd, newFit, false);
                    std::cout << "call sub with second half DONE\n";


                    // if both return false, disregard subs, return true + M
                    if (r1.status == false && r2.status == false)
                    {
                        std::cout << "both return false, disregard subs, return true + M\n";
                        l.points.push_back(cs.getM());
                    }
                    else
                    {
                        if (r1.status == true)
                        {
                            std::cout << "r1 returns true\n";
                            for (uint i = 1; i < r1.points.size() - 1; i++)
                            {
                                l.points.push_back(r1.points[i]);
                            }
                        }
                        if (r2.status == true)
                        {
                            std::cout << "r2 returns true\n";
                            for (uint i = 1; i < r2.points.size() - 1; i++)
                            {
                                l.points.push_back(r2.points[i]);
                            }
                        }
                    }
                }

                l.points.push_back(B);
            }
            else
            {
                // it is not worth going forward
                std::wcout << "it is not worth going forward\n";
                l.status = false;
                l.M = cs.getM();
            }
        }

        std::cout << "localOptimumInterpolationStrategy_sub END\n";
        if (subDepth > 5)
        {
            system("PAUSE");
        }

        subDepth--;
        return l;
    }

    std::vector<dcoord> localOptimumInterpolationStrategy(const std::vector<dcoord>& points)
    {
        //compute fit for straight line and pass it to _sub
        //double lineFit = 1;
        
        auto pointsBegin = points.begin();
        auto pointsEnd = points.end();

        dcoord A = *pointsBegin;
        dcoord B = *(pointsEnd - 1);
        uint n = pointsEnd - pointsBegin;

        std::vector<dcoord> discretizedSegment = discretizeSegment(A, B, n);
        std::cout << "discretizeSegment OK\n";
        double lineFit = l1_fit(
            discretizedSegment.begin(), discretizedSegment.end(),
            pointsBegin, pointsEnd);

        auto v = localOptimumInterpolationStrategy_sub(points.begin(), points.end(), lineFit, true).points;
        std::cout <<"localOptimumInterpolationStrategy END, size: "<< v.size() << "\n";
        return v;
    }


    
}
