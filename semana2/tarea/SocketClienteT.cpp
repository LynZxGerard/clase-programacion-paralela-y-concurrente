//
// Created by Daniel Orozco on 15/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr{};
    char buffer[1024] = {0};

    // Crear socket TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Configuración de dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convertir IP a formato binario y asignarla
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::cout << "Conectado al servidor. Escriba mensajes (\"exit\", \"terminar\" o \"0\" para salir)." << std::endl;

    // Bucle principal del chat
    while (true) {
        std::string input;
        std::cout << "Tú: ";
        std::getline(std::cin, input);  // leer mensaje del usuario

        // Enviar mensaje al servidor
        send(sock, input.c_str(), input.size(), 0);

        // Si es un mensaje de salida, terminamos el bucle
        if (input == "exit" || input == "terminar" || input == "0") {
            std::cout << "Cerrando conexión..." << std::endl;
            break;
        }

        // Recibir respuesta del servidor
        memset(buffer, 0, sizeof(buffer)); // limpiar buffer
        ssize_t n = read(sock, buffer, sizeof(buffer)-1);
        if (n <= 0) {
            std::cout << "Servidor desconectado." << std::endl;
            break;
        }
        buffer[n] = '\0';
        std::cout << buffer << std::endl;
    }

    // Cerrar socket
    close(sock);
    return 0;
}
