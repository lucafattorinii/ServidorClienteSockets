// ClienteJava.java
import java.io.*;
import java.net.*;
import java.util.Scanner;

public class ClienteJava {
    private static final String SERVER_ADDRESS = "127.0.0.1";
    private static final int SERVER_PORT = 8080;

    public static void main(String[] args) {
        try (Socket socket = new Socket(SERVER_ADDRESS, SERVER_PORT)) {
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            Scanner scanner = new Scanner(System.in);

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
                        mensaje = "nombre " + longitudNombre;
                        break;
                    case 2:
                        System.out.print("Ingrese la longitud de la contraseña: ");
                        int longitudContrasena = scanner.nextInt();
                        scanner.nextLine(); // Consumir el salto de línea
                        mensaje = "contrasena " + longitudContrasena;
                        break;
                    case 3:
                        socket.close();
                        System.exit(0);
                    default:
                        System.out.println("Opción no válida");
                        continue;
                }

                // Enviar la solicitud al servidor
                out.println(mensaje);

                // Leer la respuesta del servidor
                String respuesta = in.readLine();
                System.out.println("Respuesta del servidor: " + respuesta);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
