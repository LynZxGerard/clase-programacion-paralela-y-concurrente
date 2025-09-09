//
// Created by Daniel Orozco on 08/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // parent -> child
    int pipe2[2]; // child -> parent

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child
        // Close unused ends
        close(pipe1[1]); // child reads from pipe1
        close(pipe2[0]); // child writes to pipe2

        // Read message from parent
        char buffer[100];
        int n = read(pipe1[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Hijo recibio: " << buffer << std::endl;

        // Send reply to parent
        const char *msg = "Hola padre, recibi tu mensaje!";
        write(pipe2[1], msg, strlen(msg));

        // Close used ends
        close(pipe1[0]);
        close(pipe2[1]);
    }
    else { // Parent
        // Close unused ends
        close(pipe1[0]); // parent writes to pipe1
        close(pipe2[1]); // parent reads from pipe2

        // Send message to child
        const char *msg = "Hola hijo, ¿me escuchas?";
        write(pipe1[1], msg, strlen(msg));

        // Read reply from child
        char buffer[100];
        int n = read(pipe2[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Padre recibio: " << buffer << std::endl;

        // Close used ends
        close(pipe1[1]);
        close(pipe2[0]);

        wait(NULL);
    }

    return 0;
}

// OUTPUT
// Hijo recibio: Hola hijo, ¿me escuchas?
// Padre recibio: Hola padre, recibi tu mensaje!
