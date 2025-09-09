//
// Created by Daniel Orozco on 08/09/2025.
//

#include <csignal>
#include <iostream>
#include <unistd.h>

// Signal handler
void manejador(int signum) {
    if (signum == SIGUSR1) {
        std::cout << "Señal SIGUSR1 recibida: Acción 1 ejecutada!" << std::endl;
    } else if (signum == SIGUSR2) {
        std::cout << "Señal SIGUSR2 recibida: Acción 2 ejecutada!" << std::endl;
    }
}

int main() {
    // Register signal handlers
    signal(SIGUSR1, manejador);
    signal(SIGUSR2, manejador);

    std::cout << "PID del proceso: " << getpid() << std::endl;
    std::cout << "Envíe SIGUSR1 o SIGUSR2 desde otra terminal usando 'kill -USR1 PID' o 'kill -USR2 PID'" << std::endl;

    // Infinite loop to keep the program running
    while (true) {
        sleep(1);
    }

    return 0;
}


// PID del proceso: 1991
// Envíe SIGUSR1 o SIGUSR2 desde otra terminal usando 'kill -USR1 PID' o 'kill -USR2 PID'
