//
// Created by Daniel Orozco on 27/10/2025.
//

#include <iostream>
#include <omp.h>
using namespace std;

#define N 500  // tamaño de la matriz (500x500)

int main() {
    double **A = new double*[N];
    double **B = new double*[N];
    double **C = new double*[N];

    for (int i = 0; i < N; i++) {
        A[i] = new double[N];
        B[i] = new double[N];
        C[i] = new double[N];
    }

    // Inicialización
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }

    double start = omp_get_wtime();

    // Esto funciona de forma SECUENCIAL
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double end = omp_get_wtime();

    cout << "C[0][0] = " << C[0][0] << " | C[N-1][N-1] = " << C[N-1][N-1] << endl;
    cout << "Tiempo total (secuencial): " << (end - start) << " s" << endl;

    for (int i = 0; i < N; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
