#include "functions.h"
#include <iostream>
#include <thread>
#include <fstream>


int main() {

    std::string json = cfile2json("c:/Users/chiny/Documents/Paradox Interactive/Europa Universalis IV/save games/Ardabil/gamestate");
    //std::string json = cfile2json("c:/Users/chiny/Documents/Paradox Interactive/Victoria 3/save games/japanese shogunate_1836_01_01/gamestate");

    std::ofstream o("test.json");
    o << json;
    o.close();

}