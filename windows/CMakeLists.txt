cmake_minimum_required(VERSION 3.21)
project(Stretch_Timer C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Stretch_Timer
        main.c
        timer.h)
