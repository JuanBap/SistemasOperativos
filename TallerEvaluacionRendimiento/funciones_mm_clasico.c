#include <stdio.h>                  // Incluye la biblioteca estándar de entrada y salida para funciones como printf.
#include <stdlib.h>                 // Incluye la biblioteca estándar para manejo de memoria y conversiones.
#include <time.h>                   // Incluye la biblioteca para manejo de tiempo, útil para funciones de tiempo.
#include "funciones_MMC.h"          // Incluye un archivo de cabecera personalizado que define estructuras y prototipos de funciones.

double *mA, *mB, *mC;               // Declara punteros a tres matrices globales, A, B y C.

void llenar_matriz(int SZ){        // Función para llenar las matrices A y B con valores y C con ceros.
	srand48(time(NULL));            // Establece la semilla para la generación de números aleatorios con la hora actual.
	for(int i = 0; i < SZ*SZ; i++){ // Itera sobre cada elemento de las matrices basadas en su tamaño SZxSZ.
			mA[i] = 1.1*i;          // Asigna a cada elemento de la matriz A un valor determinístico escalado por 1.1.
			mB[i] = 2.2*i;          // Asigna a cada elemento de la matriz B un valor determinístico escalado por 2.2.
			mC[i] = 0;              // Inicializa cada elemento de la matriz C a 0.
		}	
}

void print_matrix(int sz, double *matriz){ // Función para imprimir una matriz de tamaño sz.
	if(sz < 12){                           // Restringe la impresión a matrices de tamaño menor de 12x12 para evitar salida excesiva.
		for(int i = 0; i < sz*sz; i++){    // Itera sobre cada elemento de la matriz.
			if(i%sz == 0) printf("\n");    // Imprime un salto de línea al inicio de cada fila.
			printf(" %.3f ", matriz[i]);   // Imprime el valor del elemento con tres decimales.
		}
    	printf("\n>-------------------->\n"); // Imprime una línea para separar visualmente las matrices impresas.
	}
}

void inicial_tiempo(){                    // Función para capturar el tiempo de inicio de un evento.
	struct timeval start;                 // Declara una variable para almacenar el tiempo de inicio.
	gettimeofday(&start, NULL);           // Obtiene el tiempo actual y lo guarda en start.
}

void final_tiempo(){                      // Función para capturar el tiempo de finalización y calcular la duración.
	struct timeval stop;                  // Declara una variable para almacenar el tiempo de finalización.
	gettimeofday(&stop, NULL);            // Obtiene el tiempo actual y lo guarda en stop.
	stop.tv_sec -= start.tv_sec;          // Calcula la diferencia en segundos entre start y stop.
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec)); // Imprime la duración en microsegundos.
}

void *mult_thread(void *variables){       // Función que ejecuta cada hilo para realizar parte de la multiplicación de matrices.
	struct parametros *data = (struct parametros *)variables; // Convierte el puntero void en un puntero a la estructura 'parametros'.
	
	int idH = data->idH;                  // Identificador del hilo.
	int nH  = data->nH;                   // Número total de hilos.
	int N   = data->N;                    // Tamaño de la matriz.
	int ini = (N/nH)*idH;                 // Calcula el índice inicial de la porción de matriz que este hilo procesará.
	int fin = (N/nH)*(idH+1);             // Calcula el índice final de la porción de matriz que este hilo procesará.

    for (int i = ini; i < fin; i++){      // Bucle que recorre las filas asignadas al hilo.
        for (int j = 0; j < N; j++){      // Bucle que recorre las columnas.
			double *pA, *pB, sumaTemp = 0.0; // Variables temporales para los punteros y para almacenar la suma de productos.
			pA = mA + (i*N);              // Establece pA al inicio de la fila i de A.
			pB = mB + j;                  // Establece pB al inicio de la columna j de B, ajustando para avanzar por columnas.
            for (int k = 0; k < N; k++, pA++, pB+=N){ // Bucle que realiza la multiplicación de los elementos y suma sus productos.
				sumaTemp += (*pA * *pB);  // Suma el producto de los elementos correspondientes.
			}
			mC[i*N+j] = sumaTemp;        // Asigna el resultado acumulado al elemento correspondiente en C.
		}
	}

	pthread_mutex_lock (&MM_mutex);       // Bloquea un mutex para proteger una sección crítica (no mostrada aquí).
	pthread_mutex_unlock (&MM_mutex);     // Desbloquea el mutex.
	pthread_exit(NULL);                   // Termina el hilo, liberando recursos asociados.
}
