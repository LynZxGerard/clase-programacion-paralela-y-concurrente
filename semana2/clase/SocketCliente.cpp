//
// Created by Daniel Orozco on 12/09/2025.
//

#include<iostream>
#include<unistd.h>
#include<arpa/inet.h>
#include<cstring>

int main() {
    int sock=0;

    // El profe puso esto en el pizarron
    // struct sockaddr_in serv_addr();
    // Pero le borre los parentesis y el resto funciono.

    struct sockaddr_in serv_addr;         // Estructura de Ip y Puerto
    char buffer[1024] ={0};                 // Buffer para recibir datos

    // Crear socket - AF_INET(cualquier direccion IPv4)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::perror("socket");
        return 1;
    }

    // Llamar la estructura del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);  // Puerto 8080 seleccionado

    if (inet_pton(AF_INET, "127.0.0.1",&serv_addr.sin_addr)<=0){"error";}

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){"error";}

    // Enviar mensaje al cliente
    const char* msg = "Hola servidor desde cliente";
    if (send(sock, msg, strlen(msg), 0) < 0){"error";}

    //Leer respuesta
    ssize_t n = read(sock, buffer, sizeof(buffer)-1);
    if (n<0){"error";}
    else {
        std::cout<<"Servidor responde"<<std::string(buffer, (size_t)n)<<std::endl;
        close(sock);
        return 0;
    }
}

