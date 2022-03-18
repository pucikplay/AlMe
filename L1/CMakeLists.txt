cmake_minimum_required(VERSION 3.21)
project(AlMe)

set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
include_directories(AlMe ${SDL2_INCLUDE_DIRS})

add_executable(AlMe main.cpp parser.cpp visualizer.cpp roadSolver.cpp random_generator.cpp roadSolver.h parser.h visualizer.h random_generator.h k_random.cpp k_random.h 2_opt.cpp 2_opt.h)
target_link_libraries(AlMe ${SDL2_LIBRARIES})