cmake_minimum_required(VERSION 4.0)
project(lab1t)
set(CMAKE_CXX_STANDARD 20)
add_executable(lab1 main.cpp
        smc/AppClass.cpp
        smc/AppClass.h
        smc/Analyz.h
        smc/Sm_sm.cpp
        smc/Sm_sm.h
        regex/Regex.cpp
        regex/Regex.h
)
target_include_directories(lab1 PRIVATE ${CMAKE_SOURCE_DIR}/smc)