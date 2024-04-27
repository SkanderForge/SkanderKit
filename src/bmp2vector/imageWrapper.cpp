#include "../bmp2vector/imageWrapper.h"
#include <queue>
#include <deque>

#include <unordered_map>
#include <vector>

//#define DEBUG_IMGWRAPPER



void imageWrapper::addPixelCluster(const coord& c)
{
    std::string s = toString(c);
    if (m_clusters.find(s) == m_clusters.end())
    {
        PixelCluster C;
        C.id = m_nbClusters; 
        m_nbClusters++;
        C.origin = c;
        DEBUG_IMGWRAPPER_PRINT(std::cout<<__FUNCTION__<< " " << s << "\tid: "<<C.id<<"\n";)
        m_clusters.insert(std::pair<std::string, PixelCluster>(s, C));
        m_clustersById.push_back(s);
        DEBUG_IMGWRAPPER_PRINT(std::cout<<getPixelClusterById(m_clustersById.size() - 1).id<<" "<< m_clustersById.size() - 1;)
    }

}


imageWrapper::PixelCluster& imageWrapper::getPixelCluster(const coord& c)
{
    return m_clusters.at(toString(m_parentPixel[c]));
}


imageWrapper::PixelCluster& imageWrapper::getPixelClusterById(const uint& i)
{
    //std::cout<<m_clustersById.size() <<"\ttring to access "<<i<<"\n";
    return getPixelCluster(toCoord(m_clustersById[i]));
}

void imageWrapper::clusterPixels()
{
    std::cout<<__FUNCTION__<<"BEGIN\n";
    /*
    m_pixelClusters = std::vector<std::vector<uint>>(this->getWidth(), std::vector<uint>(this->getHeight()));
    m_pixelHasBeenExplored = std::vector<std::vector<char>>(this->getWidth(), std::vector<char>(this->getHeight(), 0));
    m_parentPixel = std::vector<std::vector<coord>>(this->getWidth(), std::vector<coord>(this->getHeight(), coord(0,0)));
    */
    m_nbClusters = 0;

    m_pixelClusters = TwoDimArray<uint>(getWidth(), getHeight());
    m_pixelHasBeenExplored = TwoDimArray<char>(getWidth(), getHeight(), 0);
    m_parentPixel = TwoDimArray<coord>(getWidth(), getHeight(), coord(0, 0));

    std::queue<ts_explorationParams> w_toBeExplored;
    m_exploredPixels = 0;
    


    std::cout << "Width " << getWidth();
    for (uint w_x = 0; w_x < getWidth(); w_x++)
    {
        for (uint w_y = 0; w_y < getHeight(); w_y++)
        {

       //     std::cout << "in loop\n";
         //   std::cout << "pixel (" << w_x << ";" << w_y << ")\n";
            coord w_curPixel(w_x, w_y);

            w_toBeExplored.push(ts_explorationParams(w_curPixel, w_curPixel));
            while (w_toBeExplored.size() != 0)
            {
                ts_explorationParams w_curParams = w_toBeExplored.front();
                w_toBeExplored.pop();
                clusterPixelsIndiv(w_curParams.curCoord, w_curParams.parent, w_toBeExplored);
            }
        }
    }
    

    std::cout<<__FUNCTION__<<"END\n";
};

void imageWrapper::clusterPixelsIndiv(coord ai_curCoord, coord ai_parent, std::queue<imageWrapper::ts_explorationParams>& ai_toBeExplored)
{
    DEBUG_IMGWRAPPER_PRINT(std::cout<<"in function, coord: " << ai_curCoord.x << ";" << ai_curCoord.y << "\n");
    if (m_pixelHasBeenExplored[ai_curCoord] == 0)
    {
        DEBUG_IMGWRAPPER_PRINT(std::cout << "has not been explored\n");
        m_pixelHasBeenExplored[ai_curCoord] = 1;
        if (ai_curCoord == ai_parent)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << "new cluster with: (" << ai_parent.x << "\t" << ai_parent.y << ")\n";)
            addPixelCluster(ai_parent);
            //system("PAUSE");
        }
        
        m_exploredPixels++; 
        DEBUG_IMGWRAPPER_PRINT(
        if (m_exploredPixels % 10000 == 0)
        {
            std::cout << "explored " << 100 * static_cast<double>(m_exploredPixels) /( getWidth() * getHeight()) <<"%\n";
        })
        m_parentPixel[ai_curCoord] = ai_parent;
        DEBUG_IMGWRAPPER_PRINT(std::cout << "parent coord is now: (" << ai_parent.x << ";" << ai_parent.y << ")\n");
        
        auto l_equalPixelCondition = [&](coord c1, coord c2) {return (isEqual(c1, c2) && !m_pixelHasBeenExplored[c2]); };

        auto w_neighbors = getNeighborsFromPixel(ai_curCoord, l_equalPixelCondition);
        DEBUG_IMGWRAPPER_PRINT(std::cout << "ok getNeighbors\n");

        for (coord w_neighbor : w_neighbors)
        {
            ai_toBeExplored.push(ts_explorationParams(w_neighbor, ai_parent));
            //clusterPixelsIndiv(w_neighbor, ai_parent);
        }
    }
};

