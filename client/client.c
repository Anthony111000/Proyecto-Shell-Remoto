#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    // Crear socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Dirección inválida o no soportada");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al conectar con el servidor");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor.\n");

    // Enviar comandos y recibir respuestas
    while (1) {
        printf("Ingresa un comando para ejecutar (o 'salir' para terminar): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Eliminar salto de línea del comando
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "salir") == 0) {
            printf("Cerrando conexión...\n");
            break;
        }

        // Enviar comando al servidor
        send(sock_fd, buffer, strlen(buffer), 0);

        // Recibir la respuesta del servidor
        memset(response, 0, BUFFER_SIZE);
        int bytes_received = recv(sock_fd, response, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            response[bytes_received] = '\0';
            printf("Respuesta del servidor:\n%s\n", response);
        }
    }

    close(sock_fd);
    return 0;
}
