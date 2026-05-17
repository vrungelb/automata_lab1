cmake_minimum_required(VERSION 3.20)
project(lab1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# SMC
set(SMC_JAR        /home/timur/Documents/smc_7_6_0/bin/Smc.jar)
set(SMC_RUNTIME    /home/timur/Documents/smc_7_6_0/lib/C++)
set(SMC_INPUT      ${CMAKE_SOURCE_DIR}/smc/Sm.sm)
set(SMC_OUT_CPP    ${CMAKE_CURRENT_BINARY_DIR}/Sm_sm.cpp)
set(SMC_OUT_H      ${CMAKE_CURRENT_BINARY_DIR}/Sm_sm.h)
add_custom_command(
        OUTPUT  ${SMC_OUT_CPP} ${SMC_OUT_H}
        COMMAND java -jar ${SMC_JAR} -c++ -d ${CMAKE_CURRENT_BINARY_DIR} ${SMC_INPUT}
        DEPENDS ${SMC_INPUT}
        COMMENT "Generating SMC state machine from Sm.sm"
)
# FLEX
find_package(FLEX REQUIRED)
FLEX_TARGET(MyLexer
        ${CMAKE_SOURCE_DIR}/flex/flex.l
        ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc
)
add_executable(lab1
        main.cpp
        smc/AppClass.cpp
        smc/AppClass.h
        smc/Analyz.h
        ${SMC_OUT_CPP}
        ${SMC_OUT_H}
        regex/Regex.cpp
        regex/Regex.h
        flex/Flex.cpp
        flex/Flex.h
        strgen/strgen.cpp
        strgen/strgen.h
        bench/Bench.cpp
        bench/Bench.h
        ${FLEX_MyLexer_OUTPUTS}
)
target_include_directories(lab1 PRIVATE
        ${CMAKE_SOURCE_DIR}/smc
        ${CMAKE_SOURCE_DIR}/flex
        ${CMAKE_SOURCE_DIR}/strgen
        ${CMAKE_SOURCE_DIR}/bench
        ${CMAKE_CURRENT_BINARY_DIR}
        ${SMC_RUNTIME}
        ${FLEX_INCLUDE_DIRS}
)

