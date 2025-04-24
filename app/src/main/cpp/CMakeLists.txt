cmake_minimum_required(VERSION 3.22)
project("matrixcalc")

add_library(matrixcalc SHARED native-lib.cpp)
find_library(log-lib log)
target_link_libraries(matrixcalc ${log-lib})
