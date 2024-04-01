// fork_example2.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // Llamada a fork para crear un nuevo proceso
    pid_t pid = fork();
    if (pid == 0) {
        // El código dentro de este bloque se ejecuta en el proceso hijo
        printf("Soy el proceso hijo (PID: %d)\n", getpid());
    } else if (pid > 0) {
        // El código dentro de este bloque se ejecuta en el proceso padre
        printf("Soy el proceso padre (PID: %d) y mi otro hijo es %d\n", getpid(), pid);
    }
    return 0;
}
