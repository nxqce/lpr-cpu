set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR "i686")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32" CACHE STRING "c++ flags")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32" CACHE STRING "c flags")

set(OpenCV_FOUND 1)
cmake_minimum_required(VERSION 2.8)
project( LPR )
find_package( OpenCV REQUIRED )
add_executable( lpr main.cpp )
target_link_libraries( lpr ${OpenCV_LIBS} )

