cmake_minimum_required(VERSION 3.20)

project(code)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
set(SOURCES
        main.cpp
        memory.hpp
        register.hpp
        decode.hpp
        reorder_buffer.hpp
        reserve_station.hpp
        common_data_bus.hpp
        interpreter.hpp
        utility.hpp
        predictor.hpp
        simulator.hpp
        algorithm_logic_unit.hpp
        load_store_buffer.hpp
        algorithm_logic_unit.cpp
        common_data_bus.cpp
        decode.cpp
        load_store_buffer.cpp
        memory.cpp
        predictor.cpp
        register.cpp
        reorder_buffer.cpp
        reserve_station.cpp
        simulator.cpp
        utility.cpp
)

add_executable(code ${SOURCES})
