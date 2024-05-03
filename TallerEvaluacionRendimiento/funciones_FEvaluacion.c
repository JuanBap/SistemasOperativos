#include <stdio.h>                  // Incluye las funciones estándar de entrada y salida.
#include <stdlib.h>                 // Incluye las funciones de manejo de memoria dinámica y otras utilidades.
#include <time.h>                   // Incluye funciones de manejo de tiempo.
#include "funciones_FE.h"           // Incluye el archivo de cabecera que define estructuras y prototipos.

double *mA, *mB, *mC;               // Define punteros globales para las matrices A, B y C.

void llenar_matriz(int SZ){        // Función para llenar las matrices A y B con valores determinísticos.
	srand48(time(NULL));            // Inicializa la semilla del generador de números pseudoaleatorios basado en la hora actual.
	for(int i = 0; i < SZ*SZ; i++){ // Bucle para llenar las matrices A y B.
		mA[i] = 1.1*i;              // Asigna un valor escalado por 1.1 al elemento i de la matriz A.
		mB[i] = 2.2*i;              // Asigna un valor escalado por 2.2 al elemento i de la matriz B.
		mC[i] = 0;                  // Inicializa el elemento i de la matriz C a 0.
	}	
}

void print_matrix(int sz, double *matriz){ // Función para imprimir la matriz.
	if(sz < 12){                           // Solo imprime si la matriz es pequeña para evitar sobrecarga de salida.
		for(int i = 0; i < sz*sz; i++){    // Recorre cada elemento de la matriz.
			if(i%sz == 0) printf("\n");    // Imprime una nueva línea para cada nueva fila de la matriz.
			printf(" %.3f ", matriz[i]);   // Imprime el valor del elemento i con tres decimales.
		}
	}
    printf("\n>-------------------->\n");  // Imprime una línea divisoria después de la matriz.
}

void inicial_tiempo(){                    // Función para iniciar el contador de tiempo.
	struct timeval start;                 // Define una variable local para almacenar el tiempo de inicio.
	gettimeofday(&start, NULL);           // Obtiene el tiempo actual y lo guarda en 'start'.
}

void final_tiempo(){                      // Función para finalizar el contador de tiempo y mostrar el resultado.
	struct timeval stop;                  // Define una variable local para almacenar el tiempo de fin.
	gettimeofday(&stop, NULL);            // Obtiene el tiempo actual y lo guarda en 'stop'.
	stop.tv_sec -= start.tv_sec;          // Calcula la diferencia de tiempo en segundos.
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Muestra el tiempo en microsegundos.
}

void *mult_thread(void *variables){       // Función ejecutada por cada hilo para realizar la multiplicación de matrices.
	struct parametros *data = (struct parametros *)variables; // Convierte el puntero a void en un puntero a 'struct parametros'.
	
	int idH = data->idH;                  // ID del hilo actual.
	int nH  = data->nH;                   // Número total de hilos.
	int N   = data->N;                    // Tamaño de la matriz.
	int ini = (N/nH)*idH;                 // Calcula el índice inicial para este hilo.
	int fin = (N/nH)*(idH+1);             // Calcula el índice final para este hilo.

    for (int i = ini; i < fin; i++){      // Bucle sobre las filas asignadas a este hilo.
        for (int j = 0; j < N; j++){      // Bucle sobre las columnas.
			double *pA, *pB, sumaTemp = 0.0; // Inicializa los punteros para la fila y columna y la suma temporal.
			pA = mA + (i*N);              // Establece pA al comienzo de la fila i de A.
			pB = mB + j;                  // Establece pB al comienzo de la columna j de B.
            for (int k = 0; k < N; k++, pA++, pB+=N){ // Bucle sobre los elementos para realizar el producto punto.
				sumaTemp += (*pA * *pB);  // Suma el producto de los elementos correspondientes.
			}
			mC[i*N+j] = sumaTemp;        // Asigna el resultado a la matriz C.
		}
	}

	pthread_mutex_lock (&MM_mutex);       // Bloquea el mutex antes de la sección crítica (no visible aquí).
	pthread_mutex_unlock (&MM_mutex);     // Desbloquea el mutex después de la sección crítica.
	pthread_exit(NULL);                   // Termina el hilo.
}