template <typename Condition>
std::vector<coord> imageWrapper::getNeighborsFromPixel(const coord& c, Condition ai_condition)
{
    DEBUG_IMGWRAPPER_PRINT(std::cout << "begin getNeighbors, coord: (" << c.x << ";"<<c.y<<")\n");
    std::vector<coord> neighbors;
    std::vector<coord> potentialNeighbors;
    if (c.x > 0) { potentialNeighbors.push_back(coord(c.x - 1, c.y)); }
    if (c.y > 0) { potentialNeighbors.push_back(coord(c.x, c.y - 1)); }
    if (c.x < this->getWidth() - 1) { potentialNeighbors.push_back(coord(c.x + 1, c.y)); }
    if (c.y < this->getHeight() - 1) { potentialNeighbors.push_back(coord(c.x, c.y + 1)); }

    for (coord n : potentialNeighbors)
    {
        if (ai_condition(c, n))
        {
            neighbors.push_back(n);
            DEBUG_IMGWRAPPER_PRINT(std::cout << "added neighbor " << n.x << " " << n.y << "\n");
        }
    }

    DEBUG_IMGWRAPPER_PRINT(std::cout << "end getNeighbors\n");
    return neighbors;
};

std::vector<coord> imageWrapper::getPixelNeighborsFromPoint(const coord& c)
{
    std::vector<coord> neighbors;
    if (c.x > 0          && c.y > 0)           { neighbors.push_back(coord(c.x - 1, c.y - 1)); }
    if (c.x > 0          && c.y < getHeight()) { neighbors.push_back(coord(c.x - 1, c.y    )); }
    if (c.x < getWidth() && c.y > 0)           { neighbors.push_back(coord(c.x    , c.y - 1)); }
    if (c.x < getWidth() && c.y < getHeight()) { neighbors.push_back(coord(c.x    , c.y    )); }

   
    return neighbors;
}


template<typename condition>
std::vector<coord> imageWrapper::getPointNeighborsFromPoint(const coord& c, condition ai_condition)
{
    std::vector<coord> potentialNeighbors;
    std::vector<coord> neighbors;
    if (c.x > 0) { potentialNeighbors.push_back(coord(c.x - 1, c.y)); }
    if (c.y > 0) { potentialNeighbors.push_back(coord(c.x, c.y - 1)); }
    if (c.x < getWidth()) { potentialNeighbors.push_back(coord(c.x + 1, c.y)); }
    if (c.y < getHeight()) { potentialNeighbors.push_back(coord(c.x, c.y + 1)); }

    for (coord n : potentialNeighbors) {
        if (ai_condition(n)) {
            neighbors.push_back(n); 
        }
    }
    
    return neighbors;
}

std::vector<coord> imageWrapper::getPixelNeighborsFromPixelBorder(const PixelBorder& pb)
{
    std::vector<coord> neighbors;
    if (pb.getOrientation() == E_VERTICAL)
    {
        if (pb.getBack().x > 0)
        {
            neighbors.push_back(coord(pb.getBack() - coord(1, 0)));
        }
        if (pb.getBack().x < getWidth())
        {
            neighbors.push_back(pb.getBack());
        }
    }
    else
    {
        if (pb.getBack().y > 0)
        {
            neighbors.push_back(coord(pb.getBack() - coord(0, 1)));
        }
        if (pb.getBack().y < getHeight())
        {
            neighbors.push_back(pb.getBack());
        }
    }

    return neighbors;
}

void imageWrapper::computeTriplePoints()
{
    std::cout << __FUNCTION__ << " BEGIN\n";

    auto l_alwaysTrue = []() {return 1; };
    m_isTriplePoint = TwoDimArray<uint>(getWidth() + 1, getHeight() + 1,  0);

    //these vectors start at 1
    m_triplePoints.push_back(coord(0));
    m_borderIdsInTriplePoint.push_back(std::vector<uint>());
    uint nbTriplePoints = 0;

    for (uint w_x = 0; w_x < getWidth() + 1; w_x++)
    {
        for (uint w_y = 0; w_y < getHeight() + 1; w_y++)
        {
            coord w_curPoint(w_x, w_y);


            m_isTriplePoint[w_curPoint] = isTriplePoint(w_curPoint);

            if (m_isTriplePoint[w_curPoint] == 1)
            {
                nbTriplePoints++;
                DEBUG_IMGWRAPPER_PRINT(std::cout << "new triple point: (" << w_curPoint.x << "\t" << w_curPoint.y << ")\n");
                m_isTriplePoint[w_curPoint] = nbTriplePoints;
                m_triplePoints.push_back(w_curPoint);
                m_borderIdsInTriplePoint.push_back(std::vector<uint>());
                DEBUG_IMGWRAPPER_PRINT(std::cout << "triple point id: " << m_isTriplePoint[w_curPoint] << "\n");
                //system("PAUSE");
            }

        }
    }

    std::cout << __FUNCTION__ << " END\n";
}

