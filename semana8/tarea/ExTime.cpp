//
// Created by Daniel Orozco on 27/10/2025.
//

#include <iostream>
#include <omp.h>
using namespace std;

#define N 1000000

int main()
{
    double *A = new double[N], *B = new double[N], *C = new double[N];
    for (int i = 0; i < N; i++)
    {
        A[i] = i;
        B[i] = i;
    }

    double start = omp_get_wtime();
    for (int i = 0; i < N; i++)
        C[i] = A[i] * B[i];
    double end = omp_get_wtime();
    cout << "Tiempo total: " << (end - start) << " s" << endl;

    delete[] A;
    delete[] B;
    delete[] C;
}