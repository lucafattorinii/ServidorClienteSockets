import java.io.*;
import java.net.*;
import java.util.Scanner;

public class ClienteJava {
    private static final String SERVER_ADDRESS = "127.0.0.1";
    private static final int SERVER_PORT = 8080;

    public static void main(String[] args) {
        try (Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             Scanner scanner = new Scanner(System.in)) {

            while (true) {
                System.out.println("Menu:");
                System.out.println("1. Generar nombre de usuario");
                System.out.println("2. Generar contraseña");
                System.out.println("3. Salir");
                System.out.print("Seleccione una opción: ");
                int opcion = scanner.nextInt();
                scanner.nextLine(); // Consumir el salto de línea

                String mensaje = "";
                switch (opcion) {
                    case 1:
                        System.out.print("Ingrese la longitud del nombre de usuario: ");
                        int longitudNombre = scanner.nextInt();
                        scanner.nextLine(); // Consumir el salto de línea
                        if (longitudNombre < 5 || longitudNombre > 15) {
                            System.out.println("Error: La longitud del nombre de usuario debe ser entre 5 y 15.");
                            continue;
                        }
                        mensaje = "nombre " + longitudNombre;
                        break;
                    case 2:
                        System.out.print("Ingrese la longitud de la contraseña: ");
                        int longitudContrasena = scanner.nextInt();
                        scanner.nextLine(); // Consumir el salto de línea
                        if (longitudContrasena < 8 || longitudContrasena > 50) {
                            System.out.println("Error: La longitud de la contraseña debe ser entre 8 y 50.");
                            continue;
                        }
                        mensaje = "contrasena " + longitudContrasena;
                        break;
                    case 3:
                        return;
                    default:
                        System.out.println("Opción no válida");
                        continue;
                }

                // Enviar la solicitud al servidor
                out.println(mensaje);

                // Leer la respuesta del servidor
                String respuesta = in.readLine();
                if (respuesta != null) {
                    System.out.println("Respuesta del servidor: " + respuesta);
                } else {
                    System.out.println("Error al recibir la respuesta del servidor");
                }
            }
        } catch (IOException e) {
            System.err.println("Error en la comunicación con el servidor: " + e.getMessage());
        }
    }
}
