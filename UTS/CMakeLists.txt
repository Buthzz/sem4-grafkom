cmake_minimum_required(VERSION 3.20)
project(UTS)

set(CMAKE_CXX_STANDARD 20)

add_executable(UTS main.cpp)

target_include_directories(UTS PRIVATE
        "C:/mingw64/include"
)

target_link_libraries(UTS
        "C:/mingw64/lib/libfreeglut.a"
        opengl32
        glu32
)