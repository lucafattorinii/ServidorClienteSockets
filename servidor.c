#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER 1024

// Función para generar un nombre de usuario alternando entre vocales y consonantes
void generarNombreUsuario(int longitud, char *resultado) {
    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int usarVocal = rand() % 2; // 0: empieza con consonante, 1: empieza con vocal

    for (int i = 0; i < longitud; i++) {
        if (usarVocal) {
            resultado[i] = vocales[rand() % strlen(vocales)];
        } else {
            resultado[i] = consonantes[rand() % strlen(consonantes)];
        }
        usarVocal = !usarVocal; // Alternar
    }
    resultado[longitud] = '\0';
}

// Función para generar una contraseña alfanumérica
void generarContrasena(int longitud, char *resultado) {
    char caracteres[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    for (int i = 0; i < longitud; i++) {
        resultado[i] = caracteres[rand() % strlen(caracteres)];
    }
    resultado[longitud] = '\0';
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    int opt = 1;

    // Inicializar la semilla del generador de números aleatorios
    srand(time(0));

    // Crear el socket del servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar el socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error al configurar el socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar el socket a la dirección y puerto especificados
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error al asociar el socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar las conexiones entrantes
    if (listen(server_fd, 3) < 0) {
        perror("Error al escuchar");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor en ejecución y esperando conexiones...\n");

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Error al aceptar la conexión");
            continue; // Continuar esperando nuevas conexiones
        }

        // Leer el mensaje del cliente
        int bytes_read = read(new_socket, buffer, MAX_BUFFER - 1);
        if (bytes_read < 0) {
            perror("Error al leer del socket");
            close(new_socket);
            continue;
        }
        buffer[bytes_read] = '\0'; // Asegurar que el buffer esté correctamente terminado en null

        // Parsear la solicitud
        char respuesta[MAX_BUFFER] = {0};
        if (strncmp(buffer, "nombre", 6) == 0) {
            int longitud = atoi(buffer + 7);
            if (longitud < 5 || longitud > 15) {
                strcpy(respuesta, "Error: La longitud del nombre de usuario debe ser entre 5 y 15.");
            } else {
                generarNombreUsuario(longitud, respuesta);
            }
        } else if (strncmp(buffer, "contrasena", 10) == 0) {
            int longitud = atoi(buffer + 11);
            if (longitud < 8 || longitud > 50) {
                strcpy(respuesta, "Error: La longitud de la contraseña debe ser entre 8 y 50.");
            } else {
                generarContrasena(longitud, respuesta);
            }
        } else {
            strcpy(respuesta, "Error: Comando no reconocido.");
        }

        // Enviar la respuesta al cliente
        if (send(new_socket, respuesta, strlen(respuesta), 0) < 0) {
            perror("Error al enviar la respuesta");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}
