cmake_minimum_required(VERSION 3.26)
project(Project_name)

find_package( OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_executable(Project_name main.cpp)
target_link_libraries(Project_name ${OpenCV_LIBS})
set(CMAKE_CXX_STANDARD 17)