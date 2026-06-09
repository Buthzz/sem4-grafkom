cmake_minimum_required(VERSION 4.2) # Use a realistic version like 3.20+
project(Demo_3)

set(CMAKE_CXX_STANDARD 20)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_executable(Demo_3 main.cpp)

target_link_libraries(Demo_3
        ${GLUT_LIBRARIES}
        ${OPENGL_LIBRARIES}
)

target_compile_definitions(Demo_3 PRIVATE GLUT_DISABLE_ATEXIT_HACK)