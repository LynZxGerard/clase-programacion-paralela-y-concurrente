//
// Created by Daniel Orozco on 25/09/2025.
//

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <wait.h>
#include <cctype>
#include <vector>
#include <string> // Necesario para std::string y std::to_string

#define SHM_KEY 1234
#define SEM_KEY 5678
#define MSG_SIZE 128
#define MAX_MSGS 5

struct SharedData {
    char buffer[MAX_MSGS][MSG_SIZE]; // cola circular
    int in;  // indice siguiente a escribir
    int out; // indice siguiente a leer
};

// Operacion simple de semaforo
void sem_op(int semid, int semnum, int op) {
    struct sembuf sb{};
    sb.sem_num = semnum;
    sb.sem_op = op;
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) { perror("semop"); exit(1); }
}

int main() {
    // ------------------ Entrada de mensajes ------------------
    int total_msgs;
    std::cout << "¿Cuantos mensajes desea ingresar?: ";
    std::cin >> total_msgs;
    std::cin.ignore(); // limpiar buffer de entrada

    std::vector<std::string> mensajes(total_msgs);
    for (int i = 0; i < total_msgs; ++i) {
        std::cout << "Ingrese mensaje " << i + 1 << ": ";
        std::getline(std::cin, mensajes[i]);
    }

    // ------------------ Inicializacion IPC ------------------
    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }

    int semid = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    if (semid < 0) { perror("semget"); exit(1); }

    semctl(semid, 0, SETVAL, 1);     // mutex
    semctl(semid, 1, SETVAL, MAX_MSGS); // espacios libres
    semctl(semid, 2, SETVAL, 0);     // items disponibles

    SharedData* data = (SharedData*) shmat(shmid, NULL, 0);
    if (data == (void*)-1) { perror("shmat"); exit(1); }
    data->in = 0;
    data->out = 0;

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        // ------------------ CONSUMIDOR (Proceso Hijo) ------------------
        std::string debug_consumer_output; // Log de debug del consumidor
        std::string consumer_result_output; // Resultado final del consumidor

        SharedData* data_child = (SharedData*) shmat(shmid, NULL, 0);
        if (data_child == (void*)-1) { perror("shmat hijo"); exit(1); }

        for (int i = 0; i < total_msgs; ++i) {
            sem_op(semid, 2, -1); // esperar item disponible
            sem_op(semid, 0, -1); // lock mutex

            char msg[MSG_SIZE];
            strncpy(msg, data_child->buffer[data_child->out], MSG_SIZE);
            msg[MSG_SIZE-1] = '\0';

            data_child->out = (data_child->out + 1) % MAX_MSGS;

            int count = (data_child->in - data_child->out + MAX_MSGS) % MAX_MSGS;

            // *** ACUMULAR el DEBUG***
            debug_consumer_output += "[DEBUG Consumidor] Elementos en buffer despues de leer: " + std::to_string(count) + "\n";

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 1, 1);  // espacio disponible

            // Convertir a mayusculas
            for (int j = 0; msg[j]; ++j) msg[j] = std::toupper((unsigned char)msg[j]);

            // *** ACUMULAR el RESULTADO ***
            consumer_result_output += "[Consumidor] Recibio: " + std::string(msg) + "\n";
        }

        // *** IMPRIMIMOS TODO EL LOG DEL CONSUMIDOR EN UN BLOQUE AL FINAL ***
        std::cout << "\n================= LOG CONSUMIDOR (PID: " << getpid() << ") =================\n";
        std::cout << debug_consumer_output;
        std::cout << "--------------------------------------------------------\n";
        std::cout << consumer_result_output;
        std::cout << "========================================================\n";

        shmdt(data_child);
        exit(0);
    } else {
        // ------------------ PRODUCTOR (Proceso Padre) ------------------
        std::string debug_producer_output; // Log de debug del productor

        for (int i = 0; i < total_msgs; ++i) {
            sem_op(semid, 1, -1); // esperar espacio libre
            sem_op(semid, 0, -1); // lock mutex

            strncpy(data->buffer[data->in], mensajes[i].c_str(), MSG_SIZE);
            data->buffer[data->in][MSG_SIZE-1] = '\0';

            int before = (data->in - data->out + MAX_MSGS) % MAX_MSGS;
            data->in = (data->in + 1) % MAX_MSGS;
            int after = (data->in - data->out + MAX_MSGS) % MAX_MSGS;

            // *** ACUMULAR el DEBUG ***
            debug_producer_output += "[DEBUG Productor] Antes: " + std::to_string(before) + " | Despues: " + std::to_string(after) + "\n";

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 2, 1);  // signal item disponible
        }

        shmdt(data);
        wait(NULL);

        // *** IMPRIMIR TODO EL LOG DEL PRODUCTOR EN UN BLOQUE ***
        std::cout << "\n================= LOG PRODUCTOR (PID: " << getpid() << ") =================\n";
        std::cout << debug_producer_output;
        std::cout << "========================================================\n";

        // ------------------ Limpieza ------------------
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}