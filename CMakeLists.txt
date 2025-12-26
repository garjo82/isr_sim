cmake_minimum_required(VERSION 3.20)

project(isr_sim
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sim
    src/sim/Simulation.cpp
    src/sim/World.cpp
)
target_include_directories(sim PUBLIC src)

add_library(render
    src/render/Renderer.cpp
)
target_include_directories(render PUBLIC src)

add_library(sensors
    src/sensors/Sensor.cpp
)
target_include_directories(sensors PUBLIC src)

add_library(io
    src/io/Logger.cpp
)
target_include_directories(io PUBLIC src)

add_library(app
    src/app/App.cpp
)
target_include_directories(app PUBLIC src)
target_link_libraries(app PRIVATE sim)

add_executable(isr_sim
    src/main.cpp
)
target_include_directories(isr_sim PRIVATE src)
target_link_libraries(isr_sim PRIVATE app sim render sensors io)
