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
    long tipo;
    char texto[100];
};

int main() {
    key_t key = ftok("profgile", 65); // generate unique key
    int msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("msgget");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child: Consumer
        for (int i = 0; i < 7; ++i) {
            message msg;
            // Receive messages of type 1
            if (msgrcv(msg_id, &msg, sizeof(msg.texto), 1, 0) == -1) {
                perror("msgrcv");
                return 1;
            }
            std::cout << "Consumer received: " << msg.texto << std::endl;
        }
        return 0;
    } else { // Parent: Producer
        for (int i = 0; i < 7; ++i) {
            message msg;
            msg.tipo = 1;
            snprintf(msg.texto, sizeof(msg.texto), "Message %d from producer", i+1);
            if (msgsnd(msg_id, &msg, sizeof(msg.texto), 0) == -1) {
                perror("msgsnd");
                return 1;
            }
            std::cout << "Producer sent: " << msg.texto << std::endl;
        }

        wait(NULL); // Wait for child to finish

        // Remove the message queue
        msgctl(msg_id, IPC_RMID, NULL);
    }

    return 0;
}

// OUTPUT ===================================
// Producer sent: Message 1 from producer
// Producer sent: Message 2 from producer
// Producer sent: Message 3 from producer
// Producer sent: Message 4 from producer
// Producer sent: Message 5 from producer
// Producer sent: Message 6 from producer
// Producer sent: Message 7 from producer
// Consumer received: Message 1 from producer
// Consumer received: Message 2 from producer
// Consumer received: Message 3 from producer
// Consumer received: Message 4 from producer
// Consumer received: Message 5 from producer
// Consumer received: Message 6 from producer
// Consumer received: Message 7 from producer