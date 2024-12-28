#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    char serMsg[255];

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr));
    listen(servSockD, 1);

    printf("Esperando conexiones...\n");

    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket == -1) {
        printf("Error al aceptar la conexión.\n");
        return -1;
    }

    printf("Cliente conectado.\n");

    do {
        memset(serMsg, 0, sizeof(serMsg)); // Limpiar buffer
        int bytesReceived = recv(clientSocket, serMsg, sizeof(serMsg), 0);

        if (bytesReceived > 0) {
            printf("Cliente: %s\n", serMsg);

            if (strcmp(serMsg, "leave") == 0) {
                printf("El cliente se ha desconectado.\n");
                break;
            }

            pid_t pid = fork();
            if (pid == 0) { // Proceso hijo
                printf("Ejecutando comando: %s\n", serMsg);
                execlp(serMsg, serMsg, (char *)NULL);
                perror("Error ejecutando el comando");
                exit(EXIT_FAILURE);
            } else if (pid < 0) { // Error en fork
                perror("Error al crear el proceso");
            } else { // Proceso padre
                wait(NULL); // Esperar al proceso hijo
            }

            // Enviar confirmación al cliente
            char respuesta[] = "Comando ejecutado";
            send(clientSocket, respuesta, strlen(respuesta) + 1, 0);
        }
    } while (1);

    close(clientSocket);
    close(servSockD);
    return 0;
}
