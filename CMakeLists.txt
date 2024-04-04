cmake_minimum_required(VERSION 3.25)
project(skanderkit)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
file(GLOB c2p_SRC
        "src/clausewitz2parse/*.cpp"
        )
file(GLOB b2v_SRC
        "src/bmp2vector/*.cpp"
        )
file(GLOB b2t_SRC
        "src/binary2text/*.cpp"
)

if (is_wasm)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -lembind")
    set(CMAKE_SHARED_LINKER_FLAGS "-sALLOW_MEMORY_GROWTH -sMAXIMUM_MEMORY=4GB")
    add_executable(wasm_kit wasm_kit.cpp ${c2j_SRC} ${b2v_SRC} ${b2t_SRC} functions.cpp
            src/clausewitz2parse/Parser.cpp
            src/binary2text/eu4.cpp
    )
    set_target_properties(wasm_kit PROPERTIES
            LINK_FLAGS "-s WASM=1 -s -sMODULARIZE=1 -sASSERTIONS -sSINGLE_FILE=1 -sALLOW_MEMORY_GROWTH -sEXPORT_NAME=skanderkit"
            )
    target_include_directories(wasm_kit PRIVATE includes/clausewitz2parse)
    target_include_directories(wasm_kit PRIVATE includes/bmp2vector)
    target_include_directories(wasm_kit PRIVATE includes)
else ()
    add_executable(c2p c2p.cpp ${c2p_SRC} ${b2v_SRC} ${b2t_SRC} functions.h functions.cpp
            src/binary2text/eu4.cpp
    )
    target_include_directories(c2p PRIVATE includes/clausewitz2parse/)
    target_include_directories(c2p PRIVATE includes/bmp2vector)
    target_include_directories(c2p PRIVATE includes/binary2text)
    target_include_directories(c2p PRIVATE includes)
endif ()
include_directories(includes/clausewitz2parse)
include_directories(includes/bmp2vector)
include_directories(lib)
include_directories(lib/msgpack)
include_directories(lib/msgpack/msgpack)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_INCLUDE_DIR "lib/boost")
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})
find_package(Boost REQUIRED)