cmake_minimum_required(VERSION 4.2) # Sesuaikan versi
project(GLUT_Template)

set(CMAKE_CXX_STANDARD 20)

# Mencari paket GLUT
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

# Menyertakan direktori header
include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_executable(GLUT_Template main.cpp
)

# Menghubungkan library ke executable
target_link_libraries(GLUT_Template ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})