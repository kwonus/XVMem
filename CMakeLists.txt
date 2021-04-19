cmake_minimum_required(VERSION 3.19)
project(XVMem)

set(CMAKE_CXX_STANDARD 17)

add_library(XVMem XVMem.cpp XVMem_unix.cpp XVMem_windows.cpp)