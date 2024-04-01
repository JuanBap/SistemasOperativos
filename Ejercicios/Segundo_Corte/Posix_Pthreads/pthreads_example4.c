// exercise4.c
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

int shared_resource = 0; // Recurso compartido entre los hilos
sem_t sem; // Semáforo para la sincronización

// Función que será ejecutada por cada hilo
void *thread_function(void *thread_id) {
    long tid = (long)thread_id;

    sem_wait(&sem); // Decrementar el semáforo
    shared_resource++; // Modificar el recurso compartido
    printf("Hilo %ld: recurso compartido = %d\n", tid, shared_resource);
    sem_post(&sem); // Incrementar el semáforo

    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;
    int rc;

    // Inicializar el semáforo
    sem_init(&sem, 0, 1);

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
    sem_destroy(&sem);

    pthread_exit(NULL); // Finalizar el hilo principal
}
