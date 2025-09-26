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

#define SHM_KEY 1234    // Clave unica memoria compartida
#define SEM_KEY 5678    // Clave para semaforos
#define MSG_SIZE 128    // Tamanio maximo de cada mensaje
#define MAX_MSGS 5      // Capacidad de la cola

// Estructura de cola circular
struct SharedData {
    char buffer[MAX_MSGS][MSG_SIZE]; // Cola de mensajes
    int in;   // indice de escritura (siguiente posicion libre)
    int in;   // indice de escritura (siguiente posicion libre)
    int out;  // indice de lectura (siguiente elemento a leer)
    int out;  // indice de lectura (siguiente elemento a leer)
};

// Operacion simple de semaforo
void sem_op(int semid, int semnum, int op) {
    struct sembuf sb{};
    sb.sem_num = semnum;
    sb.sem_op = op;       // -1 wait, +1 signal
    sb.sem_flg = 0;
    if (semop(semid, &sb, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

int main() {
    // Crear segmento de memoria compartida
    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid < 0) { perror("shmget"); exit(1); }

    // Crear semaforos: [0] mutex, [1] espacios libres, [2] items disponibles
    int semid = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    if (semid < 0) { perror("semget"); exit(1); }

    // Inicializar semaforos
    semctl(semid, 0, SETVAL, 1);        // mutex = 1
    semctl(semid, 1, SETVAL, MAX_MSGS); // espacios libres = tamanio cola
    semctl(semid, 2, SETVAL, 0);        // items disponibles = 0

    // Mapear memoria compartida y inicializar indices (lo hace el padre)
    // Mapear memoria compartida y inicializar indices (lo hace el padre)
    SharedData* data = (SharedData*) shmat(shmid, NULL, 0);
    if (data == (void*)-1) { perror("shmat"); exit(1); }
    data->in = 0;
    data->out = 0;

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        // ---------------- HIJO: CONSUMIDOR ----------------
        SharedData* data_child = (SharedData*) shmat(shmid, NULL, 0);
        if (data_child == (void*)-1) { perror("shmat child"); exit(1); }

        while (true) {
            sem_op(semid, 2, -1); // esperar item disponible
            sem_op(semid, 0, -1); // lock mutex

            char msg[MSG_SIZE];
            strncpy(msg, data_child->buffer[data_child->out], MSG_SIZE);
            msg[MSG_SIZE-1] = '\0';

            int before = (data_child->in - data_child->out + MAX_MSGS) % MAX_MSGS;
            data_child->out = (data_child->out + 1) % MAX_MSGS;
            int after = (data_child->in - data_child->out + MAX_MSGS) % MAX_MSGS;

            // DEBUG Consumidor organizado
            std::cout << "\n[DEBUG - Elementos en buffer] Antes de leer: " << before
                    << " | Despues de leer: " << after << "\n" << std::endl;

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 1, 1);  // espacio disponible

            for (int i = 0; msg[i]; ++i) msg[i] = std::toupper((unsigned char)msg[i]);

            if (strcmp(msg, "EXIT") == 0 || strcmp(msg, "0") == 0) {
                std::cout << "\n[Consumidor] Finalizando.\n" << std::endl;
                break;
            }

            std::cout << "[Consumidor] Recibio: " << msg << "\n" << std::endl;

        }

        shmdt(data_child);
        exit(0);
    }
    else {
        // ---------------- PADRE: PRODUCTOR ----------------
        std::string input;
        while (true) {
            std::cout << "\n[Productor] Ingrese un mensaje (\"exit\" o \"0\" para terminar): ";
            std::getline(std::cin, input);

            sem_op(semid, 1, -1); // esperar espacio disponible
            sem_op(semid, 0, -1); // lock mutex

            strncpy(data->buffer[data->in], input.c_str(), MSG_SIZE);
            data->buffer[data->in][MSG_SIZE-1] = '\0';

            // DEBUG
            int before = (data->in - data->out + MAX_MSGS) % MAX_MSGS;
            int after = ((data->in + 1) - data->out + MAX_MSGS) % MAX_MSGS;
            std::cout << "[DEBUG - Elementos en buffer] Antes de insertar: " << before
                      << " | Despues de insertar: " << after << "\n" << std::endl;

            data->in = (data->in + 1) % MAX_MSGS;

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 2, 1);  // signal item disponible

            if (input == "exit" || input == "0") break;
        }

        shmdt(data);
        wait(NULL);

        // ------------------ Limpieza ------------------
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}