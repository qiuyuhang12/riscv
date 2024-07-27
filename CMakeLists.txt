cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(code VERSION 1.0)
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
#        lfc.hpp
        # 其他源文件
)

# 指定生成的可执行文件
add_executable(code ${SOURCES})