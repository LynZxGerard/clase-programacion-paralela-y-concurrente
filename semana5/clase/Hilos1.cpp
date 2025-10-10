//
// Created by Daniel Orozco on 03/10/2025.
//

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> contador(0);

void incrementar() {
    for (int i = 0; i < 100000; i++) {
        contador++;
    }
    std::cout << "Funcion incrementar" << std::endl;
}

int main() {
    std::vector<std::thread> hilos;

    for (int i = 0; i < 4; i++) {
        hilos.emplace_back(incrementar);
    }

    for (auto &h : hilos) h.join();

    std::cout << "Resultado esperado: 400000\n";
    std::cout << "Resultado real: " << contador << std::endl;
    return 0;
}

//OUTPUT
// Resultado esperado: 400000
// Resultado real: 143175 ~~~~~~~~~~
