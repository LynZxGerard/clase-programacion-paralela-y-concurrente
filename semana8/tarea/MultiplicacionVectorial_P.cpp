//
// Created by Daniel Orozco on 27/10/2025.
//

#include <iostream>
#include <omp.h>
using namespace std;

#define N 1000000

int main() {
    double *A = new double[N], *B = new double[N], *C = new double[N];

    // Inicializar vectores
    for (int i = 0; i < N; i++) {
        A[i] = i * 0.5;
        B[i] = i * 0.25;
    }

    omp_set_num_threads(4); // Esto controla la cantidad de hilos

    double start = omp_get_wtime(); // inicio del cronómetro

    // Este funciona de forma PARALELA
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        C[i] = A[i] * B[i];
    }

    double end = omp_get_wtime(); // fin del cronómetro

    cout << "C[0] = " << C[0] << " | C[N-1] = " << C[N - 1] << endl;
    cout << "Tiempo total (paralelo): " << (end - start) << " s" << endl;

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
