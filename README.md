# Skanderbeg Dev Kit

Skanderbeg Dev Kit is a small toolkit written in C++ 
for dealing with some processing-intensive task related to 
working with Paradox Interactive's game and datafiles.


## Modules

- clausewitz2parse is an efficient parser for save and gamefiles saved using Clausewitz Engine's syntax. 
The parser recursively loops through each object and transforms it into an AST node. The AST can then be 
serialized into either JSON or MsgPack. 
- ironman2text is a binary decoder for "ironman" savefiles from Clausewitz Engine's games. 
These savefiles use a format wherein all keys are assigned an arbitrary index on an ironman dictionary(which is game specific)
and each value type has its own mean of being serialized into binary form. The decoder takes the contents of an ironman file, and 
returns it in a plaintext form. (which can be later on processed by clausewitz2parse).
- bmp2vector was largely authored by @Milith. It takes a .bmp
  from a Paradox game as an input (for instance, provinces.bmp or a F10 map from EU4)
  and vectorizes it. The output - in a geojson form - is a basis for map display on Skanderbeg.
  However, bmp2vector can also output into a SVG file easily readable by all modern browsers, and which
  can be easily transformed into a raster image format of any size.


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