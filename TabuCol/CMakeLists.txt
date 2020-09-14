cmake_minimum_required(VERSION 3.15)
project(TabuCol)

set(CMAKE_CXX_STANDARD 17)

add_executable(TabuCol main.cpp TabuCol.cpp TabuCol.h UndiGraph.cpp UndiGraph.h TabuCol2.cpp TabuCol2.h)