//
// Created by Daniel on 05/09/2025.
//

#include "cola_mensajes.h"
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// EJEMPLO COLA DE MENSAJES

struct message {
    long tipo;
    char texto[100];
};

int main() {
    key_t key = ftok("profgile", 65); // Generar una clave unica para el msg
    int msg_int=msgget(key, 066 | IPC_CREAT);

    if(msg_int==-1) {
        perror("ERROR");
        return 0;
    }

    // Enviar mensaje
    message msg;
    msg.tipo = 1;
    strcpy(msg.texto, "Hola mundo");
    msgsnd(msg_int, &msg, sizeof(msg), 0);
    std::cout << "enviado" << std::endl;

    // Recibir mensaje
    msgrcv(msg_int, &msg, sizeof(msg.texto), 1, 0); // Double check a esto.
    std::cout << "recibido" << std::endl;

    // Eliminar de cola
    msgctl(msg_int, IPC_RMID, NULL);

}