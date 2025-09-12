//
// Created by Daniel Orozco on 08/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Comunicación: padre -> hijo
    int pipe2[2]; // Comunicación: hijo -> padre

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Proceso hijo
        // Cerrar extremos no usados
        close(pipe1[1]); // hijo lee de pipe1
        close(pipe2[0]); // hijo escribe en pipe2

        // Leer mensaje del padre
        char buffer[100];
        int n = read(pipe1[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Hijo recibio: " << buffer << std::endl;

        // Enviar respuesta al padre
        const char *msg = "Hola padre, recibi tu mensaje!";
        write(pipe2[1], msg, strlen(msg));

        // Cerrar extremos usados
        close(pipe1[0]);
        close(pipe2[1]);
    }
    else { // Proceso padre
        // Cerrar extremos no usados
        close(pipe1[0]); // padre escribe en pipe1
        close(pipe2[1]); // padre lee de pipe2

        // Enviar mensaje al hijo
        const char *msg = "Hola hijo, ¿me escuchas?";
        write(pipe1[1], msg, strlen(msg));

        // Leer respuesta del hijo
        char buffer[100];
        int n = read(pipe2[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Padre recibio: " << buffer << std::endl;

        // Cerrar extremos usados
        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL); // Esperar al hijo
    }

    return 0;
}

// OUTPUT
// Hijo recibio: Hola hijo, ¿me escuchas?
// Padre recibio: Hola padre, recibi tu mensaje!
