//
// Created by Daniel Orozco on 15/10/2025.
//

#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2;

void tarea1() {
    std::lock_guard<std::mutex> lock1(m1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Hilo 1 completado\n";
}

void tarea2() {
    std::lock_guard<std::mutex> lock2(m2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Hilo 2 completado\n";
}

int main() {
    std::thread t1(tarea1);
    std::thread t2(tarea2);

    t1.join();
    t2.join();
}