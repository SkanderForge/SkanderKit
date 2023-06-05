#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <map>
#include <unordered_set>

#include "bmp2vector/EasyBMP.h"
#include "bmp2vector/Rgb.h"
#include "bmp2vector/Map.h"
#include "bmp2vector/imageWrapper.h"
#include "bmp2vector/SVG.h"
#include "bmp2vector/types.h"
#include "bmp2vector/Interpol.h"
#include "bmp2vector/Interpol_util.h"
#include "bmp2vector/Graph.h"
#include "bmp2vector/GeoJson.h"
#include "bmp2vector/CutSegment.h"
//#include "Graph.cpp"

#ifndef DEBUGTRACE
#define DEBUGTRACE
#endif


//#define DEBUG_MAIN


#ifdef DEBUG_MAIN
#define DEBUG_MAIN_PRINT(arg) arg
#else
#define DEBUG_MAIN_PRINT(arg)
#endif

std::vector<dcoord> v_icoord2dcoord(std::vector<icoord> &vIn) {
    std::vector<dcoord> vOut(vIn.size());

    for (uint i = 0; i < vIn.size(); i++) {
        vOut[i] = dcoord(vIn[i]);
    }

    return vOut;
}


void test0(const std::string &inFile, const std::string &outFile, bool drawRef) {
    Interpol::interpolRecursionCount = 0;

    BMP inputBmp;
    std::string inputString = inFile;

    inputBmp.ReadFromFile(inputString.c_str());

    imageWrapper w_imWrapper(&inputBmp);

    DEBUG_MAIN_PRINT(std::cout << w_imWrapper.getWidth() << " " << w_imWrapper.getHeight() << "\n");

    //w_imWrapper.getNeighbors(coord(369, 369));


    w_imWrapper.clusterPixels();
    w_imWrapper.computeTriplePoints();
    w_imWrapper.computeBorderVertices();
    w_imWrapper.computeBorders();
    w_imWrapper.computeClusterBorders_p1();
    w_imWrapper.computeClusterBorders_p2();


    Geom::Svg svg(coord(w_imWrapper.getHeight() + 1, w_imWrapper.getWidth() + 1));

    std::ofstream svgFile;
    svgFile.open(outFile);

    std::vector<Geom::Path> v_path(w_imWrapper.m_borders.size());
    std::vector<Geom::Path> v_pathInterpol(w_imWrapper.m_borders.size());

    Color red = {255, 0, 0};
    Color blue = {0, 0, 255};

    for (uint i = 0; i < w_imWrapper.m_borders.size(); i++) {
        std::vector<icoord> &v = w_imWrapper.m_borders[i].m_coords;

        if (drawRef) {
            v_path[i].addLines(v_icoord2dcoord(w_imWrapper.m_borders[i].m_coords));
            v_path[i].strokeWidth = 0.2;
            v_path[i].strokeColor = red;
            svg.shapes.push_back(std::make_shared<Geom::Path>(v_path[i]));
        }

        v_pathInterpol[i].addMoveTo(v[0]);
        v_pathInterpol[i].append(Interpol::interpol_sub(v));
        v_pathInterpol[i].strokeWidth = 0.2;
        v_pathInterpol[i].strokeColor = blue;
        svg.shapes.push_back(std::make_shared<Geom::Path>(v_pathInterpol[i]));
    }

    svg.writeToFile(svgFile);
}

