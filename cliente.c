// cliente.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER 1024

void menu() {
    printf("Menu:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar contraseña\n");
    printf("3. Salir\n");
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER] = {0};
    char respuesta[MAX_BUFFER] = {0};

    // Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error al crear el socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir la dirección IP a binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Dirección no válida o no soportada\n");
        return -1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Conexión fallida\n");
        return -1;
    }

    int opcion;
    while (1) {
        menu();
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese la longitud del nombre de usuario: ");
                int longitudNombre;
                scanf("%d", &longitudNombre);
                snprintf(buffer, MAX_BUFFER, "nombre %d", longitudNombre);
                break;
            case 2:
                printf("Ingrese la longitud de la contraseña: ");
                int longitudContrasena;
                scanf("%d", &longitudContrasena);
                snprintf(buffer, MAX_BUFFER, "contrasena %d", longitudContrasena);
                break;
            case 3:
                close(sock);
                exit(0);
            default:
                printf("Opción no válida\n");
                continue;
        }

        // Enviar la solicitud al servidor
        send(sock, buffer, strlen(buffer), 0);

        // Leer la respuesta del servidor
        read(sock, respuesta, MAX_BUFFER);
        printf("Respuesta del servidor: %s\n", respuesta);
    }

    return 0;
}
