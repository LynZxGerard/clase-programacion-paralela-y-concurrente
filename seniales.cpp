//
// Created by peque on 05/09/2025.
//

#include "seniales.h"
#include "cola_mensajes.h"
#include <cstring>
#include <csignal>
#include <iostream>
#include <unistd.h>

// EJEMPLO SEÑALES

//Esta linea sobreescribe la funcion SIGINT del sistema
void manejador(int signum) {
    std::cout << "Senial recibida";

    // Esta linea haria que la terminal tambien se cerrara, que es lo que originalmente hace ctrl c.
    // exit 0;
}

int main() {
    signal(SIGINT, manejador);
    std::cout << "Presione CTRL+C";
    while (true) {
        sleep(1);
    }
    return 0;
}