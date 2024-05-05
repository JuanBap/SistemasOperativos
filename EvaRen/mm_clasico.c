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

//Bibliotecas estándar para entrada/salida, hilos, funciones estándar, manejo de tiempo, etc.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3)  //Define el tamaño total de datos para las matrices

//Variables globales para mutex y bloques de memoria para matrices
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE];
double *mA, *mB, *mC;

//Estructura para pasar parámetros a los hilos
struct parametros{
	int nH;  //Número de hilos
	int idH;  //ID del hilo actual
	int N;  //Tamaño de la matriz (N x N)
};

//Variables para medición de tiempo
struct timeval start, stop;

//Función para inicializar las matrices con valores específicos
void llenar_matriz(int SZ){ 
	srand48(time(NULL));  //Generación de números aleatorios
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1*i;  //Valores incrementales para mA
		mB[i] = 2.2*i;  //Valores incrementales para mB
		mC[i] = 0;      //Inicializa mC a cero
	}	
}

//Función para imprimir matrices de tamaño pequeño
void print_matrix(int sz, double *matriz){
	if(sz < 12){  //Solo imprime si la matriz es suficientemente pequeña
		for(int i = 0; i < sz*sz; i++){
			if(i%sz==0) printf("\n");  //Nueva línea por cada fila de la matriz
			printf(" %.3f ", matriz[i]);  //Imprime cada elemento con tres decimales
		}	
		printf("\n>-------------------->\n");
	}
}

//Función para capturar el tiempo de inicio
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

//Función para capturar el tiempo de finalización y calcular la duración
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));
}

//Función que los hilos ejecutarán para realizar la multiplicación de matrices
void *mult_thread(void *variables){
	//Castear la estructura
	struct parametros *data = (struct parametros *)variables;
	
	//Asignar a data lo correspondiente
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N/nH)*idH;	//Inicio del segmento de la matriz para este hilo
	int fin = (N/nH)*(idH+1);	//Fin del segmento de la matriz para este hilo

    for (int i = ini; i < fin; i++){
        for (int j = 0; j < N; j++){
			double *pA, *pB, sumaTemp = 0.0;	//Variables auxiliares
			pA = mA + (i*N);  //Puntero al inicio de la fila i de A
			pB = mB + j;  //Puntero al inicio de la columna j de B
            for (int k = 0; k < N; k++, pA++, pB+=N){
				sumaTemp += (*pA * *pB);  //Multiplicación y suma acumulativa
			}
			mC[i*N+j] = sumaTemp;  //Asignar resultado a la matriz C
		}
	}

	pthread_mutex_lock (&MM_mutex);  //Inicia la sección crítica 
	pthread_mutex_unlock (&MM_mutex);//Finaliza la sección crítica
	pthread_exit(NULL);  // Terminar el hilo
}

// Función principal que configura y lanza hilos para la multiplicación de matrices
int main(int argc, char *argv[]){
	if (argc < 2){ //Verifica la cantidad de argumentos que sea 2
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    int SZ = atoi(argv[1]);  //Tamaño de la matriz desde argumentos
    int n_threads = atoi(argv[2]);  //Número de hilos desde argumentos

    pthread_t p[n_threads];  //Arreglo de identificadores de hilos
    pthread_attr_t atrMM;

	mA = MEM_CHUNK;  //Inicialización de punteros para las matrices
	mB = mA + SZ*SZ;
	mC = mB + SZ*SZ;

	llenar_matriz(SZ);  //Llenar las matrices A y B
	print_matrix(SZ, mA);  //Imprimir matriz A
	print_matrix(SZ, mB);  //Imprimir matriz B

	inicial_tiempo();  //Iniciar medición de tiempo
	pthread_mutex_init(&MM_mutex, NULL);  //Inicializar mutex
	pthread_attr_init(&atrMM);  //Inicializar atributos de los hilos
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);  //Configurar hilos como "joinable"

    for (int j=0; j<n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));  //Asignar memoria para parámetros
		datos->idH = j;
		datos->nH  = n_threads;
		datos->N   = SZ;
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);  //Crear hilos
	}

    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);  //Esperar a que todos los hilos terminen
	final_tiempo();  //Mostrar tiempo transcurrido
	
	print_matrix(SZ, mC);  //Imprimir matriz resultado C

	pthread_attr_destroy(&atrMM);  //Destruir atributos de hilos
	pthread_mutex_destroy(&MM_mutex);  //Destruir mutex
	pthread_exit (NULL);  //Finalizar proceso principal
}
