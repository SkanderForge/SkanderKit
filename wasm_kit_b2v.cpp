#include <emscripten/bind.h>

#include "functions.h"


EMSCRIPTEN_BINDINGS(wasm_c2p) {
    emscripten::function("bmp2geojson", &bmp2geojson);
}
