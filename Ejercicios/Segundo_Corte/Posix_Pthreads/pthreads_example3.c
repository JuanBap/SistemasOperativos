// exercise3.c
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

int flag = 0; // Variable compartida entre los hilos
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger la variable flag
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // Variable de condición para la sincronización

// Función que será ejecutada por cada hilo
void *thread_function(void *thread_id) {
    long tid = (long)thread_id;
    pthread_mutex_lock(&mutex); // Bloquear el mutex antes de acceder a la variable compartida

    // Esperar hasta que la condición sea verdadera
    while (!flag) {
        pthread_cond_wait(&cond, &mutex);
    }

    printf("Hilo %ld: La condición es verdadera, continuando...\n", tid);
    pthread_mutex_unlock(&mutex); // Desbloquear el mutex
    pthread_exit(NULL); // Finalizar el hilo
}

int main() {
    pthread_t threads[NUM_THREADS]; // Arreglo para almacenar los identificadores de los hilos
    long t;
    int rc;

    // Crear múltiples hilos
    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
        if (rc) { // Verificar si hubo un error al crear el hilo
            printf("Error al crear el hilo %ld; código de retorno: %d\n", t, rc);
            return 1; // Salir del programa con código de error
        }
    }

    // Simular alguna operación que establezca la condición a verdadera
    sleep(2);
    printf("Estableciendo la condición a verdadera...\n");
    pthread_mutex_lock(&mutex);
    flag = 1; // Establecer la condición a verdadera
    pthread_cond_broadcast(&cond); // Despertar a todos los hilos que esperan en la condición
    pthread_mutex_unlock(&mutex);

    // Esperar a que todos los hilos terminen
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_exit(NULL); // Finalizar el hilo principal
}