bool imageWrapper::isTriplePoint(coord c)
{
    auto w_neighbors = getPixelNeighborsFromPoint(c);
    bool w_ans = 0;

    if (w_neighbors.size() == 1)
    {
        w_ans = 0;
    }
    else if (w_neighbors.size() ==2)
    {
        w_ans = (m_parentPixel[w_neighbors[0]] != m_parentPixel[w_neighbors[1]]);
    }
    else if (w_neighbors.size() == 4)
    {

        uint nbEqual = 0;
        bool diagonalEqual = false;
        for (uint w_i = 0; w_i < 4; w_i++)
        {
            for (uint w_j = w_i + 1; w_j < 4; w_j++)
            {
                if (m_parentPixel[w_neighbors[w_i]] == m_parentPixel[w_neighbors[w_j]])
                { 
                    nbEqual++; 
                    // Check if diagonal from one another
                    if ((w_neighbors[w_i].x + w_neighbors[w_i].y) == (w_neighbors[w_j].x + w_neighbors[w_j].y))
                    {
                        diagonalEqual = true;
                    }
                    else if ((w_neighbors[w_i].x + w_neighbors[w_i].y) == ((w_neighbors[w_j].x + w_neighbors[w_j].y) + 2))
                    {
                        diagonalEqual = true;
                    }
                    else if (((w_neighbors[w_i].x + w_neighbors[w_i].y) + 2) == (w_neighbors[w_j].x + w_neighbors[w_j].y) )
                    {
                        diagonalEqual = true;
                    }

                }
            }
        }
        w_ans = (nbEqual < 2) && (!diagonalEqual);
    }
    return w_ans;

}


void imageWrapper::computeBorderVertices()
{
    std::cout << __FUNCTION__ << " BEGIN\n";
    m_isInBorder = PixelBorderArray<char>(getWidth(), getHeight(), 0);

    std::cout << "hSize: " << m_isInBorder.hSize().x << "\t" << m_isInBorder.hSize().y << "\n";
    std::cout << "vSize: " << m_isInBorder.vSize().x << "\t" << m_isInBorder.vSize().y << "\n";


    coord w_hSize = m_isInBorder.hSize();
    coord w_vSize = m_isInBorder.vSize();
    
    //horz borders
    for (uint w_x = 0; w_x < w_hSize.x; w_x++)
    {
        for (uint w_y = 1; w_y < w_hSize.y - 1; w_y++)
        {
            coord curCoord(w_x, w_y);
            PixelBorder hVertex(curCoord, E_HORIZONTAL);
            coord cluster1 = m_parentPixel[curCoord];
            coord cluster2 = m_parentPixel[coord(curCoord.x, curCoord.y - 1)];
            if (cluster1 != cluster2)
            {
                m_isInBorder[hVertex] = 1;
                m_pixelBordersInBorder.push_back(hVertex);
            }
        }
    }


    for (uint w_x = 0; w_x < w_hSize.x; w_x++)
    {
        for (uint w_y = 0; w_y < w_hSize.y; w_y += (w_hSize.y - 1))
        {
            coord curCoord(w_x, w_y);
            PixelBorder vVertex = PixelBorder(curCoord, E_HORIZONTAL);
            m_isInBorder[vVertex] = 1;
            m_pixelBordersInBorder.push_back(vVertex);

        }
    }

    //vert borders
    for (uint w_x = 1; w_x < w_vSize.x-1; w_x++)
    {
        for (uint w_y = 0; w_y < w_vSize.y ; w_y++)
        {
            coord curCoord(w_x, w_y);
            PixelBorder vVertex(curCoord, E_VERTICAL);

            coord cluster1 = m_parentPixel[curCoord];
            coord cluster2 = m_parentPixel[coord(curCoord.x - 1, curCoord.y)];
            if (cluster1 != cluster2)
            {
                m_isInBorder[vVertex] = 1;
                m_pixelBordersInBorder.push_back(vVertex);
            }
        }
    }

    for (uint w_x = 0; w_x < w_vSize.x; w_x += (w_vSize.x - 1))
    {
        for (uint w_y = 0; w_y < w_vSize.y; w_y++)
        {
            coord curCoord(w_x, w_y);
            PixelBorder hVertex = PixelBorder(curCoord, E_VERTICAL);
            m_isInBorder[hVertex] = 1;
            m_pixelBordersInBorder.push_back(hVertex);

        }
    }

    //std::cout << "is in border coord(125, 36), E_VERTICAL: " <<static_cast<int>( m_isInBorder[PixelBorder(coord(125, 36), E_VERTICAL)] )<< "\n";
    //std::cout << "is in border coord(125, 36), E_VERTICAL: " << static_cast<int>(m_isInBorder[PixelBorder(coord(125, 36), E_HORIZONTAL)]) << "\n";

    std::cout << __FUNCTION__ << " END\n";
}

