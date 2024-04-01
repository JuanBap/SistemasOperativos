// pipe_example4.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];
    pid_t pid;

    // Crear la tubería
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Proceso hijo
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso hijo
        // Redirigir la salida estándar al extremo de escritura de la tubería
        dup2(pipefd[1], STDOUT_FILENO);
        // Ejecutar el comando ls para listar el directorio actual
        execlp("ls", "ls", NULL);
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso hijo
    } else { // Proceso padre
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso padre
        char buffer[4096];
        // Leer desde el extremo de lectura de la tubería
        ssize_t nbytes = read(pipefd[0], buffer, sizeof(buffer));
        // Imprimir la salida del hijo
        write(STDOUT_FILENO, buffer, nbytes);
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso padre
    }

    return 0;
}
