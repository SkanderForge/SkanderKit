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

