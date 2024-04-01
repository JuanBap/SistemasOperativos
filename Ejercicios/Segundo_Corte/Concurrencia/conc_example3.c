// concurrency_example3.c
#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE]; // Buffer compartido entre el productor y el consumidor
int count = 0; // Contador de elementos en el buffer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el buffer y el contador
pthread_cond_t condp = PTHREAD_COND_INITIALIZER; // Variable de condición para el productor
pthread_cond_t condc = PTHREAD_COND_INITIALIZER; // Variable de condición para el consumidor

// Función del productor
void *producer(void *param) {
    int produced = 0;
    while (1) {
        pthread_mutex_lock(&mutex); // Bloquear el mutex antes de acceder al buffer
        while (count == BUFFER_SIZE) { // Esperar si el buffer está lleno
            pthread_cond_wait(&condp, &mutex);
        }
        buffer[count] = produced; // Producir un elemento y agregarlo al buffer
        printf("Productor produce: %d\n", produced);
        count++;
        pthread_cond_signal(&condc); // Despertar al consumidor si está esperando
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex
        produced++;
    }
}

// Función del consumidor
void *consumer(void *param) {
    int consumed;
    while (1) {
        pthread_mutex_lock(&mutex); // Bloquear el mutex antes de acceder al buffer
        while (count == 0) { // Esperar si el buffer está vacío
            pthread_cond_wait(&condc, &mutex);
        }
        consumed = buffer[count - 1]; // Consumir un elemento del buffer
        printf("Consumidor consume: %d\n", consumed);
        count--;
        pthread_cond_signal(&condp); // Despertar al productor si está esperando
        pthread_mutex_unlock(&mutex); // Desbloquear el mutex
    }
}

int main() {
    pthread_t tid_producer, tid_consumer;

    // Crear los hilos del productor y el consumidor
    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);

    pthread_exit(NULL); // Finalizar el hilo principal
}
