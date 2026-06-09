cmake_minimum_required(VERSION 3.20)
project(pertemuan4)

set(CMAKE_CXX_STANDARD 17)

add_executable(pertemuan4 main.cpp)

target_include_directories(pertemuan4 PRIVATE
        "C:/mingw64/include"
)

target_link_libraries(pertemuan4
        "C:/mingw64/lib/libfreeglut.a"
        opengl32
        glu32
)