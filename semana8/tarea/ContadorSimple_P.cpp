//
// Created by Daniel Orozco on 27/10/2025.
//

#include <iostream>
#include <omp.h>
using namespace std;

#define N 100000000 // mismo tamaño que el secuencial

int main() {
    long long contador = 0;

    omp_set_num_threads(2);  // Se puede testear con mas hilos

    double start = omp_get_wtime(); // inicio del cronómetro

    // Paraleliza el for
    // y usa reduction(+:contador) para sumar sin el problema de la condicion de carrera
#pragma omp parallel for reduction(+:contador)
    for (int i = 0; i < N; i++) {
        contador += i;
    }

    double end = omp_get_wtime(); // fin del cronómetro

    cout << "Resultado final: " << contador << endl;
    cout << "Tiempo total (paralelo): " << (end - start) << " s" << endl;

    return 0;
}