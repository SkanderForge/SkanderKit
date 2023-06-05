#ifndef IMAGEWRAPPER_H
#define IMAGEWRAPPER_H

#include "EasyBMP.h"
#include "types.h"
#include "Graph.h"

#include <queue>
#include <deque>
#include <string>
#include <map>

#ifdef DEBUG_IMGWRAPPER
#define DEBUG_IMGWRAPPER_PRINT(arg) arg
#else
#define DEBUG_IMGWRAPPER_PRINT(arg)
#endif

class imageWrapper
{
public:
    //types
    class OrientedId
    {
    public:
        OrientedId();
        OrientedId(uint ai_id, E_DIRECTION ai_direction) :
            id(ai_id), direction(ai_direction)
        {};
        uint id;
        E_DIRECTION direction;
    };

    class Border
    {

    public:
        void computeCoords();
        void computeCoords_Old();


    public:
        uint m_id;
        std::deque<PixelBorder> m_pixelBorders;
        std::vector<icoord> m_coords;
        std::vector<int> m_pClusters;
    };

    class PixelCluster
    {
    public:
        uint height;
        bool isLoop;
        uint id;
        coord origin;
        std::vector<uint> unorderedBorders;
        Graph<uint> borderGraph;
        std::vector<uint> hasBeenAddedToConnex;
        std::vector<OrientedId> principalConnex;
        std::vector<std::vector<OrientedId>> internalConnexes;
        //std::vector<E_DIRECTION> orderedBorderDirections;
    };

private:
    struct ts_explorationParams
    {
        coord curCoord;
        coord parent;

        ts_explorationParams(coord ai_curCoord, coord ai_parent) :
            curCoord(ai_curCoord), parent(ai_parent)
        {}
    };

    struct ts_pClusterExplorationParams
    {
        int pClusterId;
        uint borderIdWithParent;
        uint height;
    };

public:
    imageWrapper(BMP* aip_bmp):
        mp_Bmp(aip_bmp)
        ,m_nbClusters(0)
        ,m_exploredPixels(0)
    {}

    uint getHeight()
    {
        return mp_Bmp->TellHeight();
    }
    
    uint getWidth()
    {
        return mp_Bmp->TellWidth();
    }

    bool isEqual(const coord& c1, const coord& c2)
    {
        return (mp_Bmp->GetPixel(c1.x, c1.y).Red == mp_Bmp->GetPixel(c2.x, c2.y).Red
             && mp_Bmp->GetPixel(c1.x, c1.y).Green == mp_Bmp->GetPixel(c2.x, c2.y).Green
             && mp_Bmp->GetPixel(c1.x, c1.y).Blue == mp_Bmp->GetPixel(c2.x, c2.y).Blue);
    }

    const RGBApixel getPixel(uint x, uint y)
    {
        return mp_Bmp->GetPixel(x, y);
    }

    void clusterPixels();

    void clusterPixelsIndiv(coord c, coord parent, std::queue<imageWrapper::ts_explorationParams>& ai_toBeExplored);

    void computeTriplePoints();

    bool isTriplePoint(coord c);

    template <typename condition>
    std::vector<coord> getNeighborsFromPixel(const coord& c, condition ai_condition);

    std::vector<coord> getPixelNeighborsFromPoint(const coord& c);

    TwoDimArray<coord> get2dPixelNeighborhoodFromPoint(const coord& c);

    template<typename condition >
    std::vector<coord> getPointNeighborsFromPoint(const coord& c, condition ai_condition);

    std::vector<coord> getPixelNeighborsFromPixelBorder(const PixelBorder& pb);

    void computeBorderVertices();

    bool computeBorderUntilTriplePoint(PixelBorderArray<char>& ai_hasBeenExplored, Border& ai_border, coord ai_curCoord, coord ai_prevCoord, E_DIRECTION ai_direction);
    void computeBorders();

    void computeClusterBorders_p1();

    void expandCluster(int i);
    void computeClusterHeights();

    /* i:   clusterId
    */
    void buildBorderGraphs();
    void computeConnex(const uint& i);
    void computeConnexStartingFrom(const uint& clusterId, const uint& borderId, E_CONNEX_TYPE connexType = E_PRINCIPAL);
    void computeClusterBorders_p2();

    void expand_pCluster();
    
    void addPixelCluster(const coord&);
    PixelCluster& getPixelCluster(const coord&);
    PixelCluster& getPixelClusterById(const uint&);

    void run();

public:
    BMP* mp_Bmp;
    TwoDimArray<uint> m_pixelClusters;
    TwoDimArray<char> m_pixelHasBeenExplored;
    TwoDimArray<coord> m_parentPixel;
    

    /*
    std::vector<std::vector<uint>> m_pixelClusters;
    std::vector<std::vector<char>> m_pixelHasBeenExplored;
    std::vector<std::vector<coord>> m_parentPixel;
    */
    std::vector<coord> m_triplePoints;
    TwoDimArray<uint> m_isTriplePoint;

    std::vector<std::vector<uint>> m_borderIdsInTriplePoint;

    PixelBorderArray<char> m_isInBorder;
    std::vector<PixelBorder> m_pixelBordersInBorder;

    std::vector<Border> m_borders;
    std::map<std::string, PixelCluster> m_clusters;
    std::vector < std::string > m_clustersById;
    uint m_nbClusters;

    Graph<int> m_pClusterGraph;

    uint m_exploredPixels;

    std::queue<ts_pClusterExplorationParams> m_pClustersToExpand;
    std::vector<char> m_pClusterHasBeenExpanded;

};

#endif
