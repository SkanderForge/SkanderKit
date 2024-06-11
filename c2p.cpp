#include "functions.h"
#include <iostream>
#include <thread>
#include <fstream>


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::string json = cfile2json(R"(test/input.eu4)");

    std::ofstream o("test.json");
    o << json;
    o.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration2 = duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Entire program run: " << duration2.count() << std::endl;

}