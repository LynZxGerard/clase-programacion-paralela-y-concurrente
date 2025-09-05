#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

// EJEMPLO PIPE PADRE-HIJO

int main() {
    int fd[2];
    if ( pipe(fd) == -1 ) {
        perror("pipe");
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
    }

    if (pid == 0) { //Hijo
        close(fd[0]); // No leer
        write(fd[1], "hola mundoo", 14);
        close(fd[1]); // Cerrar escritura
    }

    else
        close(fd[1]); //Padre
    read(fd[0], NULL, 0);
    close(fd[0]);
    //wait(NULL);


    // PADRE LEE LOS MENSAJES
    // HIJO ESCRIBE
    //pipe(fd); // Crear canal
    //fork(); // Duplica el proceso (Padre-hijo)
    //close(); // Se asegura de liberar los recursos

    const char *message = &*"Communication successfull";
    write(fd[1], message, strlen(message));
    std::cout << message << std::endl;

    return 0;
}