void makeSVG(const std::string &inFile, const std::string &outFile, bool drawRef) {
    Interpol::interpolRecursionCount = 0;

    BMP inputBmp;
    std::string inputString = inFile;

    inputBmp.ReadFromFile(inputString.c_str());

    imageWrapper w_imWrapper(&inputBmp);

    std::cout << w_imWrapper.getWidth() << " " << w_imWrapper.getHeight() << "\n";

    //w_imWrapper.getNeighbors(coord(369, 369));


    w_imWrapper.clusterPixels();
    w_imWrapper.computeTriplePoints();
    w_imWrapper.computeBorderVertices();
    w_imWrapper.computeBorders();
    w_imWrapper.computeClusterBorders_p1();
    w_imWrapper.buildBorderGraphs();
    w_imWrapper.computeClusterBorders_p2();

    //w_imWrapper.run();


    Geom::Svg svg(coord(w_imWrapper.getHeight() + 1, w_imWrapper.getWidth() + 1));

    std::ofstream svgFile;
    svgFile.open(outFile);


    std::vector<Geom::Path> v_path(w_imWrapper.m_borders.size());
    std::vector<Geom::Path> v_pathInterpol(w_imWrapper.m_clusters.size());

    Color red = {255, 0, 0};
    Color blue = {0, 0, 255};
    Color black = {0, 0, 0};


    uint maxHeight = 0;
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        if (w_imWrapper.getPixelClusterById(i).height > maxHeight) {
            maxHeight = w_imWrapper.getPixelClusterById(i).height;
        }
    }

    std::vector<std::vector<int>> clusterIdsByHeight(maxHeight + 1);
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        clusterIdsByHeight[w_imWrapper.getPixelClusterById(i).height].push_back(i);
    }

    std::vector<int> clusterIdsToDraw;
    for (uint h = 0; h < clusterIdsByHeight.size(); h++) {
        for (uint i = 0; i < clusterIdsByHeight[h].size(); i++) {
            clusterIdsToDraw.push_back(clusterIdsByHeight[h][i]);
        }
    }

    //for (auto curClusterPair : w_imWrapper.m_clusters)
    //while(clusterIdsToDraw.size() > 0)
    for (uint i = 0; i < clusterIdsToDraw.size(); i++) {
        int clusterId = clusterIdsToDraw[i];
        //clusterIdsToDraw.pop();

        auto curCluster = w_imWrapper.getPixelClusterById(clusterId);
        //uint clusterId = curCluster.id;
        DEBUG_MAIN_PRINT(std::cout << "clusterId: " << clusterId << "\n";)
        //auto curCluster = w_imWrapper.m_clusters[0];

        uint from = 0;
        DEBUG_MAIN_PRINT(std::cout << "orderedBorders: ";
                                 for (auto border: curCluster.principalConnex) { std::cout << border.id << " "; }
                                 std::cout << "\n";)

        for (uint borderId = 0; borderId < std::max(std::vector<::imageWrapper::OrientedId>::size_type(0),
                                                    curCluster.principalConnex.size()); borderId++) {
            DEBUG_MAIN_PRINT(std::cout << "\tborderId: " << curCluster.principalConnex[borderId].id << "\n";)
            auto curBorderRealId = curCluster.principalConnex[borderId].id;
            std::vector<icoord> &v = w_imWrapper.m_borders[curBorderRealId].m_coords;
            Geom::Path curBorderPath = Interpol::interpol(v);


            if (curCluster.principalConnex[borderId].direction == E_FRONT) {
                v_pathInterpol[clusterId].appendFrom(curBorderPath, from);
            } else {
                v_pathInterpol[clusterId].appendFrom(curBorderPath.reverse(), from);
            }
            if (from == 0) { from = 1; }
        }

        v_pathInterpol[clusterId].strokeWidth = 0.2;//std::max(0.1, 0.5 * (255 - 100 * d)/255);
        v_pathInterpol[clusterId].strokeColor = black;

        coord origin = curCluster.origin;
        auto clusterColor = w_imWrapper.getPixel(origin.x, origin.y);

        v_pathInterpol[clusterId].fillColor = Color{clusterColor.Red, clusterColor.Green, clusterColor.Blue};
        svg.shapes.push_back(std::make_shared<Geom::Path>(v_pathInterpol[clusterId]));
    }
    svg.writeToFile(svgFile);
}
std::string makeSVGstring(const std::string &input) {
    Interpol::interpolRecursionCount = 0;

    BMP inputBmp;
    inputBmp.ReadFromString(input);

    imageWrapper w_imWrapper(&inputBmp);

    std::cout << w_imWrapper.getWidth() << " " << w_imWrapper.getHeight() << "\n";

    //w_imWrapper.getNeighbors(coord(369, 369));


    w_imWrapper.clusterPixels();
    w_imWrapper.computeTriplePoints();
    w_imWrapper.computeBorderVertices();
    w_imWrapper.computeBorders();
    w_imWrapper.computeClusterBorders_p1();
    w_imWrapper.buildBorderGraphs();
    w_imWrapper.computeClusterBorders_p2();

    //w_imWrapper.run();


    Geom::Svg svg(coord(w_imWrapper.getHeight() + 1, w_imWrapper.getWidth() + 1));

    std::vector<Geom::Path> v_path(w_imWrapper.m_borders.size());
    std::vector<Geom::Path> v_pathInterpol(w_imWrapper.m_clusters.size());

    Color red = {255, 0, 0};
    Color blue = {0, 0, 255};
    Color black = {0, 0, 0};


    uint maxHeight = 0;
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        if (w_imWrapper.getPixelClusterById(i).height > maxHeight) {
            maxHeight = w_imWrapper.getPixelClusterById(i).height;
        }
    }

    std::vector<std::vector<int>> clusterIdsByHeight(maxHeight + 1);
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        clusterIdsByHeight[w_imWrapper.getPixelClusterById(i).height].push_back(i);
    }

    std::vector<int> clusterIdsToDraw;
    for (uint h = 0; h < clusterIdsByHeight.size(); h++) {
        for (uint i = 0; i < clusterIdsByHeight[h].size(); i++) {
            clusterIdsToDraw.push_back(clusterIdsByHeight[h][i]);
        }
    }

    //for (auto curClusterPair : w_imWrapper.m_clusters)
    //while(clusterIdsToDraw.size() > 0)
    for (uint i = 0; i < clusterIdsToDraw.size(); i++) {
        int clusterId = clusterIdsToDraw[i];
        //clusterIdsToDraw.pop();

        auto curCluster = w_imWrapper.getPixelClusterById(clusterId);
        //uint clusterId = curCluster.id;
        DEBUG_MAIN_PRINT(std::cout << "clusterId: " << clusterId << "\n";)
        //auto curCluster = w_imWrapper.m_clusters[0];

        uint from = 0;
        DEBUG_MAIN_PRINT(std::cout << "orderedBorders: ";
                                 for (auto border: curCluster.principalConnex) { std::cout << border.id << " "; }
                                 std::cout << "\n";)

        for (uint borderId = 0; borderId < std::max(std::vector<::imageWrapper::OrientedId>::size_type(0),
                                                    curCluster.principalConnex.size()); borderId++) {
            DEBUG_MAIN_PRINT(std::cout << "\tborderId: " << curCluster.principalConnex[borderId].id << "\n";)
            auto curBorderRealId = curCluster.principalConnex[borderId].id;
            std::vector<icoord> &v = w_imWrapper.m_borders[curBorderRealId].m_coords;
            Geom::Path curBorderPath = Interpol::interpol(v);


            if (curCluster.principalConnex[borderId].direction == E_FRONT) {
                v_pathInterpol[clusterId].appendFrom(curBorderPath, from);
            } else {
                v_pathInterpol[clusterId].appendFrom(curBorderPath.reverse(), from);
            }
            if (from == 0) { from = 1; }
        }

        v_pathInterpol[clusterId].strokeWidth = 0.2;//std::max(0.1, 0.5 * (255 - 100 * d)/255);
        v_pathInterpol[clusterId].strokeColor = black;

        coord origin = curCluster.origin;
        auto clusterColor = w_imWrapper.getPixel(origin.x, origin.y);

        v_pathInterpol[clusterId].fillColor = Color{clusterColor.Red, clusterColor.Green, clusterColor.Blue};
        svg.shapes.push_back(std::make_shared<Geom::Path>(v_pathInterpol[clusterId]));
    }
    return svg.writeToString();
}