bool imageWrapper::computeBorderUntilTriplePoint(PixelBorderArray<char>& ai_hasBeenExplored, Border& ai_border, coord ai_curCoord, coord ai_prevCoord, E_DIRECTION ai_direction)
{
    PixelBorder  w_curVertex(ai_curCoord, ai_prevCoord);
    coord w_curCoord = ai_curCoord;
    coord w_prevCoord = ai_prevCoord;

    coord w_initialCurCoord = w_curCoord;
    coord w_initialPrevCoord = w_prevCoord;

    bool w_isLoop = false;
    bool borderSignatureHasBeenInitialized = true;
    std::vector<coord> borderSignature;

    PixelBorder initialPixelBorder(w_initialCurCoord, w_initialPrevCoord);

    auto pixelsOnSidesOfBorder = getPixelNeighborsFromPixelBorder(initialPixelBorder);
    for (auto p : pixelsOnSidesOfBorder)
    {
        borderSignature.push_back(m_parentPixel[p]);
    }

    //std::cout << "borderSignature: ";
    //for (auto c : borderSignature) { std::cout << toString(c) << " | "; } std::cout << "\n";



    while (!m_isTriplePoint[w_curCoord] && !w_isLoop)
    {
        bool debug = 0;
        if (w_curCoord == coord(33, 19))
        {
            debug = 1;
        }
        if (debug)
        {

        }
        //std::cout << "curCoord: "<<w_curCoord.x << "\t" << w_curCoord.y << "\t\t\t, prevCoord: "<< w_prevCoord.x << "\t" << w_prevCoord.y << "\n";
        std::vector<coord> w_neighbors = getPointNeighborsFromPoint(w_curCoord, [=](const coord& c) ->bool {return (c != w_prevCoord); });

        coord w_nextCoord;
        PixelBorder w_nextVertex;

        bool isStillBorder = false;

        //std::cout << "neighbors:\n";
        for (coord c : w_neighbors)
        {
            //std::cout << "\t" << c.x << "\t" << c.y << "\n";
            PixelBorder pb(c, w_curCoord);
            //std::cout << "creating vertex " << v.getBack().x << "\t" << v.getBack().y << "\t" << v.getOrientation() << "\n";
            if (m_isInBorder[pb])
            {
                // Il faut s'assurer qu'on suit toujours la m�me fronti�re

                bool isStillBorder = false;

                pixelsOnSidesOfBorder = getPixelNeighborsFromPixelBorder(pb);
                std::vector<coord> pbSignature;
                for (auto p : pixelsOnSidesOfBorder)
                {
                    pbSignature.push_back(m_parentPixel[p]);
                }

                //std::cout << "pbSignature: ";
                //for (auto c : pbSignature) { std::cout << toString(c) << " | "; } std::cout << "\n";

                // Comparing signatures
                isStillBorder = false;
                if (pbSignature.size() == borderSignature.size())
                {
                    if (pbSignature.size() == 1)
                    {
                        if (pbSignature[0] == borderSignature[0])
                        {
                            isStillBorder = true;
                        }
                    }
                    else //size = 2
                    {
                        if (pbSignature[0] == borderSignature[0] && pbSignature[1] == borderSignature[1])
                        {
                            isStillBorder = true;
                        }
                        else if (pbSignature[0] == borderSignature[1] && pbSignature[1] == borderSignature[0])
                        {
                            isStillBorder = true;
                        }
                    }
                }
                
                if (isStillBorder)
                {
                    w_nextCoord = c;
                    w_nextVertex = PixelBorder(c, w_curCoord);
                    break;
                }
                //std::cout << "nextCoord: " << w_nextCoord.x << "\t" << w_nextCoord.y << "\n";
            }
        }


        ai_hasBeenExplored[w_nextVertex] = 1;
        w_curVertex = w_nextVertex;
        w_prevCoord = w_curCoord;
        w_curCoord = w_nextCoord;

        //std::cout<< "w_curCoord: " << toString(w_curCoord) << " | "<< toString(w_initialPrevCoord) <<"\n";
        //system("PAUSE");

        if (ai_direction == E_FRONT)
        {
            ai_border.m_pixelBorders.push_front(w_curVertex);
        }
        else
        {
            ai_border.m_pixelBorders.push_back(w_curVertex);
        }

        if (w_curCoord == w_initialPrevCoord)
        {
            w_isLoop = true;
        }
    }

    DEBUG_IMGWRAPPER_PRINT(std::cout << "reached end of border at " << toString(w_curCoord) << ", with prevcoord: "<<toString(w_prevCoord)<<", isLoop: "<< w_isLoop<<"\n");
    //system("PAUSE");

    return w_isLoop;
}


void imageWrapper::Border::computeCoords()
{
    //remplir m_coords � partir de m_pixelBorders
    
    Graph<std::string> coordGraph;

    for (auto pb : m_pixelBorders)
    {
        coordGraph.addArc(toString(pb.getBack()), toString(pb.getFront()));
    }

    auto coordStringVector = coordGraph.getKeys();

    std::string firstCoord = coordStringVector[0];
    bool isLoop = true;
    for (auto c : coordStringVector)
    {
        if (coordGraph.getDegree(c) == 1)
        {
            firstCoord = c;
            isLoop = false;
            break;
        }
    }

    m_coords.push_back(toCoord(firstCoord));
    std::string curCoord = firstCoord;
    std::string prevCoord = "";
    for (uint i = 0; i < coordStringVector.size() - 1 + isLoop; i++)
    {
        auto neighbors = coordGraph.getNeighbors(curCoord);
        std::string saved_curCoord = curCoord;
        if (neighbors[0] != prevCoord)
        {
            curCoord = neighbors[0];
        }
        else
        {
            curCoord = neighbors[1];

        }

        prevCoord = saved_curCoord;
        m_coords.push_back(toCoord(curCoord));
    }
    DEBUG_IMGWRAPPER_PRINT(
    std::cout << "coords: \n";
    for (auto c : m_coords)
    {
        std::cout << "(" << c.x << "\t" << c.y << ") | ";
    }
    std::cout << "\n");
}


