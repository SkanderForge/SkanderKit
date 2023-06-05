#include <emscripten/bind.h>

#include "functions.cpp"
int main(){
    std::cout <<"Test"<<std::endl;
}

EMSCRIPTEN_BINDINGS(wasm_c2p) {
    emscripten::function("c2msgpack", &c2msgpack);
    emscripten::function("c2json", &c2json);
    emscripten::function("bmp2geojson", &bmp2geojson);
    emscripten::function("bmp2svg", &bmp2svg);
}