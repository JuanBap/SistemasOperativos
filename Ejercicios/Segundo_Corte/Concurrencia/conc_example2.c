// concurrency_example2.c
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

int counter = 0; // Variable compartida entre los hilos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger la variable counter

// Función que será ejecutada por cada hilo
void *increment_counter(void *thread_id) {
    pthread_mutex_lock(&mutex); // Bloquear el mutex antes de acceder a la variable compartida
    counter++; // Incrementar la variable compartida
    printf("Hilo %ld: contador = %d\n", (long)thread_id, counter);
    pthread_mutex_unlock(&mutex); // Desbloquear el mutex
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;
    int rc;

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, increment_counter, (void *)t);
        if (rc) { // Verificar si hubo un error al crear el hilo
            printf("Error al crear el hilo %ld; código de retorno: %d\n", t, rc);
            return 1; // Salir del programa con código de error
        }
    }

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Valor final del contador: %d\n", counter);

    pthread_exit(NULL); // Finalizar el hilo principal
}
