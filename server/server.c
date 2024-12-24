#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

int main(int argc, char const* argv[]) {
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    char serMsg[255]; 

    struct sockaddr_in servAddr; 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(9001); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 

    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)); 
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

            // Enviar respuesta
            char respuesta[] = "Mensaje recibido";
            send(clientSocket, respuesta, strlen(respuesta) + 1, 0);
        }
    } while (1);

    close(clientSocket); 
    close(servSockD); 
    return 0;
}
