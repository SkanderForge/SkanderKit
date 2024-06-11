#include "includes/clausewitz2parse/PlainTextLexer.h"
#include "includes/clausewitz2parse/Token.h"
#include "includes/clausewitz2parse/Parser.h"
#include <fstream>
#include "includes/bmp2vector/Entry.h"
#include "functions.h"
#include <chrono>
#include "includes/rapidjson/Document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

#include "includes/clausewitz2parse/Dictionaries.h"

//std::string c2msgpack(const std::string &input) {
//    Parser parser(input);
//    std::unique_ptr<Node> ast = parser.parse();
//    auto msg = ast->toMsgPack();
//    std::stringstream buffer;
//    msgpack::pack(buffer, msg);
//    return buffer.str();
//}

std::string c2json(const std::string &input) {
    Parser parser(input);
    std::unique_ptr<Node> ast = parser.parse();
    std::string json = ast->toJSON();
    return json;
}

std::string read_binary_string_from_file(const std::string &filename, size_t offset, size_t length) {
    // Error handling
    if (length == 0) {
        throw std::invalid_argument("Length cannot be zero.");
    }

    // Open the file in binary mode
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    // Seek to the specified offset
    file.seekg(offset);
    if (!file.good()) {
        throw std::runtime_error("Error seeking in the file.");
    }

    // Read the bytes into a buffer
    std::vector<char> buffer(length);
    file.read(buffer.data(), length);

    // Check if the expected number of bytes were read
    if (file.gcount() != length) {
        throw std::runtime_error("Could not read the specified number of bytes.");
    }

    // Construct a string from the buffer
    return std::string(buffer.begin(), buffer.end());
}

std::string cfile2json(const std::string &inputSrc) {

    char three = static_cast<char>(3);
    char zero = static_cast<char>(0);
    char four = static_cast<char>(4);
    std::ifstream t(inputSrc);
    auto start = std::chrono::high_resolution_clock::now();

    std::stringstream ss;
    //ss << t.rdbuf();
    std::string content((std::istreambuf_iterator<char>(t)), (std::istreambuf_iterator<char>()));
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Preparation time: " << duration.count() << std::endl;


    int ch1 = static_cast<uint8_t>(content[0]);
    int ch2 = static_cast<uint8_t>(content[1]);
    int ch3 = static_cast<uint8_t>(content[2]);
    int ch4 = static_cast<uint8_t>(content[3]);

    std::cout << ch1 << ch2 << std::endl;


    rapidjson::Document doc;
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);



    //Victoria 3 ironman
    if (ch1 == 173 && ch2 == 85) {
        std::ifstream t(inputSrc, std::ios::binary);
        t.seekg(0, std::ios::end);
        std::string input = read_binary_string_from_file(inputSrc, 0, t.tellg());
        // input.erase(0, 6);
        input.insert(input.begin(), zero);
        input.insert(input.begin(), three);
        input.push_back(four);
        input.push_back(zero);
        std::cout << "Debug char:" << (int) input[0] << (int) input[1] << (int) input[2] << (int) input[3];

        auto ast = b2ast(input, "vic3");
        rapidjson::Value parsedDoc = ast->toJSONValue(doc);
        parsedDoc.Accept(writer);

        return buffer.GetString();

    }
    //EU4 ironman. We need to read up to the 4th character (t) to not confuse it with plaintext
    if (ch1 == 69 && ch2 == 85 && ch3 == 52 && ch4 == 98) {
        std::ifstream t(inputSrc, std::ios::binary);
        t.seekg(0, std::ios::end);
        std::string input = read_binary_string_from_file(inputSrc, 0, t.tellg());
        input.erase(0, 6);
        input.insert(input.begin(), zero);
        input.insert(input.begin(), three);
        input.push_back(four);
        input.push_back(zero);

        auto ast = b2ast(input, "eu4");
        rapidjson::Value parsedDoc = ast->toJSONValue(doc);
        parsedDoc.Accept(writer);
        return buffer.GetString();
    }
    //CK3 ironman
    if (ch1 == 85 && ch2 == 49) {
        std::ifstream t(inputSrc, std::ios::binary);
        t.seekg(0, std::ios::end);
        std::string input = read_binary_string_from_file(inputSrc, 0, t.tellg());
        input.erase(0, 6);
        input.insert(input.begin(), zero);
        input.insert(input.begin(), three);
        input.push_back(four);
        input.push_back(zero);

        auto ast = b2ast(input, "ck3");
        rapidjson::Value parsedDoc = ast->toJSONValue(doc);
        parsedDoc.Accept(writer);
        return buffer.GetString();
    }

    Parser parser(content);
    std::unique_ptr<Node> ast = parser.parse();
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = duration_cast<std::chrono::microseconds>(end1 - end);
    std::cout << "Parsing time time: " << duration1.count() << std::endl;


    rapidjson::Value parsedDoc = ast->toJSONValue(doc);
    parsedDoc.Accept(writer);


    auto end2 = std::chrono::high_resolution_clock::now();

    auto duration2 = duration_cast<std::chrono::microseconds>(end2 - end1);
    std::cout << "Generating JSON time: " << duration2.count() << std::endl;


    return buffer.GetString();
}


