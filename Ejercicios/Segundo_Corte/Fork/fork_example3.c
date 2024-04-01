// fork_example3.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // Ejecutar el bucle dos veces
    for (int i = 0; i < 2; i++) {
        // Llamada a fork para crear un nuevo proceso en cada iteración
        pid_t pid = fork();
        if (pid == 0) {
            // El código dentro de este bloque se ejecuta en el proceso hijo
            printf("Soy el proceso hijo en el bucle (PID: %d)\n", getpid());
            break; // Importante para evitar la creación de más procesos hijos en cada iteración
        } else if (pid > 0) {
            // El código dentro de este bloque se ejecuta en el proceso padre
            printf("Soy el proceso padre en el bucle (PID: %d) y mi hijo es %d\n", getpid(), pid);
        }
    }
    return 0;
}
