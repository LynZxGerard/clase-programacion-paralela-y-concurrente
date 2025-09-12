//
// Created by Daniel Orozco on 08/09/2025.
//

#include <csignal>
#include <iostream>
#include <unistd.h>

// Manejador de señales personalizado
void manejador(int signum) {
    if (signum == SIGUSR1) {
        std::cout << "Señal SIGUSR1 recibida: Acción 1 ejecutada!" << std::endl;
    } else if (signum == SIGUSR2) {
        std::cout << "Señal SIGUSR2 recibida: Acción 2 ejecutada!" << std::endl;
    }
}

int main() {
    // Registrar los manejadores para SIGUSR1 y SIGUSR2
    signal(SIGUSR1, manejador);
    signal(SIGUSR2, manejador);

    // Mostrar PID para poder enviar señales desde otra terminal
    std::cout << "PID del proceso: " << getpid() << std::endl;
    std::cout << "Envíe SIGUSR1 o SIGUSR2 desde otra terminal con:" << std::endl;
    std::cout << "kill -USR1 PID  ó  kill -USR2 PID" << std::endl;

    // Mantener el proceso en ejecución
    while (true) {
        sleep(1);
    }

    return 0;
}

/*
Ejemplo de salida:
PID del proceso: 1991
Envíe SIGUSR1 o SIGUSR2 desde otra terminal con:
kill -USR1 1991
kill -USR2 1991
*/


// PID del proceso: 1991
// Envíe SIGUSR1 o SIGUSR2 desde otra terminal usando 'kill -USR1 PID' o 'kill -USR2 PID'
