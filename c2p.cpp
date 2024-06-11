#include "functions.h"
#include <iostream>
#include <thread>
#include <fstream>


int main() {
    auto start = std::chrono::high_resolution_clock::now();
//    for(int i = 0; i < 10; i++) {
//        //std::string json = cfile2json("E:/Programming stuff/skanderbeg-api/providers/test.txt");
//                std::string json = cfile2json("c:/Users/chiny/Documents/Paradox Interactive/Europa Universalis IV/save games/Castile1458_02_02.eu4");
//    }
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = duration_cast<std::chrono::milliseconds>(end - start);
//    std::cout << "Execution time: " << duration.count() << std::endl;
    // std::string json = cfile2json("E:/Programming stuff/skanderbeg-api/providers/test.txt");
    std::string json = cfile2json(R"(c:\Users\chiny\Documents\Paradox Interactive\Victoria 3\save games\japanese shogunate_1836_01_01\gamestate)");
     //   std::string json = cfile2json(R"(c:\Users\chiny\Downloads\Meshchyora1821_01_03\gamestate)");


    //std::string json = cfile2json("c:/Users/chiny/Documents/Paradox Interactive/Victoria 3/save games/japanese shogunate_1836_01_01/gamestate");

    std::ofstream o("test.json");
    o << json;
    o.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration2 = duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Entire program run: " << duration2.count() << std::endl;



}