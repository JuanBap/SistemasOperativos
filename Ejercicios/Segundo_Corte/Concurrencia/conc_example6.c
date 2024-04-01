// concurrency_example6.c
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 2

int counter = 0; // Variable compartida entre los hilos
int lock = 0; // Bloqueo (0 significa desbloqueado, 1 significa bloqueado)

// Función para adquirir el bloqueo
void lock_acquire(int *lock) {
    while (__sync_lock_test_and_set(lock, 1)) {} // Intentar establecer el bloqueo de manera atómica
}

// Función para liberar el bloqueo
void lock_release(int *lock) {
    __sync_lock_release(lock); // Liberar el bloqueo de manera atómica
}

// Función que será ejecutada por cada hilo
void *increment_counter(void *thread_id) {
    long tid = (long)thread_id;
    lock_acquire(&lock); // Adquirir el bloqueo
    counter++; // Incrementar la variable compartida
    printf("Hilo %ld: contador = %d\n", tid, counter);
    lock_release(&lock); // Liberar el bloqueo
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_create(&threads[t], NULL, increment_counter, (void *)t);
    }

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_exit(NULL); // Finalizar el hilo principal
}
