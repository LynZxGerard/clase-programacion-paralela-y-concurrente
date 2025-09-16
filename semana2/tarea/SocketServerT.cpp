//
// Created by Daniel Orozco on 15/09/2025.
//

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address{};
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Crear socket TCP
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    // Preparar dirección del servidor (IP + puerto)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // aceptar conexiones de cualquier IP
    address.sin_port = htons(8080);

    // Asignar socket al puerto
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind");
        return 1;
    }

    // Poner socket en modo escucha
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Servidor esperando en puerto 8080..." << std::endl;

    // Aceptar conexión entrante
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    std::cout << "Cliente conectado." << std::endl;

    // Bucle principal del chat
    while (true) {
        memset(buffer, 0, sizeof(buffer));  // limpiar buffer
        ssize_t n = read(client_fd, buffer, sizeof(buffer)-1); // leer mensaje
        if (n <= 0) {
            std::cout << "Error o cliente desconectado." << std::endl;
            break;
        }

        buffer[n] = '\0'; // asegurar fin de cadena
        std::string msg(buffer);

        std::cout << "Cliente: " << msg << std::endl;

        // Si recibe un mensaje de salida, cerrar conexión
        if (msg == "exit" || msg == "terminar" || msg == "0") {
            std::cout << "Cliente solicitó cerrar la conexión." << std::endl;
            break;
        }

        // Enviar respuesta al cliente
        std::string respuesta = "Servidor recibió: " + msg;
        send(client_fd, respuesta.c_str(), respuesta.size(), 0);
    }

    // Cerrar sockets
    close(client_fd);
    close(server_fd);
    return 0;
}