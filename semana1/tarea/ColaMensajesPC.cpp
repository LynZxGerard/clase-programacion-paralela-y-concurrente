//
// Created by Daniel Orozco on 08/09/2025.
//

#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct message {
    long tipo;          // Tipo de mensaje
    char texto[100];    // Contenido del mensaje
};

int main() {
    // Genera una clave única para identificar la cola de mensajes
    key_t key = ftok("profgile", 65);
    int msg_id = msgget(key, 0666 | IPC_CREAT);

    if (msg_id == -1) {
        perror("Error al crear/obtener la cola de mensajes");
        return 1;
    }

    pid_t pid = fork();  // Crear un proceso hijo

    if (pid < 0) {
        perror("Error en fork");
        return 1;
    }

    if (pid == 0) {
        // =============================
        // PROCESO HIJO → CONSUMIDOR
        // =============================
        for (int i = 0; i < 7; ++i) {
            message msg;

            // Recibir mensajes del tipo 1
            if (msgrcv(msg_id, &msg, sizeof(msg.texto), 1, 0) == -1) {
                perror("Error al recibir mensaje");
                return 1;
            }

            std::cout << "Consumidor recibio: " << msg.texto << std::endl;
        }
        return 0;

    } else {
        // =============================
        // PROCESO PADRE → PRODUCTOR
        // =============================
        for (int i = 0; i < 7; ++i) {
            message msg;
            msg.tipo = 1; // Todos los mensajes son del tipo 1

            // Crear el texto del mensaje
            snprintf(msg.texto, sizeof(msg.texto), "Mensaje %d del productor", i+1);

            // Enviar mensaje
            if (msgsnd(msg_id, &msg, sizeof(msg.texto), 0) == -1) {
                perror("Error al enviar mensaje");
                return 1;
            }

            std::cout << "Productor envio: " << msg.texto << std::endl;
        }

        wait(NULL); // Espera a que el hijo termine

        // Eliminar la cola de mensajes
        msgctl(msg_id, IPC_RMID, NULL);
    }

    return 0;
}

// OUTPUT ===================================
// Productor envio: Mensaje 1 del productor
// Productor envio: Mensaje 2 del productor
// Productor envio: Mensaje 3 del productor
// Productor envio: Mensaje 4 del productor
// Productor envio: Mensaje 5 del productor
// Productor envio: Mensaje 6 del productor
// Productor envio: Mensaje 7 del productor
// Consumidor recibio: Mensaje 1 del productor
// Consumidor recibio: Mensaje 2 del productor
// Consumidor recibio: Mensaje 3 del productor
// Consumidor recibio: Mensaje 4 del productor
// Consumidor recibio: Mensaje 5 del productor
// Consumidor recibio: Mensaje 6 del productor
// Consumidor recibio: Mensaje 7 del productor