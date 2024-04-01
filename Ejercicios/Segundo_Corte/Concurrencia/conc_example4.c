// concurrency_example4.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

int counter = 0; // Variable compartida entre los hilos
sem_t mutex; // Semáforo para proteger la variable counter

void *increment_counter(void *thread_id) {
    long tid = (long)thread_id;
    sem_wait(&mutex); // Bloquear el semáforo
    counter++; // Incrementar la variable compartida
    printf("Hilo %ld: contador = %d\n", tid, counter);
    sem_post(&mutex); // Desbloquear el semáforo
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;

    // Inicializar el semáforo
    sem_init(&mutex, 0, 1);

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, increment_counter, (void *)t);
    }

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destruir el semáforo
    sem_destroy(&mutex);

    pthread_exit(NULL); // Finalizar el hilo principal
}
