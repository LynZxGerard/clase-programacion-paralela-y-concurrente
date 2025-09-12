//
// Created by Daniel Orozco on 08/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // Padre → Hijo 1
    int pipe2[2]; // Padre → Hijo 2
    int resp1[2]; // Hijo 1 → Padre
    int resp2[2]; // Hijo 2 → Padre

    // Crear todos los pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(resp1) == -1 || pipe(resp2) == -1) {
        perror("pipe");
        return 1;
    }

    // Crear primer hijo
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) { // Hijo 1
        // Cerrar extremos no usados
        close(pipe1[1]); // lectura desde padre
        close(resp1[0]); // escritura a padre
        close(pipe2[0]); close(pipe2[1]); // cerrar pipe hijo2
        close(resp2[0]); close(resp2[1]); // cerrar respuesta hijo2

        // Leer mensaje del padre
        char buffer[100];
        int n = read(pipe1[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Hijo 1 recibio: " << buffer << std::endl;

        // Responder al padre
        const char *respuesta = "Hola padre, Hijo 1 recibio tu mensaje!";
        write(resp1[1], respuesta, strlen(respuesta));

        // Cerrar pipes usados
        close(pipe1[0]);
        close(resp1[1]);
        return 0;
    }

    // Crear segundo hijo
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) { // Hijo 2
        // Cerrar extremos no usados
        close(pipe2[1]); // lectura desde padre
        close(resp2[0]); // escritura a padre
        close(pipe1[0]); close(pipe1[1]); // cerrar pipe hijo1
        close(resp1[0]); close(resp1[1]); // cerrar respuesta hijo1

        // Leer mensaje del padre
        char buffer[100];
        int n = read(pipe2[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Hijo 2 recibio: " << buffer << std::endl;

        // Responder al padre
        const char *respuesta = "Hola padre, Hijo 2 recibio tu mensaje!";
        write(resp2[1], respuesta, strlen(respuesta));

        // Cerrar pipes usados
        close(pipe2[0]);
        close(resp2[1]);
        return 0;
    }

    // Padre: cerrar extremos no usados
    close(pipe1[0]); close(pipe2[0]); // escritura
    close(resp1[1]); close(resp2[1]); // lectura

    // Enviar mensajes a los hijos
    const char *msg1 = "Hola hijo 1!";
    const char *msg2 = "Hola hijo 2!";
    write(pipe1[1], msg1, strlen(msg1));
    write(pipe2[1], msg2, strlen(msg2));

    // Leer respuestas de los hijos
    char buffer1[100], buffer2[100];
    int n1 = read(resp1[0], buffer1, sizeof(buffer1)-1);
    if (n1 > 0) buffer1[n1] = '\0';
    std::cout << "Padre recibio: " << buffer1 << std::endl;

    int n2 = read(resp2[0], buffer2, sizeof(buffer2)-1);
    if (n2 > 0) buffer2[n2] = '\0';
    std::cout << "Padre recibio: " << buffer2 << std::endl;

    // Cerrar todos los pipes
    close(pipe1[1]); close(pipe2[1]);
    close(resp1[0]); close(resp2[0]);

    // Esperar a los hijos
    wait(NULL);
    wait(NULL);

    return 0;
}

/*
OUTPUT =================================================
Hijo 1 recibio: Hola hijo 1!
Hijo 2 recibio: Hola hijo 2!
Padre recibio: Hola padre, Hijo 1 recibio tu mensaje!
Padre recibio: Hola padre, Hijo 2 recibio tu mensaje!
*/
