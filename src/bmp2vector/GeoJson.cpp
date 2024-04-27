#include "../bmp2vector/GeoJson.h"
#include <sstream>

#ifdef DEBUG_GEOJSON
#define DEBUG_GEOJSON_PRINT(arg) arg
#else
#define DEBUG_GEOJSON_PRINT(arg)  
#endif

namespace GeoJson
{
    std::string Polygon::writeConnex(const Geom::Path& p)
    {

        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::writeConnex BEGIN\n";)
        std::string s;
        if (p.commands.size() > 0)
        {
            for (uint i = 0; i < p.commands.size() - 1; i++)
            {
                s += ("[" + std::to_string(p.commands[i]->to.x) + "," + std::to_string(-p.commands[i]->to.y) + "],");
            }
            s += ("[" + std::to_string(p.commands.front()->to.x) + "," + std::to_string(-p.commands.front()->to.y) + "]");
        }

        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::writeConnex END\n";)
        return s;
    }

    std::string Polygon::write_sub()
    {

        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::write_sub BEGIN\n";)
        std::string s;
        s += "[";
        s += writeConnex(*principalConnex);
        s += "]";
        if (internalConnexes.size() > 0)
        {

            for (uint i = 0; i < internalConnexes.size(); i++)
            {
                s += ",";
                s += "[";
                s += writeConnex(*internalConnexes[i]);
                s += "]";
            }
        }

        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::write_sub END\n";)
        return s;
    }

    std::string Polygon::write()
    {

        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::write BEGIN\n";)

        std::string s;
        s += "\"type\": \"Polygon\",\n";
        s += "\"coordinates\": [\n";
        s += write_sub();
        s += "]";


        DEBUG_GEOJSON_PRINT(std::cout << "Polygon::write END\n";)
        return s;
    }

    std::string MultiPolygon::write()
    {
        DEBUG_GEOJSON_PRINT(std::cout << "nb polygons:" << polygons.size() << "\n";)
        
        std::string s;
        DEBUG_GEOJSON_PRINT(std::cout << "MultiPolygon::write BEGIN\n";)
        if (polygons.size() <= 1)
        {
            s += "\"type\": \"Polygon\",\n";
            s += "\"coordinates\": [\n";
            s += polygons[0]->write_sub();
            s += "]";
        }
        else
        {
            s += "\"type\": \"MultiPolygon\",\n";
            s += "\"coordinates\": [\n";
            for (auto polygon : polygons)
            {
                s += "[";
                s += polygon->write_sub();
                s += +"]";
                if (polygon != polygons.back()) {
                    s += ",\n";
                }
            }
            s += "]";
        }

        DEBUG_GEOJSON_PRINT(std::cout << "MultiPolygon::write END\n";)

        return s;
    }

    std::string Feature::write()
    {

        DEBUG_GEOJSON_PRINT(std::cout << "Feature::write BEGIN\n";)

        std::string s;
        s += "{\n";
        s += "\"type\": \"Feature\",\n";
        s += "\"geometry\": { \n";

        s += geometry->write();

        s += "\n}\n";
        if (properties.size() > 0)
        {
            s += ",\n";
            s += "\"properties\":{\n";

            uint nbProperties = 0;

            for (auto pair : properties)
            {
                nbProperties++;
                s += "\"" + pair.first + "\":" + pair.second;
                if (nbProperties < properties.size()) { s += ","; }
                s += "\n";
            }

            s += "}\n";
        }
        s += "}\n";


        DEBUG_GEOJSON_PRINT(std::cout << "Feature::write END\n";)
        return s;
    }

    void GeoJsonFile::writeToFile(std::ofstream& ai_fileStream)
    {
        DEBUG_GEOJSON_PRINT(std::cout << "GeoJsonFile::writeToFile BEGIN\n";)

        ai_fileStream << "var provinces = {\n";
        ai_fileStream << "\"type\":\"FeatureCollection\",\n";
        ai_fileStream << "\"features\":[\n";
        for (uint i = 0; i < features.size(); i++ )
        {
            ai_fileStream << features[i]->write();
            if (i < features.size() - 1) { ai_fileStream << ","; }
        }
        ai_fileStream << "]};\n";

        DEBUG_GEOJSON_PRINT(std::cout << "GeoJsonFile::writeToFile END\n";)
    }
    std::string GeoJsonFile::writeToString()
    {
        std::stringstream buffer;
        DEBUG_GEOJSON_PRINT(std::cout << "GeoJsonFile::writeToFile BEGIN\n";)

        buffer << "{\n";
        buffer << "\"type\":\"FeatureCollection\",\n";
        buffer << "\"features\":[\n";
        for (uint i = 0; i < features.size(); i++ )
        {
            buffer << features[i]->write();
            if (i < features.size() - 1) { buffer << ","; }
        }
        buffer << "]};\n";

        DEBUG_GEOJSON_PRINT(std::cout << "GeoJsonFile::writeToFile END\n";)
        return buffer.str();
    }
}