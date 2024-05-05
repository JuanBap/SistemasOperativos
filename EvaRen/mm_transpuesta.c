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

#include <stdio.h>  //Para operaciones de entrada/salida.
#include <pthread.h>  //Para el manejo de hilos POSIX.
#include <unistd.h>  //Para llamadas al sistema POSIX.
#include <stdlib.h>  //Para memoria dinámica y control de procesos.
#include <time.h>  //Para manejo de funciones de tiempo.
#include <sys/time.h>  //Para estructuras de medición de tiempo de alta resolución.

#define DATA_SIZE (1024*1024*64*3)  //Definición del tamaño de la matriz, calculado como 192 MB.

pthread_mutex_t MM_mutex;  //Definición del mutex para controlar el acceso concurrente.
static double MEM_CHUNK[DATA_SIZE];  //Reserva de espacio en memoria estática para las matrices.
double *mA, *mB, *mC;  //Punteros a las matrices A, B y C.

//Estructura para encapsular 
struct parametros{
    int nH;  //Número total de hilos.
    int idH;  //ID del hilo actual.
    int N;  //Tamaño de la matriz (dimensiones NxN).
};

struct timeval start, stop;  //Variables para medir tiempos de ejecución.

void llenar_matriz(int SZ){
    srand48(time(NULL));  //Inicialización del generador de números aleatorios.
    for(int i = 0; i < SZ*SZ; i++){
        mA[i] = 1.1*i; 
        mB[i] = 2.2*i; 
        mC[i] = 0; 
    }   
}

void print_matrix(int sz, double *matriz){
    if(sz < 12){  //Condicional para imprimir solo matrices pequeñas.
        for(int i = 0; i < sz*sz; i++){
            if(i%sz == 0) printf("\n");  //Nueva línea al inicio de cada fila.
            printf(" %.3f ", matriz[i]);  //Impresión de cada elemento con tres decimales.
        }  
        printf("\n>-------------------->\n");
    }
}

void inicial_tiempo(){
    gettimeofday(&start, NULL);  //Captura del tiempo inicial.
}

void final_tiempo(){
    gettimeofday(&stop, NULL);  //Captura del tiempo final.
    stop.tv_sec -= start.tv_sec;  //Cálculo de la diferencia en segundos.
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));  //Impresión del tiempo en microsegundos.
}

void *mult_thread(void *variables){
    struct parametros *data = (struct parametros *)variables;
    int idH = data->idH;
    int nH = data->nH;
    int N = data->N;
    int ini = (N/nH)*idH;  //Determinación del inicio del segmento de matriz para este hilo.
    int fin = (N/nH)*(idH+1);  //Determinación del fin del segmento de matriz para este hilo.

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
            double *pA = mA + (i*N), *pB = mB + (j*N), sumaTemp = 0.0;
            for (int k = 0; k < N; k++, pA++, pB++){
                sumaTemp += (*pA * *pB);  //Multiplicación y suma acumulativa.
            }
            mC[i*N+j] = sumaTemp;  //Almacenamiento del resultado en la matriz C.
        }
    }

    pthread_mutex_lock(&MM_mutex);  //Uso de mutex para proteger una sección crítica.
    pthread_mutex_unlock(&MM_mutex);
    pthread_exit(NULL);  //Terminación del hilo.
}

int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;  //Error si no se proporcionan suficientes argumentos.
    }
    int SZ = atoi(argv[1]);  //Conversión de argumento a entero para tamaño de matriz.
    int n_threads = atoi(argv[2]);  //conversión de argumento a entero para número de hilos.

    pthread_t p[n_threads];  //Arreglo de identificadores de hilos.
    pthread_attr_t atrMM;  // Atributos para los hilos.

    mA = MEM_CHUNK;  //Asignación de la matriz A al inicio del bloque de memoria.
    mB = mA + SZ*SZ;  //Asignación de la matriz B.
    mC = mB + SZ*SZ;  //Asignación de la matriz C.

    llenar_matriz(SZ);  //Llenado inicial de las matrices.
    print_matrix(SZ, mA);  //Impresión de la matriz A.
    print_matrix(SZ, mB);  //Impresión de la matriz B.

    inicial_tiempo();  //Inicio de la medición del tiempo de ejecución.
    pthread_mutex_init(&MM_mutex, NULL);  //Inicialización del mutex.
    pthread_attr_init(&atrMM);  //Inicialización de atributos de hilo.
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);  //Atributos para que los hilos sean 'joinable'.

    for (int j = 0; j < n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
        datos->idH = j;
        datos->nH = n_threads;
        datos->N = SZ;
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);  //Creación de hilos.
    }

    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);  //Espera a que todos los hilos terminen.
    final_tiempo();  //Fin de la medición del tiempo y su impresión.

    print_matrix(SZ, mC);  //Impresión de la matriz resultado C.

    pthread_attr_destroy(&atrMM);  //Destrucción de atributos de hilo.
    pthread_mutex_destroy(&MM_mutex);  //Destrucción del mutex.
    pthread_exit(NULL);  //Finalización de todos los hilos.
}

