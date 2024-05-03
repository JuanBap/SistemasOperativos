#ifndef FUNCIONES_MMC_H
#define FUNCIONES_MMC_H
// Previene la inclusión múltiple del archivo de cabecera, asegurando que este bloque de código se incluya solo una vez.

#include <sys/time.h>
// Incluye las definiciones necesarias para funciones de manejo de tiempo, como gettimeofday.

#define DATA_SIZE (1024*1024*64*3)
// Define una constante que especifica el tamaño de los bloques de datos a ser utilizados para las matrices en la memoria.

struct parametros {
	int nH;  // Número de hilos participantes en el procesamiento.
	int idH; // Identificador único para cada hilo.
	int N;   // Tamaño de la matriz, asumiendo matrices cuadradas NxN.
};
// Define una estructura 'parametros' que almacena la configuración necesaria para ejecutar un hilo que realiza operaciones matriciales.

extern double *mA, *mB, *mC;
// Declara punteros externos para las matrices A, B y C. Estos punteros serán definidos en otro lugar, permitiendo su uso en múltiples archivos.

void llenar_matriz(int SZ);
// Declara una función que llenará las matrices A y B con valores generados y establecerá todos los elementos de C a cero.

void print_matrix(int sz, double *matriz);
// Declara una función que imprime una matriz dada de tamaño 'sz' x 'sz'.

void inicial_tiempo();
// Declara una función para capturar el momento inicial, usada para medir tiempos de ejecución.

void final_tiempo();
// Declara una función para capturar el momento final y calcular la duración desde el tiempo inicializado por 'inicial_tiempo'.

void *mult_thread(void *variables);
// Declara una función que será ejecutada por cada hilo. Esta función toma un puntero a una estructura 'parametros' y realiza la multiplicación de matrices correspondiente a la parte de la matriz que le es asignada.

#endif
// Finaliza la condición de preprocesador para asegurarse de que el cuerpo del archivo de cabecera no se procese más de una vez.
