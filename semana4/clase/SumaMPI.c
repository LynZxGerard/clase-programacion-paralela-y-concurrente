//
// Created by Daniel Orozco on 26/09/2025.
//


#include "mpi.h"
#include <stdio.h>

// Esto se corre con;
// mpicc SumaMPI.c -o SumaMPI
// mpirun -np # ./SumaMPI.c
// Donde # -> Numero de procesos

// Si uso # -> 4 ----> Suma = 12

void main(int argc, char** argv){

    int my_rank, s=0, x=0;

    // Inicializa el entorno MPI
    MPI_Init(&argc, &argv);

    // Cada proceso obtiene su identificador único (rank)
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Solo el proceso 0 define el valor inicial de x
    // En este caso, elegimos 2 para que todos los procesos lo usen
    // Ese 2 representa los multiplos que se acumulan segun los n procesos del usuario
    if(my_rank == 0) x = 2;

    // Se "difunde" el valor de x a todos los procesos
    // Esto funciona como los broadcast que aprendimos en redes
    // Ahora todos los procesos tienen x = 2
    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso multiplica x por su rank
    // Esto crea valores distintos en cada proceso dependiendo de su número
    x *= my_rank;

    // Se suman los valores de x de todos los procesos y se envía el resultado al proceso 0
    // MPI_Reduce se encarga de recolectar y combinar todos los resultados
    MPI_Reduce(&x, &s, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Solo el proceso 0 imprime el resultado final de la suma
    if(my_rank == 0)
        printf("The sum is %d.\n", s);

    // Finaliza el entorno MPI, liberando recursos
    MPI_Finalize();
}