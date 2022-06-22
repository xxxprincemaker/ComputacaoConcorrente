cmake_minimum_required(VERSION 3.22)
project(ComputacaoConcorrente)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Lab01)

add_executable(ComputacaoConcorrente
        "Lab01/Lab01 - Atividade 5.c")
