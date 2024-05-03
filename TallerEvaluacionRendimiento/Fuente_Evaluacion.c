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

/***************************************************************
		Pontificia Universidad Javeriana
		Solución Taller de Evaluación de Rendimiento
	Autor: Juan Pablo Cañón Contreras
	Fecha: Mayo 10 del 2024
	Tema: Solución del Taller de Evaluación de Rendimiento
	Fichero: Fuente_Evaluacion.c -> Programa principal
	Objetivo: 

***************************************************************/


#include <pthread.h>            // Incluye las funciones de la biblioteca pthread para el manejo de hilos
#include <unistd.h>             // Incluye llamadas a sistema estándares como getpid()
#include <sys/time.h>           // Incluye definiciones para manejo de tiempo, incluyendo gettimeofday()
#include "funciones_FE.h"       // Incluye las declaraciones de funciones y variables externas usadas

pthread_mutex_t MM_mutex;       // Define un mutex global para la sincronización entre hilos
static double MEM_CHUNK[DATA_SIZE]; // Define un array estático para almacenar las matrices en memoria

int main(int argc, char *argv[]){
	if (argc < 2){               // Comprueba que se hayan pasado al menos dos argumentos al programa
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n"); // Imprime cómo se deben ingresar los argumentos si no se cumplen los requerimientos
		return -1;               // Termina el programa con un código de error si no hay suficientes argumentos
	}
    int SZ = atoi(argv[1]);      // Convierte el primer argumento en un entero para obtener el tamaño de la matriz
    int n_threads = atoi(argv[2]); // Convierte el segundo argumento en un entero para obtener el número de hilos

    pthread_t p[n_threads];      // Crea un array de pthread_t para almacenar los identificadores de los hilos
    pthread_attr_t atrMM;        // Define los atributos de los hilos

	mA = MEM_CHUNK;              // Inicializa el puntero de la matriz A apuntando al inicio del bloque de memoria
	mB = mA + SZ*SZ;             // Inicializa el puntero de la matriz B después de la matriz A en memoria
	mC = mB + SZ*SZ;             // Inicializa el puntero de la matriz C después de la matriz B en memoria

	llenar_matriz(SZ);           // Llama a la función para llenar las matrices A y B con valores
	print_matrix(SZ, mA);        // Imprime la matriz A
	print_matrix(SZ, mB);        // Imprime la matriz B

	inicial_tiempo();            // Inicia el contador de tiempo para medir el rendimiento
	pthread_mutex_init(&MM_mutex, NULL); // Inicializa el mutex con atributos por defecto
	pthread_attr_init(&atrMM);   // Inicializa los atributos de los hilos con valores por defecto
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Configura los hilos como 'joinable'

    for (int j=0; j<n_threads; j++){ // Bucle para crear los hilos
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Asigna memoria para los datos que se pasarán a los hilos
		datos->idH = j;             // Establece el ID del hilo
		datos->nH  = n_threads;     // Establece el número total de hilos
		datos->N   = SZ;            // Establece el tamaño de la matriz
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos); // Crea un hilo y le pasa la función 'mult_thread' y los datos
	}

    for (int j=0; j<n_threads; j++) // Bucle para esperar que todos los hilos terminen
        pthread_join(p[j],NULL);   // 'Joins' cada hilo con el hilo principal
	final_tiempo();                // Detiene el contador de tiempo y muestra el resultado

	print_matrix(SZ, mC);          // Imprime la matriz C resultante

	pthread_attr_destroy(&atrMM); // Destruye los atributos de los hilos
	pthread_mutex_destroy(&MM_mutex); // Destruye el mutex
	pthread_exit (NULL);           // Termina el hilo principal y libera recursos
}
