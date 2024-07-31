cmake_minimum_required(VERSION 3.20)

# 设置项目名称和版本
project(code)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
# 添加源文件
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
        #        lfc.hpp
        # 其他源文件
)

# 指定生成的可执行文件
add_executable(code ${SOURCES})
