# ServidorClienteSockets

Este proyecto implementa un servidor en lenguaje C utilizando sockets, junto con dos clientes, uno en C y otro en Java, para generar nombres de usuario y contraseñas.

## Descripción

El servidor implementa las siguientes funcionalidades:
1. **Generador de nombres de usuario**: Indicando la longitud del nombre de usuario (debe validar que no sea menor a 5 ni mayor a 15), el servidor generará la cadena correspondiente alternando entre vocales y consonantes.
2. **Generador de contraseñas**: Indicando la longitud de la contraseña (debe validar que sea mayor o igual a 8 y menor a 50), el servidor generará una cadena alfanumérica, incluyendo mayúsculas y minúsculas.

## Estructura del Proyecto

- **servidor.c**: Implementación del servidor en C.
- **cliente.c**: Implementación del cliente en C.
- **ClienteJava.java**: Implementación del segundo cliente en Java.

## Compilación y Ejecución

### Servidor en C

Para compilar y ejecutar el servidor, sigue estos pasos:

```sh
gcc -o servidor servidor.c
./servidor
