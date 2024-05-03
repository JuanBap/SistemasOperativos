#ifndef FUNCIONES_FE_H               // Previene la inclusión múltiple del archivo de cabecera.
#define FUNCIONES_FE_H               // Define un identificador único para el archivo de cabecera.

#include <sys/time.h>                // Incluye definiciones para funciones de manejo de tiempo.

#define DATA_SIZE (1024*1024*64*3)   // Define el tamaño de la memoria reservada para las matrices como una constante.

struct parametros {                 // Define una estructura para almacenar los parámetros pasados a los hilos.
	int nH;                         // Número de hilos.
	int idH;                        // Identificador único de cada hilo.
	int N;                          // Tamaño de la matriz (NxN).
};

extern double *mA, *mB, *mC;        // Declara punteros externos para las matrices A, B y C que serán definidos en otro archivo.

void llenar_matriz(int SZ);         // Declara una función para llenar las matrices A y B con valores.
void print_matrix(int sz, double *matriz); // Declara una función para imprimir una matriz de tamaño 'sz'.
void inicial_tiempo();              // Declara una función para iniciar el conteo del tiempo de ejecución.
void final_tiempo();                // Declara una función para finalizar el conteo del tiempo y mostrar el resultado.
void *mult_thread(void *variables); // Declara una función que será ejecutada por cada hilo para realizar la multiplicación de matrices.

#endif                              // Termina la condición de preprocesador.
