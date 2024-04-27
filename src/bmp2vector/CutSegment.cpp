#include "../bmp2vector/CutSegment.h"
//#include "Interpol.h"
#include "../bmp2vector/Interpol_util.h"


#ifdef DEBUG_CUTSEGMENT
#define DEBUG_CUTSEGMENT_PRINT(arg) arg
#else
#define DEBUG_CUTSEGMENT_PRINT(arg)  
#endif

namespace Interpol
{
    CutSegment::CutSegment(const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end)
    {
        DEBUG_CUTSEGMENT_PRINT(std::cout << "constructor BEGIN\n");
        optimize(begin, end);
        DEBUG_CUTSEGMENT_PRINT(std::cout << "constructor END\n");
    }

    dcoord CutSegment::optimize(const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end)
    {
        DEBUG_CUTSEGMENT_PRINT(std::cout << "optimize BEGIN\n");
        uint n = end - begin;
        DEBUG_CUTSEGMENT_PRINT(std::cout << "size of subsection: " << n << "\n");
        dcoord A = *begin;
        dcoord B = *(end-1);

        _M = dcoord(A + B) * dcoord(0.5);

        dcoord G;
        double toleranceFactor = 1;
        do
        {
            G = _gradient(_M, begin, end);
            _M -= G;

            toleranceFactor *= 1.01;
            DEBUG_CUTSEGMENT_PRINT(std::cout << "M: " << toString(_M) << "\tnorm(G) = "<< norm(G)<<"\n");
        } while (norm(G) > toleranceFactor * 0.0001);

        //M -= lineToGrad(M, points);

        DEBUG_CUTSEGMENT_PRINT(std::cout << "M: " << toString(_M) << "\n";
        std::cout << "optimize END\n");
        return _M;

    }


    dcoord CutSegment::_gradient(const dcoord M, const std::vector<dcoord>::const_iterator begin, const std::vector<dcoord>::const_iterator end)
    {
        dcoord G(0);
        double n = end - begin;

        if (n <= 2)
        {
            return G;
        }
        dcoord A = *begin;
        dcoord B = *(end-1);

        double a = norm(M - A);
        double b = norm(B - M);

        double q = a / (a + b);

        _points.clear();
        _points.push_back(A);

        for (uint i = 1; i < n - 1; i++)
        {
            // Calcul du point �quivalent � points[i] en abscisse curviligne
            dcoord P;
            double mCoeff;


            double t = i / n;
            if (q == 0 || q == 1)
            {
                // M = A ou M = B
                P = dcoord(1 - t) * A + dcoord(t) * B;
                mCoeff = 0;
            }
            else
            {
                if (t <= q)
                {
                    mCoeff = t / q;
                    double aCoeff = 1 - (t / q);
                    P = dcoord(aCoeff) * A + dcoord(mCoeff) * M;
                }
                else
                {
                    mCoeff = (t - q) / (1 - q);
                    double bCoeff = (1 - t) / (1 - q);
                    P = dcoord(mCoeff) * M + dcoord(bCoeff) * B;
                }
            }

            // Ajout au gradient li� � l'erreur entre P et points[i]
            G += dcoord(2 * mCoeff) * (P - dcoord(*(begin + i)));
            _points.push_back(P);
        }
        _points.push_back(B);
        return G * dcoord(1.0 / (n - 2));
    }

    const std::vector<dcoord>& CutSegment::getPoints() const
    {
        return _points;
    }

    const dcoord CutSegment::getM() const
    {
        return _M;
    }
}