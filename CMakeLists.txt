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
        semana3/tarea/MemoriaCompartidaT.cpp
        semana3/tarea/MemoriaCompartidaAlterno.cpp
        semana4/clase/PiMPI.c
        semana4/clase/SumaMPI.c
        semana5/clase/Hilos1.cpp
        semana5/clase/Hilos2.cpp
        semana5/clase/Hilos3.cpp
        semana5/tarea/Hilos1.cpp
        semana5/tarea/Hilos2.cpp
        semana5/tarea/Hilos3.cpp
        semana8/tarea/ExTime.cpp
        semana8/tarea/ContadorSimple_S.cpp
        semana8/tarea/ContadorSimple_P.cpp
        semana8/tarea/MultiplicacionVectorial_S.cpp
        semana8/tarea/MultiplicacionVectorial_P.cpp
        semana8/tarea/MultiplicacionMatricial_S.cpp
        semana8/tarea/MultiplicacionMatricial_P.cpp
        proyecto-final/proyecto_defectuoso.cpp
        proyecto-final/proyecto_corregido.cpp
)
