#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Hijo
        close(fd[0]); // cerrar lectura
        const char *msg = "Hola mundo desde el hijo";
        write(fd[1], msg, strlen(msg));
        close(fd[1]); // cerrar escritura
    }
    else { // Padre
        close(fd[1]); // cerrar escritura
        char buffer[100];
        int n = read(fd[0], buffer, sizeof(buffer)-1);
        if (n > 0) buffer[n] = '\0';
        std::cout << "Padre recibió: " << buffer << std::endl;
        close(fd[0]);
        wait(NULL);
    }

    return 0;
}

// OUTPOUT
// Padre recibió: Hola mundo desde el hijo