void test2() {
    Geom::Path path;

    auto m = std::make_shared<Geom::MoveTo>(icoord(2, 2));

    path.addCommand(m);
    path.addCommand(std::make_shared<Geom::QuadraticBezierTo>(icoord(2, 4), dcoord(4, 4)));
    path.addCommand(std::make_shared<Geom::QuadraticBezierTo>(icoord(6, 4), dcoord(8, 8)));
    path.addCommand(std::make_shared<Geom::LineTo>(icoord(12, 12)));


    for (auto pc: path.commands) {
        std::cout << pc->draw() << " ";
    }
    std::cout << "\n";

    auto reversePath = path.reverse();

    std::cout << "reversePath:\n";

    for (auto pc: reversePath.commands) {
        std::cout << pc->draw() << " ";
    }
    std::cout << "\n";


}


std::string makeGeojsonString(const std::string& input) {
    GeoJson::GeoJsonFile gfile;

    std::map<std::string, std::shared_ptr<GeoJson::Feature>> uniqueColorFeatures;

    BMP inputBmp;
    std::cout<<"t";
    inputBmp.ReadFromString(input);
    imageWrapper w_imWrapper(&inputBmp);
    std::cout << w_imWrapper.getWidth() << " " << w_imWrapper.getHeight() << "\n";
    w_imWrapper.run();
    std::vector<Geom::Path> v_pathInterpol(w_imWrapper.m_clusters.size());
    uint maxHeight = 0;
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        if (w_imWrapper.getPixelClusterById(i).height > maxHeight) {
            maxHeight = w_imWrapper.getPixelClusterById(i).height;
        }
    }
    std::vector<std::vector<int>> clusterIdsByHeight(maxHeight + 1);
    for (uint i = 0; i < w_imWrapper.m_nbClusters; i++) {
        clusterIdsByHeight[w_imWrapper.getPixelClusterById(i).height].push_back(i);
    }
    std::vector<int> clusterIdsToDraw;
    for (uint h = 0; h < clusterIdsByHeight.size(); h++) {
        for (uint i = 0; i < clusterIdsByHeight[h].size(); i++) {
            clusterIdsToDraw.push_back(clusterIdsByHeight[h][i]);
        }
    }
    for (uint i = 0; i < clusterIdsToDraw.size(); i++) {
        int clusterId = clusterIdsToDraw[i];
        //clusterIdsToDraw.pop();

        auto curCluster = w_imWrapper.getPixelClusterById(clusterId);
        //uint clusterId = curCluster.id;
        DEBUG_MAIN_PRINT(std::cout << "clusterId: " << clusterId << "\n");
        //auto curCluster = w_imWrapper.m_clusters[0];

        uint from = 0;
        DEBUG_MAIN_PRINT(std::cout << "orderedBorders: ");
        for (auto cluster: curCluster.principalConnex) { std::cout << cluster.id << " "; }
        std::cout << "\n";

        std::vector<imageWrapper::OrientedId> *connexPointer;
        connexPointer = &curCluster.principalConnex;

        for (uint borderId = 0; borderId < std::max(std::vector<::imageWrapper::OrientedId>::size_type(0),
                                                    (*connexPointer).size()); borderId++) {
            DEBUG_MAIN_PRINT(std::cout << "\tborderId: " << (*connexPointer)[borderId].id << "\n");
            auto curBorderRealId = (*connexPointer)[borderId].id;
            std::vector<icoord> &v = w_imWrapper.m_borders[curBorderRealId].m_coords;
            Geom::Path curBorderPath = Interpol::interpol(v);


            if ((*connexPointer)[borderId].direction == E_FRONT) {
                v_pathInterpol[clusterId].appendFrom(curBorderPath, from);
            } else {
                v_pathInterpol[clusterId].appendFrom(curBorderPath.reverse(), from);
            }
            if (from == 0) { from = 1; }
        }

        std::vector<Geom::Path> internalPolys;
        for (uint ip = 0; ip < curCluster.internalConnexes.size(); ip++) {
            DEBUG_MAIN_PRINT(std::cout << "internal path number " << ip << "\n");
            connexPointer = &curCluster.internalConnexes[ip];
            internalPolys.push_back(Geom::Path());
            internalPolys.back();
            from = 0;
            for (uint borderId = 0; borderId < std::max(std::vector<::imageWrapper::OrientedId>::size_type(0),
                                                        (*connexPointer).size()); borderId++) {
                DEBUG_MAIN_PRINT(std::cout << "\tborderId: " << (*connexPointer)[borderId].id << "\n");
                auto curBorderRealId = (*connexPointer)[borderId].id;
                std::vector<icoord> &v = w_imWrapper.m_borders[curBorderRealId].m_coords;
                Geom::Path curBorderPath = Interpol::interpol(v);


                if ((*connexPointer)[borderId].direction == E_FRONT) {
                    internalPolys.back().appendFrom(curBorderPath, from);
                } else {
                    internalPolys.back().appendFrom(curBorderPath.reverse(), from);
                }
                if (from == 0) { from = 1; }
            }

            DEBUG_MAIN_PRINT(std::cout << "internal path number " << ip << " done\n");

        }


        DEBUG_MAIN_PRINT(std::cout << "internal paths completed\n");

        v_pathInterpol[clusterId].strokeWidth = 0.2;//std::max(0.1, 0.5 * (255 - 100 * d)/255);

        coord origin = curCluster.origin;
        auto clusterColor = w_imWrapper.getPixel(origin.x, origin.y);

        v_pathInterpol[clusterId].fillColor = Color{clusterColor.Red, clusterColor.Green, clusterColor.Blue};

        auto ptPoly = std::make_shared<GeoJson::Polygon>();
        ptPoly->principalConnex = std::make_shared<Geom::Path>(v_pathInterpol[clusterId]);


        DEBUG_MAIN_PRINT(std::cout << "start adding internal polys...\n");
        for (uint ip = 0; ip < curCluster.internalConnexes.size(); ip++) {
            DEBUG_MAIN_PRINT(std::cout << "adding internal poly " << ip << "\n");

            ptPoly->internalConnexes.push_back(std::make_shared<Geom::Path>(internalPolys[ip]));
            DEBUG_MAIN_PRINT(std::cout << "adding internal poly " << ip << " done\n");
        }
        DEBUG_MAIN_PRINT(std::cout << "end adding internal polys...\n");

        std::shared_ptr<GeoJson::Feature> ptFeature;

        std::string hex = v_pathInterpol[clusterId].fillColor.hexString();

        if (uniqueColorFeatures.find(hex) == uniqueColorFeatures.end()) {
            DEBUG_MAIN_PRINT(std::cout << "new hex: " << hex << "\n");

            ptFeature = std::make_shared<GeoJson::Feature>();
            ptFeature->properties = std::map<std::string, std::string>();
            ptFeature->properties.insert(std::pair<std::string, std::string>("hex", "\"" +
                                                                                    v_pathInterpol[clusterId].fillColor.hexString() +
                                                                                    "\""));
            ptFeature->geometry = std::make_shared<GeoJson::MultiPolygon>();

            uniqueColorFeatures.emplace(std::pair<std::string, std::shared_ptr<GeoJson::Feature>>(hex, ptFeature));
            gfile.features.push_back(ptFeature);
        } else {
            DEBUG_MAIN_PRINT(std::cout << "existing hex: " << hex << "\n");
            ptFeature = uniqueColorFeatures.at(hex);
        }
        dynamic_cast<GeoJson::MultiPolygon *>(ptFeature->geometry.get())->polygons.push_back(ptPoly);
    };
    std::cout<<gfile.features.size();
    return gfile.writeToString();
}

void _displayHelp() {
    std::cout << "Two arguments: path_to_bmp_input path_to_geojson_output\n";
}