cmake_minimum_required(VERSION 3.5.1)
project(forward_iterator)

#Brind header files in project
include_directories(include)

# Manually setting source files
# set(SOURCES src/forward_iterator.cc)

file(GLOB SOURCES "src/*.cc")

# Set C++11 as standard
# set(CMAKE_CXX_STANDARD 11) # C++11...
# set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
# set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

add_executable(main ${SOURCES})

# C++11 Standard can be set like this also
set_target_properties(main PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)