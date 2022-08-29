cmake_minimum_required(VERSION 3.22)
project(Learn)




set(CMAKE_CXX_STANDARD 17)

include_directories(${Catch2_INCLUDE_DIRS})

add_executable(Learn main.cpp)

target_link_libraries(Learn ${Catch2_LIBS})

