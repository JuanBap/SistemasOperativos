// exercise1.c
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

// Función que será ejecutada por cada hilo
void *print_hello(void *thread_id) {
    long tid = (long)thread_id;
    printf("Hola desde el hilo %ld\n", tid);
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    int rc;
    long t;

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creando hilo %ld\n", t);
        // Crear un nuevo hilo y ejecutar la función print_hello
        rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);
        if (rc) { // Verificar si hubo un error al crear el hilo
            printf("Error al crear el hilo %ld; código de retorno: %d\n", t, rc);
            return 1; // Salir del programa con código de error
        }
    }

    pthread_exit(NULL); // Finalizar el hilo principal
}
