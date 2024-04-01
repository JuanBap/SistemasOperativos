// fork_example4.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // Llamada a fork para crear el primer proceso en la cadena
    pid_t pid = fork();
    if (pid == 0) {
        // El código dentro de este bloque se ejecuta en el primer proceso hijo
        printf("Soy el proceso hijo inicial (PID: %d)\n", getpid());
        // Crear dos procesos hijos más en una cadena
        for (int i = 0; i < 2; i++) {
            pid_t pid_child = fork();
            if (pid_child == 0) {
                // El código dentro de este bloque se ejecuta en los procesos hijos de la cadena
                printf("Soy el proceso hijo en la cadena (PID: %d)\n", getpid());
                break; // Importante para evitar que los procesos hijos creen más hijos
            } else if (pid_child > 0) {
                // El código dentro de este bloque se ejecuta en el proceso padre de la cadena
                printf("Soy el proceso padre en la cadena (PID: %d) y mi hijo es %d\n", getpid(), pid_child);
            }
        }
    } else if (pid > 0) {
        // El código dentro de este bloque se ejecuta en el proceso padre inicial
        printf("Soy el proceso padre del árbol (PID: %d) y mi hijo inicial es %d\n", getpid(), pid);
    }
    return 0;
}
