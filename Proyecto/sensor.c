/**************************************************************
#               Pontificia Universidad Javeriana
#     Autores: Juan Pablo Cañón Contreras, Maria Paula Rodríguez Mancera, Natalia Echeverry Salcedo y Daniel Carvajal Gutiérrez
#     Fecha: Mayo 2024
#     Materia: Sistemas Operativos
#     Profesor: John Corredor
#     Tema: Proyecto de Monitoreo de Sensores
#     Fichero: sensor.c
#     Objetivo: Implementar los procesos simulados de sensores que
#               recogen datos de temperatura y PH y los envían al
#               proceso monitor a través de un pipe nominal. Este
#               archivo contiene las funciones necesarias para simular
#               la captura de los datos y su envío, gestionando la 
#               sincronización y la comunicación interprocesos.
#****************************************************************/


// Inclusión de bibliotecas
#include <stdio.h>      //Biblioteca estándar de entrada y salida para funciones como printf, scanf, etc
#include <stdlib.h>     //Biblioteca estándar de utilidades generales, incluyendo manejo de memoria, números aleatorios, etc
#include <string.h>     //Biblioteca para manejo de cadenas y operaciones de memoria
#include <unistd.h>     //Interfaz para llamadas al sistema POSIX, que provee acceso a servicios del sistema operativo, como read, write, close
#include <fcntl.h>      //Definiciones de control de archivos, proporcionando banderas para operaciones con archivos como open (ej. O_RDONLY, O_WRONLY)

//Colores
const char *RED = "\033[31m";
const char *GREEN = "\033[32m";
const char *YELLOW = "\033[33m";
const char *BLUE = "\033[34m";
const char *RESET = "\033[0m"; 


//Inicio del programa principal
int main(int argc, char *argv[]) {
  
    //Verificamos que se proporcionen los argumentos correctos
    if (argc != 9) {
        fprintf(stderr, "Modo de uso: %s -s [1|2] -t tiempo -f archivo -p pipe\n", argv[0]);
        exit(1);
    }

    //Variables puntero para los datos recibidos
    char *sensor = NULL, *archivo = NULL, *tiempo = NULL, *pipe_name = NULL;
  
    //Procesamos los argumentos recibidos de la línea de comandos
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

    //Verificamos que los argumentos sean válidos
    if (!sensor || !archivo || !tiempo || !pipe_name) {
        fprintf(stderr, "Argumentos inválidos.\n");
        fprintf(stderr, "Modo de uso: %s -s [1|2] -t tiempo -f archivo -p pipe\n", argv[0]);
        exit(1);
    }

    //Casteamos el tiempo de espera a entero
    int tiempo_espera = atoi(tiempo);  
    //Casteamos el tipo de sensor a entero
    int sensor_type = atoi(sensor);  

    //Imprimimos decoración e información del sensor
    printf("%s┌─────────────────────────────────────────┐%s\n",GREEN,RESET);
    printf("%s│          INFORMACIÓN DEL SENSOR         │%s\n",GREEN,RESET);
    printf("%s├─────────────────────────────────────────┤%s\n",GREEN,RESET);

    //Abrimos el pipe nominal para escritura
    int fd = open(pipe_name, O_WRONLY);  
    if (fd == -1) {
        perror(" Error abriendo el pipe\n");
        exit(1);
    } else {
        printf("%s│ Pipe abierto correctamente %s\n",GREEN, RESET);
    }

    //Abrimos el archivo de datos del sensor
    FILE *file = fopen(archivo, "r");  
    if (!file) {
        fprintf(stderr, "Error al abrir el archivo: %s\n", archivo);
        close(fd);
        exit(1);
    } else {
      printf("%s│ Archivo abierto correctamente %s\n",GREEN, RESET);

    }
    //Espacio entre información
    printf("%s└─────────────────────────────────────────┘%s\n",GREEN,RESET);

    //Buffer para almacenar líneas leídas del archivo
    char line[100];  
    while (fgets(line, sizeof(line), file)) {
        //Casteamos la línea a un valor flotante
        float value = atof(line);  
        if (value < 0) //Valores negativos los descartamos
            continue;

        //Verificamos si el valor está dentro de los rangos permitidos
        if ((sensor_type == 1 && (value < 20.0 || value > 31.6)) ||
            (sensor_type == 2 && (value < 6.0 || value > 8.0))) {
            printf("%sValor fuera de rango: %.2f%s\n", RED, value, RESET);
        }

        char buffer[128];
        //Formateamos el dato como "<sensor_type>:<value>"
        snprintf(buffer, sizeof(buffer), "%d:%.2f\n", sensor_type, value);

        //Imprimimos el tipo de sensor y el valor en color azul o amarillo dependiendo del sensor
        if (sensor_type == 1) {
            printf("%sSensor envía temperatura: %.2f%s\n", BLUE, value, RESET);
        } else if (sensor_type == 2) {
            printf("%sSensor envía pH: %.2f%s\n", YELLOW, value, RESET);
        }
        
        

        //Escribimos el dato formateado en el pipe nominal
        ssize_t bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written == -1) {
            perror("Error escribiendo en el pipe");
            break;
        }
        //Esperamos antes de leer el siguiente dato
        sleep(tiempo_espera);  
    }
    
    
    //Cerramos el archivo
    fclose(file);  

    //Cerramos el pipe
    close(fd);  

    //Terminamos la ejecución
    return 0;  
}
