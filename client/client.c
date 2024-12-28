#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

void *senter(void* sockDe) {
    char strData[326];
    char strDataI[326];
    int sockD = *(int *)sockDe;

    do {
        // Pide al usuario un comando para enviar
        printf("Introduce un mensaje: ");
        fgets(strData, sizeof(strData), stdin);
        strData[strcspn(strData, "\n")] = '\0'; // Elimina el salto de línea
        send(sockD, strData, strlen(strData) + 1, 0); // Envia el comando al servidor

        if (strcmp(strData, "leave") == 0) {
            break; // Salir del ciclo si el mensaje es "leave"
        }

        // Recibe la respuesta del servidor (salida del comando ejecutado)
        memset(strDataI, 0, sizeof(strDataI));  // Limpiar buffer
        recv(sockD, strDataI, sizeof(strDataI), 0); // Recibe la salida del servidor
        printf("Salida del comando: %s\n", strDataI);  // Mostrar la salida

    } while (1);
}

int main(int argc, char const* argv[]) {
    pthread_t job;
    int sockD = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9001);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus = connect(sockD, (struct sockaddr*)&servAddr, sizeof(servAddr));

    if (connectStatus == -1) {
        printf("Error al conectar con el servidor...\n");
        return -1;
    }

    printf("Conectado al servidor.\nEscribe 'leave' para salir.\n");
    pthread_create(&job, NULL, senter, &sockD);
    pthread_join(job, NULL);  // Espera a que termine el hilo

    close(sockD); // Cerrar socket
    return 0;
}
