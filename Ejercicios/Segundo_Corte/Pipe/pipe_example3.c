// pipe_example3.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define PING "ping"
#define PONG "pong"

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
        for (int i = 0; i < 5; i++) {
            read(pipefd[0], buffer, sizeof(buffer)); // Leer desde la tubería
            printf("Proceso hijo (PID: %d) recibe: %s\n", getpid(), buffer);
            write(pipefd[1], PONG, sizeof(PONG)); // Enviar respuesta al padre
        }
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso hijo
    } else { // Proceso padre
        close(pipefd[0]); // Cerrar el extremo de lectura del proceso padre
        for (int i = 0; i < 5; i++) {
            write(pipefd[1], PING, sizeof(PING)); // Enviar mensaje al hijo
            read(pipefd[0], buffer, sizeof(buffer)); // Leer respuesta del hijo
            printf("Proceso padre (PID: %d) recibe: %s\n", getpid(), buffer);
        }
        close(pipefd[1]); // Cerrar el extremo de escritura del proceso padre
    }

    return 0;
}
