cmake_minimum_required(VERSION 3.0.2)
project(buddhabrot)

set(CMAKE_CXX_STANDARD_REQUIRED C++14)

set(EXE buddhabrot)
set(SOURCES buddhabrot.cpp)
add_executable(${EXE} ${SOURCES})
target_link_libraries(${EXE} GL glut)