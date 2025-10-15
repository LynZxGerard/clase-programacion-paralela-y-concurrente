//
// Created by Daniel Orozco on 15/10/2025.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> listo(false);

void productor() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    listo = true;
    std::cout << "Productor esta listo\n";
}

void consumidor() {
    while (!listo) {
        // Necesidad de este fragmento?
        std::cout << "Consumidor no esta listo" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::cout << "Consumidor recibió señal\n";
}

int main() {
    std::thread t1(productor), t2(consumidor);
    t1.join();
    t2.join();
}
