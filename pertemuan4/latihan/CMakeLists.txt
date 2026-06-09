cmake_minimum_required(VERSION 3.20)
project(latihan)

set(CMAKE_CXX_STANDARD 17)

add_executable(latihan main.cpp)

target_include_directories(latihan PRIVATE
        "C:/mingw64/include"
)

target_link_libraries(latihan
        "C:/mingw64/lib/libfreeglut.a"
        opengl32
        glu32
)