void imageWrapper::computeBorders()
{
    std::cout << __FUNCTION__ << " BEGIN\n";

    PixelBorderArray<char> w_hasBeenExplored(getWidth(),getHeight(), 0);

    for (PixelBorder w_curVertex : m_pixelBordersInBorder)
    {
        if (w_hasBeenExplored[w_curVertex] == 0)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << "computing borderId: " << m_borders.size() << "\n");

            w_hasBeenExplored[w_curVertex] = 1;
            DEBUG_IMGWRAPPER_PRINT(std::cout << "NEW BORDER starting from vertex (" << w_curVertex.getBack().x << "\t" << w_curVertex.getBack().y << "\t" << w_curVertex.getOrientation() << ")\n");
            Border w_curBorder;
            w_curBorder.m_pixelBorders.push_back(w_curVertex);

            //start front
            coord w_curCoord = w_curVertex.getFront();
            coord w_prevCoord = w_curVertex.getBack();

            bool w_isLoop = false;

            w_isLoop = computeBorderUntilTriplePoint(w_hasBeenExplored, w_curBorder, w_curCoord, w_prevCoord, E_FRONT);

            //std::cout << "front ok\n";

            if (!w_isLoop)
            {
                //std::cout << "start back\n";
                w_curCoord = w_curVertex.getBack();
                w_prevCoord = w_curVertex.getFront();
                computeBorderUntilTriplePoint(w_hasBeenExplored, w_curBorder, w_curCoord, w_prevCoord, E_BACK);
            }

            w_curBorder.computeCoords();
            w_curBorder.m_id = m_borders.size();
            //TODO
            // find clusters on both sides of w_curBorder.m_pixelBorders[0] and add them to 
            // w_curBorder.m_pClusters
            auto pBorder = w_curBorder.m_pixelBorders[0];
            auto pBorderNeighbors = getPixelNeighborsFromPixelBorder(pBorder);
            DEBUG_IMGWRAPPER_PRINT(std::cout << "pBorderNeighbors:\n";
            for (auto c : pBorderNeighbors)
            {
                std::cout << toString(c) << "\n";
            })
            for (coord c : pBorderNeighbors)
            {
                PixelCluster& pC = getPixelCluster(c);
                DEBUG_IMGWRAPPER_PRINT(std::cout << "adding pC.id: " << pC.id << "\n";)
                w_curBorder.m_pClusters.push_back(pC.id);
            }
            if (pBorderNeighbors.size() == 1)
            {
                w_curBorder.m_pClusters.push_back(-1);
            }

            DEBUG_IMGWRAPPER_PRINT(std::cout << "Border " << w_curBorder.m_id << " has pCluster neighbors: "; for (auto w_i : w_curBorder.m_pClusters) { std::cout << w_i << " "; }
            std::cout << "\n";)


            m_borders.push_back(w_curBorder);
            
            

            DEBUG_IMGWRAPPER_PRINT(std::cout << "borderId: " << w_curBorder.m_id << "\n";
            for (PixelBorder v : w_curBorder.m_pixelBorders)
            {
                std::cout << "(" << v.getBack().x << " " << v.getBack().y << ") "<< "(" << v.getFront().x << " " << v.getFront().y << ") " << " | ";
            }
            std::cout << "\n";)

            if (!w_isLoop)
            {
                //not a loop so first and last coords must be triple points
                DEBUG_IMGWRAPPER_PRINT(
                std::cout << "between triple points id: " << m_isTriplePoint[w_curBorder.m_coords[0]] << " and " << m_isTriplePoint[w_curBorder.m_coords.back()] << "\n";
                std::cout << "m_borderIdsInTriplePoint.size(): " << m_borderIdsInTriplePoint.size() << "\n";)
                m_borderIdsInTriplePoint[m_isTriplePoint[w_curBorder.m_coords[0]]].push_back(w_curBorder.m_id);
                m_borderIdsInTriplePoint[m_isTriplePoint[w_curBorder.m_coords.back()]].push_back(w_curBorder.m_id);
            }
        }
    }

    std::cout << __FUNCTION__ << " END\n";
}


void imageWrapper::computeClusterBorders_p1()
{
    std::cout << __FUNCTION__ << " BEGIN\n";

    for (uint borderId = 0; borderId < m_borders.size(); borderId++)
    {
        PixelBorder pb = m_borders[borderId].m_pixelBorders[0];
        std::vector<coord> neighboringPixels;
        if (pb.getOrientation() == E_HORIZONTAL)
        {
            if (pb.getBack().y > 0)
            {
                neighboringPixels.push_back(pb.getBack() - coord(0, 1));
            }
            if (pb.getBack().y < getHeight())
            {
                neighboringPixels.push_back(pb.getBack());
            }
        }
        else
        {
            if (pb.getBack().x > 0)
            {
                neighboringPixels.push_back(pb.getBack() - coord(1, 0));
            }
            if (pb.getBack().x < getWidth())
            {
                neighboringPixels.push_back(pb.getBack());
            }

        }

        DEBUG_IMGWRAPPER_PRINT(std::cout << "neighbors of border " << borderId << " (inc. pborder " << pb.getBack().x << "\t" << pb.getBack().y << "\t" << pb.getOrientation() << ") :\n";)
        for (auto c : neighboringPixels)
        {
            coord parent = m_parentPixel[c];
            getPixelCluster(parent).unorderedBorders.push_back(borderId);
        }
        
        if (neighboringPixels.size() == 2)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << getPixelCluster(m_parentPixel[neighboringPixels[0]]).id << "\t" << getPixelCluster(m_parentPixel[neighboringPixels[1]]).id << "\n";)
            m_pClusterGraph.addArc(getPixelCluster(m_parentPixel[neighboringPixels[0]]).id, getPixelCluster(m_parentPixel[neighboringPixels[1]]).id);
        }
        else if (neighboringPixels.size() == 1)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << "addArc to border " << getPixelCluster(m_parentPixel[neighboringPixels[0]]).id << "\t" << -1 << "\n";)
            m_pClusterGraph.addArc(getPixelCluster(m_parentPixel[neighboringPixels[0]]).id, -1);
        }
        DEBUG_IMGWRAPPER_PRINT(std::cout << "neighboringPixels.size(): " << neighboringPixels.size() << "\n";)
    }

    std::cout << __FUNCTION__ << " END\n";
}





