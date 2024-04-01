// exercise6.c
#include <stdio.h>
#include <pthread.h>

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
void *thread_function(void *thread_id) {
    long tid = (long)thread_id;

    lock_acquire(&lock); // Adquirir el bloqueo
    printf("Hilo %ld: Entrando a la sección crítica\n", tid);
    // Sección crítica
    printf("Hilo %ld: Saliendo de la sección crítica\n", tid);
    lock_release(&lock); // Liberar el bloqueo

    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[2]; // Arreglo para almacenar los identificadores de los hilos
    int rc;

    // Crear dos hilos
    for (long t = 0; t < 2; t++) {
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) { // Verificar si hubo un error al crear el hilo
            printf("Error al crear el hilo %ld; código de retorno: %d\n", t, rc);
            return 1; // Salir del programa con código de error
        }
    }

    // Esperar a que ambos hilos terminen
    for (long t = 0; t < 2; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_exit(NULL); // Finalizar el hilo principal
}
