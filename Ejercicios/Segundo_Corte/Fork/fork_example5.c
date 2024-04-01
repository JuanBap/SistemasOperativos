// fork_example5.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[20];

    // Crear la tubería
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Crear un nuevo proceso
    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Proceso hijo
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso hijo

        // Leer desde la tubería
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Proceso hijo (PID: %d) recibe mensaje del padre: %s\n", getpid(), buffer);

        close(pipefd[0]); // Cerrar el extremo de lectura del proceso hijo
    } else { // Proceso padre
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso padre

        // Escribir en la tubería
        char mensaje[] = "Hola desde el padre";
        write(pipefd[1], mensaje, sizeof(mensaje));

        close(pipefd[1]); // Cerrar el extremo de escritura del proceso padre

        wait(NULL); // Esperar a que el proceso hijo termine
    }

    return 0;
}
