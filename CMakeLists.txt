cmake_minimum_required(VERSION 4.0)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled pipe.cpp
        cola_mensajes.cpp
        cola_mensajes.h
        seniales.cpp
        seniales.h)
