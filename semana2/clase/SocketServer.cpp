//
// Created by Daniel Orozco on 12/09/2025.
//

#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>

int main() {
    int server_fd, client_fd;               // Descriptores del socket POSIX
    struct sockaddr_in address{};            // Estructura de Ip y Puerto
    socklen_t addrlen = sizeof(address);    // Tipo para longitudes de direcciones

    char buffer[1024] ={0};                 // Buffer para recibir datos

    // Crear socket - AF_INET(cualquier direccion IPv4)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::perror("socket");
        return 1;
    }

    // Preparar estructura
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Acepta direcciones de cualquier tipo
    address.sin_port = htons(8080);       // Puerto 8080 seleccionado

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {"error";}

    // Socket en modo escucha
    if (listen(server_fd, 3) < 0) {"error";}

    std::cout << "Server esperando en puerto 8080" << std::endl;

    // accept() bloquea hasta que llega el cliente
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) {"error"; close(server_fd);}
    std::cout << "Cliente: " << client_fd << " conectado."<< std::endl;

    // Leer al cliente
    ssize_t n = read(client_fd, buffer, sizeof(buffer)-1);
    if (n < 0) {"error";}
    else{std::cout<<"Cliente envia: "<<std::string(buffer, (size_t)n) <<std::endl;}

    // Enviar respuesta al cliente
    const char* msg = "Hola cliente desde servidor";
    ssize_t s = send(client_fd, msg, strlen(msg), 0);
    if (s < 0) {"error";}
    close(client_fd);
    close(server_fd);
    return 0;
}
