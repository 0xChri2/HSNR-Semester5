cmake_minimum_required(VERSION 3.25.2)
project(praktikum2)

find_package( OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_executable(praktikum2 main.cpp)
target_link_libraries(praktikum2 ${OpenCV_LIBS})
set(CMAKE_CXX_STANDARD 17)