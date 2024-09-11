import socket

def main():
    server_ip = '127.0.0.1'
    server_port = 8080

    try:
        flag = True
        while flag:
            # Crear un socket
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # Conectar al servidor
            client_socket.connect((server_ip, server_port))

            # Leer y mostrar el menu del servidor
            server_response1 = client_socket.recv(1024).decode('utf-8').strip()
            print(server_response1)

            # Leer la opción del usuario
            opcion = int(input(""))

            # Enviar la opción al servidor
            client_socket.sendall(opcion.to_bytes(4, byteorder='big'))

            # Leer la siguiente respuesta del servidor
            server_response2 = client_socket.recv(1024).decode('utf-8').strip()
            print(server_response2)


            if server_response2 == "Adios":
                flag = False
            elif server_response2 == "Opcion incorrecta":
                print("Inténtelo nuevamente\n")
            else:
                # Leer el largo del usuario o contraseña
                largo = int(input())

                # Enviar el largo al servidor
                client_socket.sendall(largo.to_bytes(4, byteorder='big'))

                # Leer y mostrar el resultado del servidor
                resultado = client_socket.recv(1024).decode('utf-8').strip()
                print(resultado)

            client_socket.close()

        client_socket.close()
    except (socket.error, ValueError) as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()