std::shared_ptr<Node> b2ast(const std::string &input, const std::string &game) {
    std::stringstream ss;
    ss.str(input);


    Parser parser(ss, game);

    std::shared_ptr<Node> ast = parser.parse();
    return ast;
}

std::shared_ptr<Node> c2ast(const std::string &input) {
    Parser parser(input);
    std::shared_ptr<Node> ast = parser.parse();
    return ast;
}

std::string bmp2geojson(const std::string &input) {
    return makeGeojsonString(input);
}

std::string bmp2svg(const char &input) {
    return makeSVGstring((const char *&) input);
}


//std::string charPtrToHexStr(const char *input) {
//    std::stringstream ss;
//    ss << std::hex;
//
//    while (*input) {
//        // Convert the current character to its hex value and append to the result
//        ss << std::setw(2) << static_cast<int>(*input);
//        ++input; // Move to the next character
//    }
//
//    return ss.str();
//}

std::string readBytesAsHex(std::ifstream &file, std::streamsize numBytes) {
    if (!file || numBytes <= 0) {
        return ""; // Return an empty string on error or invalid input
    }

    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0') << std::uppercase;

    char buffer;
    for (std::streamsize i = 0; i < numBytes && file.get(buffer); ++i) {
        hexStream << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(buffer));
    }

    return hexStream.str();
}

std::string lookupBytes(std::ifstream &file, int length) {
    auto pos = file.tellg();

    auto bytes = readBytesAsHex(file, length);
    file.seekg(pos);

    return bytes;
}


