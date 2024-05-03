/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J. Corredor
	Fecha: Febrero 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo clásico de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <pthread.h>            // Incluye la biblioteca de hilos POSIX para la gestión de hilos.
#include <unistd.h>             // Proporciona acceso a la API del sistema operativo POSIX.
#include "funciones_MMT.h"      // Incluye el archivo de cabecera donde se declaran las operaciones de matriz y las funciones de hilos.

pthread_mutex_t MM_mutex;       // Declara un mutex utilizado para sincronizar hilos.
static double MEM_CHUNK[DATA_SIZE]; // Declara un arreglo de memoria estática para almacenar los datos de las matrices.

int main(int argc, char *argv[]){
	if (argc < 2){               // Comprueba si el número de argumentos de la línea de comandos es menor al requerido.
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	            // Si no hay suficientes argumentos, imprime cómo deben ingresarse y termina el programa con un código de error.
	}
    int SZ = atoi(argv[1]);      // Convierte el primer argumento de la línea de comandos a un entero para el tamaño de la matriz.
    int n_threads = atoi(argv[2]); // Convierte el segundo argumento a un entero para el número de hilos.

    pthread_t p[n_threads];      // Declara un arreglo de identificadores de hilo de tipo pthread_t.
    pthread_attr_t atrMM;        // Declara una variable para los atributos de los hilos.

	mA = MEM_CHUNK;              // Inicializa el puntero de la matriz A al comienzo del bloque de memoria.
	mB = mA + SZ*SZ;             // Inicializa el puntero de la matriz B justo después de la matriz A en el bloque de memoria.
	mC = mB + SZ*SZ;             // Inicializa el puntero de la matriz C justo después de la matriz B.

	llenar_matriz(SZ);           // Llama a la función para llenar las matrices A y B con valores y establecer C en cero.
	print_matrix(SZ, mA);        // Imprime la matriz A.
	print_matrix(SZ, mB);        // Imprime la matriz B.

	inicial_tiempo();            // Inicia la medición del tiempo de ejecución.
	pthread_mutex_init(&MM_mutex, NULL); // Inicializa el mutex con atributos predeterminados.
	pthread_attr_init(&atrMM);   // Inicializa los atributos del hilo con valores predeterminados.
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Establece los hilos como "joinable".

    for (int j=0; j<n_threads; j++){ // Bucle para crear los hilos.
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;           // Asigna el ID del hilo.
		datos->nH = n_threads;    // Asigna el número total de hilos.
		datos->N = SZ;            // Asigna el tamaño de la matriz.
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea un hilo para ejecutar la función mult_thread.
	}

    for (int j=0; j<n_threads; j++) // Espera a que todos los hilos terminen.
        pthread_join(p[j],NULL);
	final_tiempo();               // Finaliza la medición del tiempo y lo imprime.

	print_matrix(SZ, mC);         // Imprime la matriz C resultante.

	pthread_attr_destroy(&atrMM); // Destruye los atributos de hilo.
	pthread_mutex_destroy(&MM_mutex); // Destruye el mutex.
	pthread_exit (NULL);           // Termina el hilo principal y libera recursos.
}
