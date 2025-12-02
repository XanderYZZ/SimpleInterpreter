cmake_minimum_required(VERSION 3.28)

project(SimpleInterpreter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(SimpleInterpreter
    src/main.cpp
    src/TokenStream.cpp
    src/Parser.cpp
)

target_include_directories(SimpleInterpreter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)