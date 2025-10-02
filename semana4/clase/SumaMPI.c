//
// Created by Daniel Orozco on 26/09/2025.
//


#include "mpi.h"
#include <stdio.h>

// Esto se corre con;
// mpicc SumaMPI.c -o SumaMPI
// mpirun -np# ./SumaMPI.c
// Donde # -> Numero de procesos

// Si uso # -> 4 ----> Suma = 12

void main(int argc, char** argv){

    int my_rank, s=0, x=0;

    MPI_Init(&argc, &argv);
    //MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank==0) x=2;

    MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

    x *= my_rank;

    MPI_Reduce(&x, &s, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(my_rank==0) printf("The sum is %d.\n", s);

    MPI_Finalize();

}