// exercise5.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

sem_t barrier_sem; // Semáforo para sincronizar los hilos
int threads_arrived = 0; // Contador de hilos que han llegado a la barrera

// Función que será ejecutada por cada hilo
void *thread_function(void *thread_id) {
    long tid = (long)thread_id;
    printf("Hilo %ld esperando en la barrera...\n", tid);

    sem_wait(&barrier_sem); // Decrementar el semáforo

    threads_arrived++; // Aumentar el contador de hilos llegados
    if (threads_arrived == NUM_THREADS) {
        // Si todos los hilos han llegado, liberar todos los hilos
        for (int i = 0; i < NUM_THREADS - 1; i++) {
            sem_post(&barrier_sem);
        }
    }

    printf("Hilo %ld continuando después de la barrera\n", tid);
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;
    int rc;

    // Inicializar el semáforo
    sem_init(&barrier_sem, 0, 0);

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) { // Verificar si hubo un error al crear el hilo
            printf("Error al crear el hilo %ld; código de retorno: %d\n", t, rc);
            return 1; // Salir del programa con código de error
        }
    }

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destruir el semáforo
    sem_destroy(&barrier_sem);

    pthread_exit(NULL); // Finalizar el hilo principal
}
