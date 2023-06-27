cmake_minimum_required(VERSION 3.16)
project(RISC-V-Simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fmax-errors=10 -O2 -DONLINE_JUDGE -lm -std=c++17")
add_compile_options(-fmax-errors=10 -O2 -DONLINE_JUDGE -lm -std=c++17)

include_directories(src)

add_executable(code src/main.cpp src/Bus.cpp src/Instructions.cpp
        src/Memory.cpp src/ReorderBuffer.cpp src/ReservationStation.cpp src/Register.cpp
        src/Predictor.cpp src/Unit.cpp src/Utils.cpp)