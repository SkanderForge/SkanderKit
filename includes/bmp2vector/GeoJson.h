#ifndef GEOJSON_H
#define GEOJSON_H

#include <vector>
#include <map>

#include "Shape.h"


namespace GeoJson
{
    class Geometry
    {
    public:
        virtual std::string write() = 0;
        virtual ~Geometry() {};
    };

    class Polygon : public Geometry
    {
    public:
        std::string write();
        std::string write_sub();
    private:
        std::string writeConnex(const Geom::Path&);
    public:
        std::shared_ptr<Geom::Path> principalConnex;
        std::vector<std::shared_ptr<Geom::Path>> internalConnexes;
    };

    class MultiPolygon : public Geometry
    {
    public:
        std::string write();
    public:
        std::vector<std::shared_ptr<Polygon>> polygons;
    };

    class Feature
    {
    public:
        std::string write();

    public:
        std::shared_ptr<Geometry> geometry;
        std::map<std::string, std::string> properties;
    };

    class GeoJsonFile
    {
    public:
        void writeToFile(std::ofstream& ai_fileStream);
        std::string writeToString();
    public:
        std::vector<std::shared_ptr<Feature>> features;



    };

}

#endif