void imageWrapper::buildBorderGraphs()
{
    std::cout << "buildBorderGraphs BEGIN\n";
    for (uint pClusterId = 0; pClusterId < m_clustersById.size(); pClusterId++)
    {
        PixelCluster& pc = getPixelClusterById(pClusterId);
        DEBUG_IMGWRAPPER_PRINT(std::cout << "clusterId: " << pc.id << " (actual id : " << pClusterId << "), parent: " << toString(pc.origin) << "\n";)
        if (pc.unorderedBorders.size() == 1)
        {
            pc.isLoop = true;
            pc.principalConnex.emplace_back(pc.unorderedBorders[0], E_FRONT);
            DEBUG_IMGWRAPPER_PRINT(std::cout << "is loop\n";)
            //pc.orderedBorderDirections.push_back(E_FRONT);
        }
        else
        {
            pc.isLoop = false;
            DEBUG_IMGWRAPPER_PRINT(std::cout << "is not loop\n";
            for (auto b : pc.unorderedBorders) 
            { 
                std::cout << b << ": " << m_borders[b].m_pClusters[0] << " " << m_borders[b].m_pClusters.back() << "\n";
            }
            std::cout << "\n";)
            
            //building borderGraph of cluster
            for (uint i = 0; i < pc.unorderedBorders.size(); i++)
            {
                for (uint j = i + 1; j < pc.unorderedBorders.size(); j++)
                {
                    coord i_begin = m_borders[pc.unorderedBorders[i]].m_coords[0];
                    coord i_end = m_borders[pc.unorderedBorders[i]].m_coords[m_borders[pc.unorderedBorders[i]].m_coords.size() - 1];

                    coord j_begin = m_borders[pc.unorderedBorders[j]].m_coords[0];
                    coord j_end = m_borders[pc.unorderedBorders[j]].m_coords[m_borders[pc.unorderedBorders[j]].m_coords.size() - 1];

                    if (i_begin == j_begin || i_begin == j_end || i_end == j_begin || i_end == j_end)
                    {
                        pc.borderGraph.addArc(i, j);
                    }
                }
            }

            DEBUG_IMGWRAPPER_PRINT(std::cout << "ordering done\n";)
        }
    }
    

    std::cout << "buildBorderGraphs END\n";
}

/*
void imageWrapper::computeConnex(const uint& pClusterId)
{
    PixelCluster& pc = getPixelClusterById(pClusterId);
    std::cout << "clusterId: " << pc.id << ", parent: " << toString(pc.origin) << "\n";

    if (!pc.isLoop)
    {

        //
        uint curBorderId = 0;
        uint nextBorderId;
        E_DIRECTION curDirection = E_FRONT;

        //debut boucle
        //pc.orderedBorders.push_back(pc.unorderedBorders[curBorderId]);
        //pc.orderedBorderDirections.push_back(curDirection);



        coord curCoord;

        //ordering connex starting from localBorderId 0
        do
        {

            pc.principalConnex.emplace_back(pc.unorderedBorders[curBorderId], curDirection);
            //pc.orderedBorderDirections.push_back(curDirection);
            std::cout << "new iteration of loop, curborder realid:" << pc.unorderedBorders[curBorderId] << "\n";
            for (auto c : pc.unorderedBorders) { std::cout << c << " "; }
            std::cout << "\n";
            if (curDirection == E_FRONT)
            {
                curCoord = m_borders[pc.unorderedBorders[curBorderId]].m_coords.back();//[m_borders[pc.orderedBorders[curBorderId]].m_coords.size() - 1];
            }
            else
            {
                curCoord = m_borders[pc.unorderedBorders[curBorderId]].m_coords[0];
            }
            auto neighborhood = pc.borderGraph.getNeighbors(curBorderId);
            for (auto p : neighborhood)
            {
                if (m_borders[pc.unorderedBorders[p]].m_coords[0] == curCoord)
                {
                    curBorderId = p;
                    curDirection = E_FRONT;
                    break;
                }
                else if (m_borders[pc.unorderedBorders[p]].m_coords.back() == curCoord)
                {
                    curBorderId = p;
                    curDirection = E_BACK;
                    break;
                }

            }
            std::cout << "curBorderId: " << curBorderId << "\n";

            m_borders[pc.unorderedBorders[0]].m_coords[0];
            std::cout << "curCoord: " << toString(curCoord) << ",  m_borders[pc.unorderedBorders[0]].m_coords[0]: " << toString(m_borders[pc.unorderedBorders[0]].m_coords[0]) << "\n";

        } while (curCoord != m_borders[pc.unorderedBorders[0]].m_coords[0]);
        std::cout << "orderedBorders: ";
        for (auto i : pc.principalConnex) { std::cout << i.id << " "; }
        std::cout << "\n";

        std::cout << "w4\n";
    }
}

*/