std::string binary2txt(const std::string &input) {
    std::ifstream f(input,
            //  R"(c:\Users\jarvi\Documents\Paradox Interactive\Europa Universalis IV\save games\Research\gamestate)",
                    std::ios::binary);
    if (!f) {
        std::cerr << "Failed to open input file" << std::endl;
        return "";
    }

    char magic[2];
    std::string header = readBytesAsHex(f, 2);
    std::string game = "";
    std::unordered_map<std::string, std::string> dict;
    int layer = 0;
    std::string layerString;
    std::string output;

    if (header == "AD55") {
        dict = vic3Dict;
        //    readBytesAsHex(f, 4);
        // f.seekg(0, std::ios::beg);
        output += "meta_data";
    }
    if (header == "4555") {
        readBytesAsHex(f, 4);
    }
    bool unresolvedEquals = false;
    bool finishedValue = false;
    bool inStringArray = false;
    uint16_t code;
    while (true) {
        char bytes_[2];
        f.read(bytes_, 2);
        if (f.gcount() != 2) {
            break;
        }
        code = *reinterpret_cast<uint16_t *>(bytes_);
        //  std::cout << code << std::endl;
        if (code == 12) {
            int32_t val; // Assuming 4-byte integer (32 bits)
            f.read(reinterpret_cast<char *>(&val), sizeof(val));
            if (!unresolvedEquals) {
                output += layerString;
                output += std::to_string(val);
            } else {
                output += std::to_string(val);
                unresolvedEquals = false;
                finishedValue = true;
            }
        } else if (code == 1) {
            output += '=';
            unresolvedEquals = true;
        } else if (code == 3) {
            layerString += '\t';
            if (!unresolvedEquals) {
                output += layerString;
            }
            output += "{";

            unresolvedEquals = false;
            finishedValue = true;
            layer++;
        } else if (code == 4) {
            if (!layerString.empty()) {
                layerString.pop_back();
                //    std::cout << layerString.length() << std::endl;
                //     if (layerString.length() > 25) break;
            }
            output += layerString;
            output += "}";
            finishedValue = true;
            unresolvedEquals = false;
            inStringArray = false;
            layer--;
        } else if (code == 13) {
            int32_t val; // Assuming 4-byte integer (32 bits)
            f.read(reinterpret_cast<char *>(&val), sizeof(val));
            val /= 1000;
            output += std::to_string(val);
            unresolvedEquals = false;
        } else if (code == 14) {
            bool val;
            f.read(reinterpret_cast<char *>(&val), 1);
            val ? output += "true" : output += "false";
            finishedValue = true;
            unresolvedEquals = false;
        } else if (code == 15) {
            uint16_t length; // Unsigned short (2 bytes)
            f.read(reinterpret_cast<char *>(&length), sizeof(length));
            char buffer[length + 1]; // +1 for null terminator
            f.read(buffer, length);
            buffer[length] = '\0'; // Null-terminate the string
            std::string val(buffer);
            if (!unresolvedEquals) {
                output += layerString;
                unresolvedEquals = false;
            }
            auto lookup = lookupBytes(f, 2);

            if (!inStringArray) {
                if (lookup == "0F00") inStringArray = true;
            }
            if (inStringArray) {
                val.insert(0, "\"");
                val += "\"";
                output += val;
                finishedValue = true;
            } else {
                if (unresolvedEquals) {
                    finishedValue = true;
                }
                output += "\"" + val + "\"";
            }
            unresolvedEquals = false;
        } else if (code == 20) {
            int32_t val; // Assuming 4-byte integer (32 bits)
            f.read(reinterpret_cast<char *>(&val), sizeof(val));

            if (!unresolvedEquals) {
                output += layerString;
                output += std::to_string(val);
            } else {
                output += std::to_string(val);
                unresolvedEquals = false;
                finishedValue = true;
            }
        } else if (code == 23) {
            uint16_t length;
            f.read(reinterpret_cast<char *>(&length), sizeof(length));
            char buffer[length + 1];
            f.read(buffer, length);
            buffer[length] = '\0';
            std::string val(buffer);
            output += val;
            finishedValue = true;
            unresolvedEquals = false;
        } else if (code == 359) {
            int64_t val;
            f.read(reinterpret_cast<char *>(&val), sizeof(val));
            auto floatVal = static_cast<float>(val);
            floatVal /= 100000;
            if (!unresolvedEquals) {
                output += layerString;
            }
            output += std::to_string(floatVal);
            finishedValue = true;
            unresolvedEquals = false;
        } else if (code == 668) {
            int64_t val;
            f.read(reinterpret_cast<char *>(&val), sizeof(val));
            auto floatVal = static_cast<float>(val);
            floatVal /= 100000;
            if (!unresolvedEquals) {
                output += layerString;
            }
            output += std::to_string(floatVal);
            finishedValue = true;
            unresolvedEquals = false;
        } else {
            if (!unresolvedEquals) {
                output += layerString;
            }
            if (unresolvedEquals) {
                finishedValue = true;
            }

            unresolvedEquals = false;

            std::string code_stringified = std::to_string(code);
            if (dict.contains(code_stringified)) {
                output += dict[code_stringified];
            } else {
                output += "UNKNOWN_" + code_stringified;
            }
        }
        if (finishedValue) {
            output += '\n';
            finishedValue = false;
        }
    }
    std::ofstream out("output.txt");
    out << output;
    out.close();
    std::string json = c2json(output);

    std::ofstream outj("outputj.txt");
    outj << json;
    outj.close();
}
