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
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


if (is_wasm)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -lembind -fwasm-exceptions")
    set(CMAKE_SHARED_LINKER_FLAGS "-sALLOW_MEMORY_GROWTH=1 --bind")
    add_executable(c2p_wasm wasm_kit_c2p.cpp ${c2p_SRC} ${b2t_SRC} functions.cpp
            src/clausewitz2parse/Parser.cpp)
    add_executable(b2v_wasm wasm_kit_b2v.cpp ${b2v_SRC} functions.cpp)

    set_target_properties(c2p_wasm PROPERTIES COMPILE_FLAGS "-O3 -sSIDE_MODULE=1")
    set_target_properties(c2p_wasm PROPERTIES
            LINK_FLAGS "-s WASM=1 -fwasm-exceptions -flto  -sMAXIMUM_MEMORY=4gb -sEXPORTED_RUNTIME_METHODS=[\"FS\"] -sSTACK_SIZE=512mb -sINITIAL_MEMORY=1024mb -sMODULARIZE=1 -O3  -sASSERTIONS -sSINGLE_FILE=1 -sALLOW_MEMORY_GROWTH=1 -sEXPORT_NAME=skanderkit_c2j"
    )

    set_target_properties(b2v_wasm PROPERTIES COMPILE_FLAGS "-O3 -sSIDE_MODULE=1")
    set_target_properties(b2v_wasm PROPERTIES
            LINK_FLAGS "-s WASM=1 -fwasm-exceptions -flto  -sMAXIMUM_MEMORY=4gb -sEXPORTED_RUNTIME_METHODS=[\"FS\"] -sSTACK_SIZE=512mb -sINITIAL_MEMORY=1024mb -sMODULARIZE=1 -O3  -sASSERTIONS -sSINGLE_FILE=1 -sALLOW_MEMORY_GROWTH=1 -sEXPORT_NAME=skanderkit_b2v"
    )


    target_include_directories(c2p_wasm PRIVATE includes/clausewitz2parse)
    target_include_directories(c2p_wasm PRIVATE includes)
    target_include_directories(b2v_wasm PRIVATE includes/bmp2vector)


else ()
    add_executable(c2p c2p.cpp ${c2p_SRC} ${b2v_SRC} ${b2t_SRC} functions.h functions.cpp
            src/clausewitz2parse/eu4.cpp
            includes/clausewitz2parse/BinaryLexer.h
    )
    target_include_directories(c2p PRIVATE includes/clausewitz2parse/)
    target_include_directories(c2p PRIVATE includes/bmp2vector)
    target_include_directories(c2p PRIVATE includes)
endif ()