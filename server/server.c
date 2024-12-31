#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

// Variable global para llevar el conteo de clientes
int clientCounter = 1;

// Estructura para almacenar la info del cliente
typedef struct {
    int socket;
    int id;
} ClientInfo;

// Función para manejar la conexión de un cliente en un hilo
void *handleClient(void *arg) {
    ClientInfo *clientInfo = (ClientInfo *)arg;
    int clientSocket = clientInfo->socket;
    int clientId = clientInfo->id;
    char serMsg[255];

    // Mensaje de cliente conectado
    printf("Cliente %d conectado.\n", clientId);

    do {
        memset(serMsg, 0, sizeof(serMsg)); // Limpiar buffer
        int bytesReceived = recv(clientSocket, serMsg, sizeof(serMsg), 0);

        if (bytesReceived > 0) {
            printf("Cliente %d: %s\n", clientId, serMsg);

            if (strcmp(serMsg, "leave") == 0) {
                printf("El cliente %d se ha desconectado.\n", clientId);
                break;
            }

            printf("Cliente %d ejecutando comando: %s\n", clientId, serMsg);

            // Ejecutar el comando y capturar la salida
            FILE *fp = popen(serMsg, "r");
            if (fp == NULL) {
                perror("Error ejecutando el comando");
                break;
            }

            // Leer la salida del comando
            char output[1024];
            while (fgets(output, sizeof(output), fp) != NULL) {
                send(clientSocket, output, strlen(output), 0);  // Enviar la salida al cliente
            }
            pclose(fp);
        }
    } while (1);

    close(clientSocket); // Cerrar el socket del cliente
    free(clientInfo); // Liberar memoria asignada al socket
    pthread_exit(NULL); // Terminar el hilo
}

int main(int argc, char const *argv[]) {
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD == -1) {
        perror("Error al crear el socket del servidor");
        return -1;
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
        perror("Error al enlazar el socket");
        return -1;
    }

    if (listen(servSockD, 5) == -1) {
        perror("Error al escuchar en el socket");
        return -1;
    }

    printf("Esperando conexiones...\n");

    while (1) {
        // Asigna memoria para la estructura del cliente
        ClientInfo *clientInfo = malloc(sizeof(ClientInfo));
        if (clientInfo == NULL) {
            perror("Error al asignar memoria para el cliente");
            continue;
        }
        
        // Acepta la conexion del cliente
        clientInfo->socket = accept(servSockD, NULL, NULL);
        if (clientInfo->socket == -1) {
            perror("Error al aceptar la conexión");
            free(clientInfo);
            continue;
        }

	// Asigna un ID unico al cliente
	clientInfo->id = clientCounter++;

	// Crea un hilo para manejar al cliente
        pthread_t threadID;
        if (pthread_create(&threadID, NULL, handleClient, clientInfo) != 0) {
            perror("Error al crear el hilo");
	    close(clientInfo->socket);
            free(clientInfo);
            continue;
        }

        pthread_detach(threadID); // Separa el hilo para manejo independiente
    }

    close(servSockD); // Cerrar el socket del servidor
    return 0;
}
