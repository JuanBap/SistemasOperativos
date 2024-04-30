#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define TEMPERATURA_FLAG "TEMP"
#define PH_FLAG "PH"

int BUFFER_SIZE;
sem_t empty_temp, full_temp, empty_ph, full_ph;
pthread_mutex_t mutex_temp, mutex_ph;
char **buffer_temp;
char **buffer_ph;
int in_temp = 0, out_temp = 0, in_ph = 0, out_ph = 0;
char *file_temp = NULL, *file_ph = NULL;

void ini_buffers() {
    buffer_temp = (char **)malloc(BUFFER_SIZE * sizeof(char *));
    buffer_ph = (char **)malloc(BUFFER_SIZE * sizeof(char *));
    if (!buffer_temp || !buffer_ph) {
        perror("Error al asignar memoria para los buffers");
        exit(1);
    }
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_temp[i] = (char *)malloc(128 * sizeof(char));
        buffer_ph[i] = (char *)malloc(128 * sizeof(char));
        if (!buffer_temp[i] || !buffer_ph[i]) {
            perror("Error al asignar memoria para los buffers");
            exit(1);
        }
    }
}

void free_buffers() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        free(buffer_temp[i]);
        free(buffer_ph[i]);
    }
    free(buffer_temp);
    free(buffer_ph);
}

void *recolector(void *param) {
    char *pipe_name = (char *)param;
    int fd = open(pipe_name, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el pipe en el recolector");
        exit(1);
    }

    char read_buffer[128];
    while (read(fd, read_buffer, sizeof(read_buffer)) > 0) {
        if (read_buffer[strlen(read_buffer) - 1] == '\n') {
            read_buffer[strlen(read_buffer) - 1] = '\0'; // Eliminar el salto de línea
        }

        char *token = strtok(read_buffer, ":");
        int sensor_type = atoi(token);
        token = strtok(NULL, ":");
        char *data = token;

        if (sensor_type == 1 || sensor_type == 2) { // Temperatura o pH
            sem_t *empty, *full;
            pthread_mutex_t *mutex;
            char **buffer;
            int *in, *out;
            if (sensor_type == 1) {
                empty = &empty_temp;
                full = &full_temp;
                mutex = &mutex_temp;
                buffer = buffer_temp;
                in = &in_temp;
                out = &out_temp;
            } else {
                empty = &empty_ph;
                full = &full_ph;
                mutex = &mutex_ph;
                buffer = buffer_ph;
                in = &in_ph;
                out = &out_ph;
            }

            sem_wait(empty);
            pthread_mutex_lock(mutex);

            // Marcar el dato con el flag correspondiente
            sprintf(buffer[*in], "%s:%s", (sensor_type == 1) ? TEMPERATURA_FLAG : PH_FLAG, data);
            *in = (*in + 1) % BUFFER_SIZE;

            pthread_mutex_unlock(mutex);
            sem_post(full);
        } else {
            printf("Error: Medida recibida incorrecta.\n");
        }
    }

    // Esperar 10 segundos y luego enviar mensaje de finalización...
    sleep(10);
    sem_post(&empty_temp);
    sem_post(&empty_ph);

    close(fd);
    return NULL;
}

void *hilo_ph(void *param) {
    FILE *file = fopen(file_ph, "a");
    if (file == NULL) {
        perror("Error al abrir el archivo de pH");
        exit(1);
    }

    while (1) {
        sem_wait(&full_ph);
        pthread_mutex_lock(&mutex_ph);

        if (strncmp(buffer_ph[out_ph], PH_FLAG, strlen(PH_FLAG)) == 0) {
            char *data = buffer_ph[out_ph] + strlen(PH_FLAG) + 1;
            float ph = atof(data);
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

            fprintf(file, "[%s] %f\n", timestamp, ph);
            fflush(file); // Vaciar el buffer inmediatamente

            if (ph < 6 || ph > 8) {
                printf("Alerta: PH fuera de rango! %f\n", ph);
            } else {
              //Imprime pa saber que funciona
              //  printf("PH recibido: %f\n", ph);
            }
        }

        out_ph = (out_ph + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex_ph);
        sem_post(&empty_ph);
    }

    fclose(file);
    return NULL;
}

void *hilo_temperatura(void *param) {
    FILE *file = fopen(file_temp, "a");
    if (file == NULL) {
        perror("Error al abrir el archivo de temperatura");
        exit(1);
    }

    while (1) {
        sem_wait(&full_temp);
        pthread_mutex_lock(&mutex_temp);

        if (strncmp(buffer_temp[out_temp], TEMPERATURA_FLAG, strlen(TEMPERATURA_FLAG)) == 0) {
            char *data = buffer_temp[out_temp] + strlen(TEMPERATURA_FLAG) + 1;
            float temp = atof(data);
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            char timestamp[20];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

            fprintf(file, "[%s] %f\n", timestamp, temp);
            fflush(file); // Vaciar el buffer inmediatamente

            if (temp < 20.0 || temp > 31.6) {
                printf("Alerta: Temperatura fuera de rango! %f\n", temp);
            } else {
              //Esto imprime pa saber que funciona
                //printf("Temperatura recibida: %f\n", temp);
            }
        }

        out_temp = (out_temp + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex_temp);
        sem_post(&empty_temp);
    }

    fclose(file);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 9) {
        fprintf(stderr, "Modo de uso: %s -b <tamaño_buffer> -t <file-temp> -h <file-ph> -p <pipe-nominal>\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            BUFFER_SIZE = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0) {
            file_temp = argv[++i];
        } else if (strcmp(argv[i], "-h") == 0) {
            file_ph = argv[++i];
        }
    }

    printf("Tamaño del buffer: %d\n", BUFFER_SIZE);

    if (!file_temp || !file_ph) {
        fprintf(stderr, "Error al abrir archivo.\n");
        exit(1);
    }

    char *pipe_name = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            pipe_name = argv[++i];
        }
    }

    if (!pipe_name) {
        fprintf(stderr, "Error al abrir pipe.\n");
        exit(1);
    }

    if (access(pipe_name, F_OK) == -1) {
        printf("Creando pipe...\n");
        if (mkfifo(pipe_name, 0666) == -1) {
            perror("Error creando el pipe");
            exit(1);
        }
    } else {
        printf("Pipe existente. No se debe crear.\n");
    }

    printf("Inicializando semáforos...\n");
    sem_init(&empty_temp, 0, BUFFER_SIZE);
    sem_init(&full_temp, 0, 0);
    sem_init(&empty_ph, 0, BUFFER_SIZE);
    sem_init(&full_ph, 0, 0);
    pthread_mutex_init(&mutex_temp, NULL);
    pthread_mutex_init(&mutex_ph, NULL);

    ini_buffers();
    printf("Buffers inicializados\n");

    pthread_t recolector_thread, ph_thread, temperatura_thread;
    pthread_create(&recolector_thread, NULL, recolector, pipe_name);
    pthread_create(&ph_thread, NULL, hilo_ph, NULL);
    pthread_create(&temperatura_thread, NULL, hilo_temperatura, NULL);

    pthread_join(recolector_thread, NULL);
    pthread_join(ph_thread, NULL);
    pthread_join(temperatura_thread, NULL);

    sem_destroy(&empty_temp);
    sem_destroy(&full_temp);
    sem_destroy(&empty_ph);
    sem_destroy(&full_ph);
    pthread_mutex_destroy(&mutex_temp);
    pthread_mutex_destroy(&mutex_ph);

    free_buffers();
    printf("Buffers liberados\n");

    return 0;
}

