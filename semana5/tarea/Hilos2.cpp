//
// Created by Daniel Orozco on 15/10/2025.
//

#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2;

// uso de std::lock y std::lock_guard con std::adopt_lock ---
// Esto garantiza que ambos mutex se bloqueen sin riesgo de deadlock.

void tarea1() {
    // Bloqueamos los dos mutex simultáneamente para evitar interbloqueo
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);  // adopt_lock indica que ya está bloqueado
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);

    std::cout << "Hilo 1 completado\n";
}

void tarea2() {
    // Bloquear ambos mutex en el mismo orden, igual que en tarea1
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);

    std::cout << "Hilo 2 completado\n";
}

int main() {
    std::thread t1(tarea1);
    std::thread t2(tarea2);

    t1.join();
    t2.join();
}