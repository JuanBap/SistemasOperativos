#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 9) {
        fprintf(stderr, "Modo de uso: %s -s [1|2] -t tiempo -f archivo -p pipe\n", argv[0]);
        exit(1);
    }

    char *sensor = NULL, *archivo = NULL, *tiempo = NULL, *pipe_name = NULL;
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-s") == 0)
            sensor = argv[i + 1];
        else if (strcmp(argv[i], "-t") == 0)
            tiempo = argv[i + 1];
        else if (strcmp(argv[i], "-f") == 0)
            archivo = argv[i + 1];
        else if (strcmp(argv[i], "-p") == 0)
            pipe_name = argv[i + 1];
    }

    if (!sensor || !archivo || !tiempo || !pipe_name) {
        fprintf(stderr, "Argumentos inválidos.\n");
        fprintf(stderr, "Modo de uso: %s -s [1|2] -t tiempo -f archivo -p pipe\n", argv[0]);
        exit(1);
    }

    int tiempo_espera = atoi(tiempo);
    int sensor_type = atoi(sensor);

    printf("Abriendo el pipe...\n");
    int fd = open(pipe_name, O_WRONLY);
    if (fd == -1) {
        perror("Error abriendo el pipe");
        exit(1);
    } else {
        printf("Pipe abierto correctamente...\n");
    }

    FILE *file = fopen(archivo, "r");
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo: %s\n", archivo);
        close(fd);
        exit(1);
    } else {
        printf("Archivo abierto correctamente...\n");
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        float value = atof(line);
        if (value < 0) // Valores negativos se descartan
            continue;

        // Verificar si el valor está dentro de los rangos permitidos
        if ((sensor_type == 1 && (value < 20.0 || value > 31.6)) ||
            (sensor_type == 2 && (value < 6.0 || value > 8.0))) {
            printf("Valor fuera de rango: %f\n", value);
        }

        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%d:%f\n", sensor_type, value);
        printf("Sensor envía %d:%f\n", sensor_type, value);

        ssize_t bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written == -1) {
            perror("Error escribiendo en el pipe");
            break;
        }

        sleep(tiempo_espera);
    }

    fclose(file);
    close(fd);

    return 0;
}
