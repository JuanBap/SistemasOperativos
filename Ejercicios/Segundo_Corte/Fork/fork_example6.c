// fork_example6.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Proceso hijo
        printf("Soy el proceso hijo (PID: %d)\n", getpid());

        // Reemplazar el programa del proceso hijo con otro programa usando exec
        execl("/bin/ls", "ls", "-l", NULL);

        // Si exec falla, el siguiente código nunca se ejecutará a menos que haya un error
        perror("exec");
        return 1;
    } else { // Proceso padre
        printf("Soy el proceso padre (PID: %d) y mi hijo es %d\n", getpid(), pid);

        wait(NULL); // Esperar a que el proceso hijo termine
    }

    return 0;
}
