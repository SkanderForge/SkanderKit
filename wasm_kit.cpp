#include <emscripten/bind.h>

#include "functions.h"


EMSCRIPTEN_BINDINGS(wasm_c2p) {
    emscripten::function("c2msgpack", &c2msgpack);
    emscripten::function("c2json", &c2json);
    emscripten::function("c2ast", &c2ast);
    emscripten::function("bmp2geojson", &bmp2geojson);
    emscripten::function("bmp2svg", &bmp2svg);
}
