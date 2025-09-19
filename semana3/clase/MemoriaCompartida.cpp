//
// Created by Daniel Orozco on 19/09/2025.
//

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <wait.h>

#define SHM_SIZE 1024   // Tamaño de memoria compartida
#define SHM_KEY 1234    // Clave única
#define SEM_KEY 5678    // Clave para semáforos

// Estructura para memoria compartida
struct SharedData {
    char buffer[SHM_SIZE];
    bool ready; // Indicador de que hay datos
};

// Operaciones para semáforos
void sem_op(int semid, int semnum, int op) {
    struct sembuf sb;
    sb.sem_num = semnum;  // Semáforo a usar
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
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Crear semáforos: [0] mutex, [1] espacio disponible, [2] items disponibles
    int semid = semget(SEM_KEY, 3, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        exit(1);
    }

    // Inicializar semáforos
    semctl(semid, 0, SETVAL, 1); // mutex = 1
    semctl(semid, 1, SETVAL, 1); // espacio disponible = 1
    semctl(semid, 2, SETVAL, 0); // items disponibles = 0

    // Fork: padre será productor, hijo será consumidor
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    else if (pid == 0) {
        // ----- PROCESO HIJO: CONSUMIDOR -----
        SharedData* data = (SharedData*) shmat(shmid, NULL, 0);

        for (int i = 0; i < 5; i++) {
            sem_op(semid, 2, -1); // esperar item disponible
            sem_op(semid, 0, -1); // lock mutex

            std::cout << "Consumidor recibió: " << data->buffer << std::endl;

            data->ready = false;

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 1, 1);  // espacio disponible
            sleep(1);
        }

        shmdt(data);
        exit(0);
    }
    else {
        // ----- PROCESO PADRE: PRODUCTOR -----
        SharedData* data = (SharedData*) shmat(shmid, NULL, 0);

        for (int i = 0; i < 5; i++) {
            sem_op(semid, 1, -1); // esperar espacio disponible
            sem_op(semid, 0, -1); // lock mutex

            std::string msg = "Mensaje " + std::to_string(i+1);
            strncpy(data->buffer, msg.c_str(), SHM_SIZE);
            data->ready = true;

            std::cout << "Productor envió: " << msg << std::endl;

            sem_op(semid, 0, 1);  // unlock mutex
            sem_op(semid, 2, 1);  // item disponible
            sleep(1);
        }

        shmdt(data);
        wait(NULL);

        // Eliminar recursos
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}