void imageWrapper::computeConnexStartingFrom(const uint& pClusterId, const uint& borderId, E_CONNEX_TYPE connexType)
{
    DEBUG_IMGWRAPPER_PRINT(std::cout << "computeConnexStartingFrom BEGIN\n";)
    PixelCluster& pc = getPixelClusterById(pClusterId);
    //std::cout << "clusterId: " << pc.id << ", parent: " << toString(pc.origin) << "\n";
    std::vector<OrientedId>* connexToFill;
    if (connexType == E_PRINCIPAL)
    {
        DEBUG_IMGWRAPPER_PRINT(std::cout << "principal\n";)
        connexToFill = &pc.principalConnex;
    }
    else
    {
        DEBUG_IMGWRAPPER_PRINT(std::cout << "new internal\n";)
        pc.internalConnexes.push_back(std::vector<OrientedId>());
        connexToFill = &pc.internalConnexes.back();
    }

    if (!pc.isLoop)
    {

        // find borderId in unorderedBorders
        //std::cout << "find borderId "<< borderId <<" in unorderedBorders...\n";
        uint localBorderId = 0;
        for (uint i = 0; i < pc.unorderedBorders.size(); i++)
        {
            if (pc.unorderedBorders[i] == borderId)
            {
                localBorderId = i;
                //std::cout << "found at localBorderId: " << localBorderId << "\n";
                break;
            }
        }

        uint curBorderId = localBorderId;
        E_DIRECTION curDirection = E_FRONT;

        //debut boucle
        //pc.orderedBorders.push_back(pc.unorderedBorders[curBorderId]);
        //pc.orderedBorderDirections.push_back(curDirection);



        coord curCoord;

        do
        {
            
            connexToFill->emplace_back(pc.unorderedBorders[curBorderId], curDirection);
            pc.hasBeenAddedToConnex[curBorderId] = 1;
            //pc.orderedBorderDirections.push_back(curDirection);
            //std::cout << "new iteration of loop, curborder realid:" << pc.unorderedBorders[curBorderId] << "\n";
            //for (auto c : pc.unorderedBorders) { std::cout << c << " "; }
            //std::cout << "\n";
            if (curDirection == E_FRONT)
            {
                curCoord = m_borders[pc.unorderedBorders[curBorderId]].m_coords.back();//[m_borders[pc.orderedBorders[curBorderId]].m_coords.size() - 1];
            }
            else
            {
                curCoord = m_borders[pc.unorderedBorders[curBorderId]].m_coords[0];
            }
            auto neighborhood = pc.borderGraph.getNeighbors(curBorderId);
            for (auto p : neighborhood)
            {
                if (m_borders[pc.unorderedBorders[p]].m_coords[0] == curCoord)
                {
                    //check if p has already been added
                    bool alreadyBeenAdded = 0;
                    for (uint i = 0; i < connexToFill->size(); i++)
                    {
                        if ((*connexToFill)[i].id == pc.unorderedBorders[p])
                        {
                            alreadyBeenAdded = 1;
                            break;
                        }
                    }

                    if (!alreadyBeenAdded)
                    {
                        curBorderId = p;
                        curDirection = E_FRONT;
                        break;
                    }
                }
                else if (m_borders[pc.unorderedBorders[p]].m_coords.back() == curCoord)
                {
                    //check if p has already been added
                    bool alreadyBeenAdded = 0;
                    for (uint i = 0; i < connexToFill->size(); i++)
                    {
                        if ((*connexToFill)[i].id == pc.unorderedBorders[p])
                        {
                            alreadyBeenAdded = 1;
                            break;
                        }
                    }

                    if (!alreadyBeenAdded)
                    {
                        curBorderId = p;
                        curDirection = E_BACK;
                        break;
                    }
                }

            }
            //std::cout << "curBorderId: " << curBorderId << "\n";

            m_borders[pc.unorderedBorders[0]].m_coords[0];
            //std::cout << "curCoord: " << toString(curCoord) << ",  m_borders[pc.unorderedBorders[0]].m_coords[0]: " << toString(m_borders[pc.unorderedBorders[0]].m_coords[0]) << "\n";

        } while (curCoord != m_borders[pc.unorderedBorders[localBorderId]].m_coords[0]);
        //std::cout << "orderedBorders: ";
        for (auto i : (*connexToFill)) { std::cout << i.id << " "; }
        //std::cout << "\n";


        //std::cout << "w4\n";
    }
    else
    {
        pc.hasBeenAddedToConnex[0] = 1;
    }

    DEBUG_IMGWRAPPER_PRINT(std::cout << "computeConnexStartingFrom END\n";)
}

