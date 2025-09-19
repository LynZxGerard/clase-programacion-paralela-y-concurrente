cmake_minimum_required(VERSION 3.28.3)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled semana1/clase/pipe.cpp
        semana1/clase/cola_mensajes.cpp
        semana1/clase/cola_mensajes.h
        semana1/clase/seniales.cpp
        semana1/clase/seniales.h
        semana1/tarea/PipeBidireccional.cpp
        semana1/tarea/ColaMensajesPC.cpp
        semana1/tarea/SenialesSIGUSR.cpp
        semana1/tarea/PipeDosHijos.cpp
        semana2/clase/SocketServer.cpp
        semana2/clase/SocketCliente.cpp
        semana2/tarea/SocketClienteT.cpp
        semana2/tarea/SocketServerT.cpp
        semana3/clase/MemoriaCompartida.cpp
)
