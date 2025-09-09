//
// Created by Daniel Orozco on 08/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

int main() {
    int pipe1[2]; // parent → child1
    int pipe2[2]; // parent → child2

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) { // Child 1
        close(pipe1[1]); // Close write end
        char buffer[100];
        int n = read(pipe1[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Child 1 received: " << buffer << std::endl;
        close(pipe1[0]);
        return 0;
    }

    // Parent continues to create second child
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) { // Child 2
        close(pipe2[1]); // Close write end
        char buffer[100];
        int n = read(pipe2[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Child 2 received: " << buffer << std::endl;
        close(pipe2[0]);
        return 0;
    }

    // Parent sends different messages
    close(pipe1[0]); // Close read ends
    close(pipe2[0]);

    const char *msg1 = "Hola hijo 1!";
    const char *msg2 = "Hola hijo 2!";

    write(pipe1[1], msg1, strlen(msg1));
    write(pipe2[1], msg2, strlen(msg2));

    close(pipe1[1]);
    close(pipe2[1]);

    // Wait for both children
    wait(NULL);
    wait(NULL);

    return 0;
}

// OUTPUT
// Child 1 received: Hola hijo 1!
// Child 2 received: Hola hijo 2!