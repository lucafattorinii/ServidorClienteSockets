#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int obtenerBooleanoAleatorio() {
    return rand() % 2;
}

char obtenerCaracterAleatorio(int tipo) {
    char digitos[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char *conjunto;
    int tamano;

    if (tipo == 1) {
        conjunto = digitos;
        tamano = sizeof(digitos) / sizeof(digitos[0]);
    } else {
        conjunto = letras;
        tamano = sizeof(letras) / sizeof(letras[0]);
    }
    int indiceAleatorio = rand() % tamano;
    return conjunto[indiceAleatorio];
}

char* generarNombreUsuario(int longitud) {
    if (longitud < 5 || longitud > 15) {
        return "Longitud no valida, debe estar entre 5 y 15 caracteres";
    }
    char *usuario = (char *)malloc((longitud + 1) * sizeof(char));
    char consonantes[] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};
    char vocales[] = {'a', 'e', 'i', 'o', 'u'};
    int alternar = obtenerBooleanoAleatorio();
    int indiceVocal = 0;
    usuario[0] = '\0';

    for (int i = 0; i < longitud; i++) {
        if (i % 2 == 0) {
            usuario[i] = alternar ? consonantes[i % (sizeof(consonantes)/sizeof(consonantes[0]))] : vocales[indiceVocal];
            if (alternar == 0) {
                indiceVocal++;
                if (indiceVocal >= sizeof(vocales) / sizeof(vocales[0])) {
                    indiceVocal = 0;
                }
            }
        } else {
            usuario[i] = alternar ? vocales[indiceVocal] : consonantes[i % (sizeof(consonantes)/sizeof(consonantes[0]))];
            if (alternar == 1) {
                indiceVocal++;
                if (indiceVocal >= sizeof(vocales) / sizeof(vocales[0])) {
                    indiceVocal = 0;
                }
            }
        }
    }
    usuario[longitud] = '\0';  // Terminar cadena
    return usuario;
}

char* generarClave(int longitud) {
    if (longitud < 8 || longitud > 50) {
        return "Longitud incorrecta, debe estar entre 8 y 50 caracteres";
    }
    char *clave = (char *)malloc((longitud + 1) * sizeof(char));
    clave[0] = '\0';

    for (int i = 0; i < longitud; i++) {
        int esLetra = obtenerBooleanoAleatorio();
        if (esLetra) {
            char letra = obtenerCaracterAleatorio(0);
            clave[i] = obtenerBooleanoAleatorio() ? letra : toupper((unsigned char)letra);
        } else {
            clave[i] = obtenerCaracterAleatorio(1);
        }
    }
    clave[longitud] = '\0';  // Terminar cadena
    return clave;
}

void iniciarSockets() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("No se pudo iniciar Winsock. Codigo de error: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

void finalizarSockets() {
    WSACleanup();
}

int main() {
    srand(time(NULL));
    iniciarSockets();

    int socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor == INVALID_SOCKET) {
        printf("Error al crear el socket. Codigo de error: %d\n", WSAGetLastError());
        finalizarSockets();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = INADDR_ANY;
    direccionServidor.sin_port = htons(8080);

    if (bind(socketServidor, (struct sockaddr*)&direccionServidor, sizeof(direccionServidor)) == SOCKET_ERROR) {
        printf("Error al asociar el socket. Codigo de error: %d\n", WSAGetLastError());
        finalizarSockets();
        exit(EXIT_FAILURE);
    }

    if (listen(socketServidor, 5) == SOCKET_ERROR) {
        printf("Error al empezar a escuchar. Codigo de error: %d\n", WSAGetLastError());
        finalizarSockets();
        exit(EXIT_FAILURE);
    }

    printf("Servidor en funcionamiento en el puerto 8080...\n");

    int socketCliente;
    struct sockaddr_in direccionCliente;
    int tamanoCliente = sizeof(direccionCliente);
    int salir = 0;

    do {
        socketCliente = accept(socketServidor, (struct sockaddr*)&direccionCliente, &tamanoCliente);
        if (socketCliente == INVALID_SOCKET) {
            printf("Error al aceptar la conexion. Codigo de error: %d\n", WSAGetLastError());
            finalizarSockets();
            exit(EXIT_FAILURE);
        }
        char *menu = "Selecciona una opcion:\n 1. Crear nombre de usuario.\n 2. Crear clave.\n 0. Salir\n";
        send(socketCliente, menu, strlen(menu), 0);

        int opcion;
        recv(socketCliente, &opcion, sizeof(opcion), 0);
        opcion = ntohl(opcion);

        if (opcion == 0) {
            char *mensaje = "Hasta luego";
            send(socketCliente, mensaje, strlen(mensaje), 0);
        } else if (opcion > 2) {
            char *mensaje = "Opcion no valida";
            send(socketCliente, mensaje, strlen(mensaje), 0);
        } else {
            char *mensaje = "Indica la longitud deseada:";
            send(socketCliente, mensaje, strlen(mensaje), 0);

            int longitud;
            recv(socketCliente, &longitud, sizeof(longitud), 0);
            longitud = ntohl(longitud);

            char *resultado;
            if (opcion == 1) {
                resultado = generarNombreUsuario(longitud);
            } else if (opcion == 2) {
                resultado = generarClave(longitud);
            }

            send(socketCliente, resultado, strlen(resultado), 0);
            free(resultado);
        }
        closesocket(socketCliente);
    } while (!salir);

    closesocket(socketServidor);
    finalizarSockets();
    return 0;
}
