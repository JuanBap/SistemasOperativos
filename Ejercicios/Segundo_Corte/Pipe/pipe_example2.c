// pipe_example2.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipefd[2];
    char buffer[20];

    // Crear la tubería
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Proceso hijo
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso hijo
        read(pipefd[0], buffer, sizeof(buffer)); // Leer desde la tubería
        printf("Proceso hijo (PID: %d) recibe mensaje del padre: %s\n", getpid(), buffer);
        // Enviar una respuesta al padre
        char respuesta[] = "Mensaje recibido en el hijo";
        write(pipefd[1], respuesta, sizeof(respuesta));
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso hijo
    } else { // Proceso padre
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso padre
        char mensaje[] = "Hola desde el padre";
        write(pipefd[1], mensaje, sizeof(mensaje)); // Escribir en la tubería
        // Leer la respuesta del hijo
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Proceso padre (PID: %d) recibe respuesta del hijo: %s\n", getpid(), buffer);
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso padre
    }

    return 0;
}
