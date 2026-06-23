cmake_minimum_required(VERSION 4.2)
project(UAS)

set(CMAKE_CXX_STANDARD 20)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_executable(UAS main.cpp)

target_link_libraries(UAS
        ${GLUT_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

target_compile_definitions(UAS PRIVATE GLUT_DISABLE_ATEXIT_HACK)
