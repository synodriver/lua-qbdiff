cmake_minimum_required(VERSION 3.25)
project(luaqbdiff C)

set(CMAKE_C_STANDARD 11)

if (CMAKE_C_COMPILER_ID MATCHES MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -openmp:llvm /experimental:c11atomics")
else ()
    include_directories(D:\\toolbox\\apps\\CLion\\ch-0\\231.9011.31\\bin\\mingw\\lib\\gcc\\x86_64-w64-mingw32\\11.2.0\\include)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
endif ()

# change this to your lua.h directory
include_directories(D:\\conda\\envs\\py310\\Library\\include ./dep/include)
# change this to your lua.lib\liblua.a directory
link_directories("D:\\conda\\envs\\py310\\Library\\lib" D:\\conda\\envs\\py310\\Library\\bin)

file(GLOB SOURCES  ./dep/src/blake2b.c ./dep/src/libqbdiff.c ./dep/src/libsais.c ./dep/src/libsais64.c)

message(${SOURCES})

add_library(qbdiff SHARED src/lqbdiff.c ${SOURCES})
target_link_libraries(qbdiff lua liblzma)
target_compile_definitions(qbdiff PUBLIC LIBQDIFF_PUBLIC_API="__declspec(dllexport)")

set_target_properties(qbdiff PROPERTIES OUTPUT_NAME qbdiff PREFIX "")
