# Skanderbeg Dev Kit

Skanderbeg Dev Kit is a small toolkit written in C++ 
for dealing with some processing-intensive task related to 
working with Paradox Interactive's game and datafiles.

### Features

All functions here are contained in functions.h/.cpp file.

| Function name | Expected input                                                                     | Output                                         |
|---------------|------------------------------------------------------------------------------------|------------------------------------------------|
| c2json        | String representing contents of a file  serialized in PDX's syntax(ie. a savefile) | String containing a JSON object                |
| c2msgpack     | String representing contents of a file  serialized in PDX's syntax(ie. a savefile) | Binary string containing a MsgPack object      |
| bmp2svg       | Binary string representing contents of a bmp file                                  | String containing a HTML document with the SVG |
| bmp2geojson   | Binary string representing contents of a bmp file                                  | String containing a (geo)JSON object           |

## Why strings everywhere? 

Skanderbeg Kit is supposed to be exported into a WASM module. 
WASM modules are ran in a browser, which sandboxes all of its operation,
preventing us from accessing the filesystem directly, hence operating 
on strings is the best & only way to deal with this.

## Limitations of the serializers

Paradox's syntax has a lot of quirks, and overall it doesn't really fully 
conform to any modern data exchange formats. 
An example of a quirk:
> { a = b 0 1 2 3 4 5 6 }  - a map and an array inside one object.

Even though the parsing is generally precise. Serialization of the parsed data isn't fully losless. 
If you are interested in adjusting  how the kit serializes objects, refer to: 
> src/clausewitz2parse/Node.cpp 


## Building 

As a prerequisite. You have to have Boost installed on your system to be able to
build this tool. Once you have it, make sure to change the include dir in CMakeLists.txt 

To build the kit into a WASM module, you have to have Emscripten installed. Specifically, 
make sure that you have the location of a "Emscripten.cmake" toolchain file. 


Afterwards run the following 
> -DCMAKE_TOOLCHAIN_FILE=.../emscripten/cmake/Modules/Platform/Emscripten.cmake
> -Dis_wasm=true

Running CMake without setting the is_wasm flag, will result in it building 
from main.cpp, which can be used to test the functionality of the tool from the CLI 
or for building a CLI/lib version of the tool.