void imageWrapper::expand_pCluster()
{
    DEBUG_IMGWRAPPER_PRINT(std::cout << "expand_pCluster BEGIN\n";)
    auto expandParams = m_pClustersToExpand.front();
    m_pClustersToExpand.pop();
    DEBUG_IMGWRAPPER_PRINT(std::cout << "expandParams.pClusterId: " << expandParams.pClusterId << "\n";)
    if (m_pClusterHasBeenExpanded[expandParams.pClusterId] == 0)
    {
        DEBUG_IMGWRAPPER_PRINT(std::cout << "has not been explored\n";)
        m_pClusterHasBeenExpanded[expandParams.pClusterId] = 1;

        PixelCluster& pCluster = getPixelClusterById(expandParams.pClusterId);
        pCluster.height = expandParams.height;
        DEBUG_IMGWRAPPER_PRINT(std::cout << "height: " << pCluster.height << "\n";)
        pCluster.hasBeenAddedToConnex = std::vector<uint>(pCluster.unorderedBorders.size());
        // Compute principal connex
        computeConnexStartingFrom(expandParams.pClusterId, expandParams.borderIdWithParent, E_PRINCIPAL);
        // Compute internal connexes
        bool allHaveBeenAdded = false;
        uint notAddedId = 0;
        while (!allHaveBeenAdded)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << "pCluster.hasBeenAddedToConnex.size(): " << pCluster.hasBeenAddedToConnex.size() << "\n";)
            while (notAddedId < pCluster.hasBeenAddedToConnex.size())
            {
                if (pCluster.hasBeenAddedToConnex[notAddedId] == 0)
                {
                    break;
                }
                else 
                {
                    notAddedId++;
                }
            }

            if (notAddedId == pCluster.hasBeenAddedToConnex.size())
            {
                allHaveBeenAdded = true;
            }
            else
            {
                DEBUG_IMGWRAPPER_PRINT(std::cout << "notAddedId: " << notAddedId << "\n";)
                computeConnexStartingFrom(expandParams.pClusterId, pCluster.unorderedBorders[notAddedId], E_INTERNAL);
            }
            

        }

        
        auto neighbors = m_pClusterGraph.getNeighbors(expandParams.pClusterId);

        for (auto neighborClusterId : neighbors)
        {
            if (neighborClusterId >= 0)
            {
                
                // Find border shared between neighbor and this cluster
                ts_pClusterExplorationParams neighborExpandParams;
                neighborExpandParams.pClusterId = neighborClusterId;
                neighborExpandParams.height = 0;
                neighborExpandParams.borderIdWithParent = 0; //TODO ne devrait pas �tre n�cessaire
                //find intersecting border id
                bool foundIntersectingBorderId = false;
                for (uint i = 0; i < pCluster.unorderedBorders.size(); i++)
                {
                    if (m_borders[pCluster.unorderedBorders[i]].m_pClusters[0] == neighborClusterId || m_borders[pCluster.unorderedBorders[i]].m_pClusters[1] == neighborClusterId)
                    {
                        neighborExpandParams.borderIdWithParent = i;
                        foundIntersectingBorderId = true;
                        break;
                    }
                }
                if (!foundIntersectingBorderId)
                {
                    DEBUG_IMGWRAPPER_PRINT(
                    std::cout << "intersectingBorderId not found between:\n";
                    std::cout << "clusterId: " << expandParams.pClusterId << " (from coord : "<< toString(getPixelClusterById(expandParams.pClusterId).origin) <<") and \n";
                    std::cout << "clusterId: " << neighborClusterId << " (from coord : " << toString(getPixelClusterById(neighborClusterId).origin) << ") and \n";
                    )
                }
                //check if border is in principal connex to determine height
                bool isInPrincipalConnex = 0;
                for (uint i = 0; i < pCluster.principalConnex.size(); i++)
                {
                    if (neighborExpandParams.borderIdWithParent == pCluster.principalConnex[i].id)
                    {
                        isInPrincipalConnex = 1;
                        break;
                    }
                }

                if (isInPrincipalConnex)
                {
                    neighborExpandParams.height = pCluster.height;
                }
                else
                {
                    neighborExpandParams.height = pCluster.height+1;
                }

                m_pClustersToExpand.push(neighborExpandParams);

            }
        }

    }

    DEBUG_IMGWRAPPER_PRINT(std::cout << "expand_pCluster END\n";)
}

void imageWrapper::computeClusterBorders_p2()
{
    std::cout << __FUNCTION__ << " BEGIN\n";
    m_pClusterHasBeenExpanded = std::vector<char>(m_nbClusters, 0);
    
    auto neighborsFromEdge = m_pClusterGraph.getNeighbors(-1);

    for (auto n : neighborsFromEdge)
    {
        DEBUG_IMGWRAPPER_PRINT(std::cout << n << " is neighbor with edge\n";)
        //find borderid between -1 and neighbor
        PixelCluster& pCluster = getPixelClusterById(n);
        
        uint borderIdWithParent;
        for (auto borderId : pCluster.unorderedBorders)
        {
            if (m_borders[borderId].m_pClusters[0] == -1 || m_borders[borderId].m_pClusters[1] == -1)
            {
                borderIdWithParent = borderId;
                break;
            }
        }
        ts_pClusterExplorationParams pClusterExplorationParams;
        pClusterExplorationParams.pClusterId = n;
        pClusterExplorationParams.borderIdWithParent = borderIdWithParent;
        pClusterExplorationParams.height = 0;
        m_pClustersToExpand.push(pClusterExplorationParams);

        DEBUG_IMGWRAPPER_PRINT(std::cout << "borderId with edge: " << borderIdWithParent << "\n";)
    }

    std::cout << "writing down neighbors of image edge END, nb of image edge: "<< m_pClustersToExpand.size()<<"\n";

    while (m_pClustersToExpand.size() > 0)
    {
        expand_pCluster();
    }

    for (uint i = 0; i < m_pClusterHasBeenExpanded.size(); i++)
    {
        if (m_pClusterHasBeenExpanded[i] == 0)
        {
            DEBUG_IMGWRAPPER_PRINT(std::cout << "pCluster " << i << " has not been expanded\n";)
            //TODO comprendre pourquoi certains ne sont pas expanded
        }
    }


    std::cout << __FUNCTION__ << " END\n";
}

void imageWrapper::run()
{
    clusterPixels();
    computeTriplePoints();
    computeBorderVertices();
    computeBorders();
    computeClusterBorders_p1();
    buildBorderGraphs();
    computeClusterBorders_p2();

}
