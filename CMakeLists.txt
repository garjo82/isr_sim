cmake_minimum_required(VERSION 3.20)

project(isr_sim
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(isr_sim
    src/main.cpp
)
