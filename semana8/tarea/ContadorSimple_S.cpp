//
// Created by Daniel Orozco on 27/10/2025.
//

#include <iostream>
#include <omp.h>
using namespace std;

#define N 100000000 // tamaño grande para ver la diferencia

int main() {
    long long contador = 0;

    double start = omp_get_wtime(); // inicio del cronómetro

    // Este funciona de forma SECUENCIAL
    for (int i = 0; i < N; i++) {
        contador += i;
    }

    double end = omp_get_wtime(); // fin del cronómetro

    cout << "Resultado final: " << contador << endl;
    cout << "Tiempo total (secuencial): " << (end - start) << " s" << endl;

    return 0;
}