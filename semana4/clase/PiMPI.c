//
// Created by Daniel Orozco on 26/09/2025.
//

#include <stdio.h>
#include "mpi.h"
#include <math.h>

// Esto se corre con;
// mpicc PiMPI.c -o PiMPI
// mpirun ./PiMPI

// Por alguna razon el print me funciono mal asi que es probable que antes de correrlo la terminal no haga nada
// y para que empiece a compilar haya que ingresar algun número.

int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643; // Valor "real" de pi para comparar el error
    double mypi, pi, h, sum, x;

    // Inicializa MPI y arranca todos los procesos en paralelo
    MPI_Init(&argc,&argv);

    // Obtiene el número total de procesos que se lanzaron con mpirun -np X
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // Obtiene el identificador único de este proceso (0,1,2,...,numprocs-1)
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    while (1) {
        // El proceso 0 es el encargado de pedir al usuario el número de intervalos "n"
        if (myid == 0) {
            printf("Enter the number of intervals: (0 quits) ");
            scanf("%d",&n);
        }

        // Todos los procesos reciben el valor de "n" desde el proceso 0
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        // Si el usuario escribe 0, todos los procesos terminan
        if (n == 0)
            break;
        else {
            // h es el tamaño de cada subintervalo (ancho de cada "rectángulo" en la integración)
            h   = 1.0 / (double) n;
            sum = 0.0;

            // Cada proceso calcula una parte de la suma total:
            // el proceso 0 arranca en i=1, el 1 en i=2, ..., el último en i=numprocs
            // después cada proceso sigue saltando de numprocs en numprocs
            for (i = myid + 1; i <= n; i += numprocs) {
                x = h * ((double)i - 0.5);             // Punto medio del subintervalo
                sum += (4.0 / (1.0 + x*x));            // Fórmula de la función 4/(1+x^2)
            }

            // Cada proceso obtiene su aproximación parcial de pi
            mypi = h * sum;

            // Se suman todas las aproximaciones parciales en el proceso 0
            MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
                       MPI_COMM_WORLD);

            // Solo el proceso 0 imprime el resultado final
            if (myid == 0)
                printf("pi is approximately %.16f, Error is %.16f\n",
                       pi, fabs(pi - PI25DT));
        }
    }

    // Finaliza la ejecución de todos los procesos MPI
    MPI_Finalize();
    return 0;
}
