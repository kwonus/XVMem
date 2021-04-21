cmake_minimum_required(VERSION 3.19)
project(XVMem)

set(CMAKE_CXX_STANDARD 17)

include_directories(BEFORE "./")
add_library(XVMem XVMem.cpp XVMem_unix.cpp XVMem_windows.cpp
                  XVMem.h   XVMem_unix.h   XVMem_windows.h
                  XVMem_platform.h)

set(CMAKE_PREFIX_PATH "C:/CMake/public")
install(FILES XVMem.h XVMem_unix.h XVMem_windows.h XVMem_platform.h DESTINATION include)
install(TARGETS XVMem DESTINATION lib)
install(FILES XVMemConfig.cmake DESTINATION "${CMAKE_PREFIX_PATH}/lib/XVMem")