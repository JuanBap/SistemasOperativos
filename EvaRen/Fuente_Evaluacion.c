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

/**************************************************************
		Pontificia Universidad Javeriana
	Autor: Juan Pablo Cañón
	Fecha: Mayo 2024
	Materia: Sistemas Operativos
	Tema: Solución de Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Comparar la ejecución de un algoritmo: serie y paralelo,
			  comparar diferentes sistemas de cómputo, analizar e interpretar
			  los resultados para extraer recomendaciones y conclusiones y
			  elaborar un informe de evaluación en formato pdf. 
****************************************************************/

//Bibliotecas estándar para entrada/salida, hilos, funciones estándar, manejo de tiempo, etc.
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

//Define la cantidad total de datos para las matrices 
#define DATA_SIZE (1024*1024*64*3)

//Declaración de un mutex para controlar el acceso a secciones críticas entre hilos
pthread_mutex_t MM_mutex;
//Reserva de un bloque de memoria estática para las matrices
static double MEM_CHUNK[DATA_SIZE];
//Punteros para las matrices A, B y C
double *mA, *mB, *mC;

//Estructura para almacenar parámetros a pasar a los hilos (número de hilo, ID y tamaño de la matriz)
struct parametros{
    int nH;
    int idH;
    int N;
};

//Variables para medir tiempo de ejecución
struct timeval start, stop;

//Función para llenar las matrices A y B con valores y inicializar C a 0
void llenar_matriz(int SZ){ 
    srand48(time(NULL));  //Establecer la semilla para la generación de números aleatorios basada en el tiempo actual
    for(int i = 0; i < SZ*SZ; i++){
        mA[i] = 1.1*i;  //Llenar la matriz A con valores incrementales
        mB[i] = 2.2*i;  //Llenar la matriz B con valores incrementales
        mC[i] = 0;      //Inicializar la matriz C con ceros
    }   
}

//Función para imprimir matrices 
void print_matrix(int sz, double *matriz){
    if(sz < 12){  //Condición para limitar la impresión a matrices pequeñas
        for(int i = 0; i < sz*sz; i++){
            if(i%sz==0) printf("\n");  //Nueva línea por cada fila de la matriz
            printf(" %.3f ", matriz[i]);  //Imprimir cada elemento de la matriz con tres decimales
        }   
    }
    printf("\n>-------------------->\n");
}

//Función para capturar el tiempo de inicio
void inicial_tiempo(){
    gettimeofday(&start, NULL);
}

//Función para capturar el tiempo de finalización y calcular la duración
void final_tiempo(){
    gettimeofday(&stop, NULL);	//Finaliza la toma captura de tiempo
    stop.tv_sec -= start.tv_sec;	//Calculo de la diferencia de tiempo
    printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec*1000000 + stop.tv_usec));  //Imprimir el tiempo en microsegundos
}

//Función que realiza la multiplicación de matrices utilizando múltiples hilos
void *mult_thread(void *variables){
    struct parametros *data = (struct parametros *)variables;  //Castear el puntero de entrada a la estructura de parámetros
    
    int idH = data->idH;	//ID del hilo
    int nH  = data->nH;		//Número total de hilos
    int N   = data->N;		//Tamaño de la matriz
    int ini = (N/nH)*idH;	//Calcular el índice de inicio para este hilo
    int fin = (N/nH)*(idH+1);	//Calcular el índice de fin para este hilo

    for (int i = ini; i < fin; i++){  //Ciclo sobre las filas asignadas a este hilo
        for (int j = 0; j < N; j++){  //Ciclo sobre todas las columnas
            double *pA, *pB, sumaTemp = 0.0;	//Variables auxiliares
            pA = mA + (i*N);	//Apuntador al inicio de la fila de A
            pB = mB + j;	//Apuntador al inicio de la columna de B
            for (int k = 0; k < N; k++, pA++, pB+=N){	//Ciclo sobre elementos de la fila de A y columna de B
                sumaTemp += (*pA * *pB);	//Multiplicar y acumular el resultado
            }
            mC[i*N+j] = sumaTemp;	//Guardar el resultado en C
        }
    }

    //Bloqueo y desbloqueo del mutex 
    pthread_mutex_lock (&MM_mutex);
    pthread_mutex_unlock (&MM_mutex);
    pthread_exit(NULL);	//Terminar el hilo
}

//Función principal que prepara datos, crea hilos, y coordina la ejecución y finalización de la multiplicación
int main(int argc, char *argv[]){
    if (argc < 2){	//Revisa que el ingreso de argumentos sea correcto
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;  
    }
    int SZ = atoi(argv[1]);	//Tamaño de la matriz desde los argumentos
    int n_threads = atoi(argv[2]);	//Número de hilos desde los argumentos

    pthread_t p[n_threads];	//Arreglo de identificadores de hilos
    pthread_attr_t atrMM;	//Atributos para los hilos

    mA = MEM_CHUNK;		//Asignar memoria para A
    mB = mA + SZ*SZ;	//Asignar memoria para B después de A
    mC = mB + SZ*SZ;	//Asignar memoria para C después de B

    llenar_matriz(SZ);	//Llenar las matrices A y B
    print_matrix(SZ, mA);  //Imprimir matriz A
    print_matrix(SZ, mB);  //Imprimir matriz B

    inicial_tiempo();  //Capturar tiempo de inicio
    pthread_mutex_init(&MM_mutex, NULL);	//Inicializar mutex
    pthread_attr_init(&atrMM);	//Inicializar atributos de hilos
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);	// Establecer los hilos como "joinable"
	
	//Ciclo para reservar memoria y asignar atributos a los hilos
    for (int j=0; j<n_threads; j++){
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); // Reservar memoria para parámetros
        datos->idH = j;	//Asignar ID de hilo
        datos->nH  = n_threads;	//Asignar número total de hilos
        datos->N   = SZ;	//Asignar tamaño de la matriz
        pthread_create(&p[j],&atrMM,mult_thread,(void *)datos);	//Crear hilo
    }

	//Ciclo para joinear los hilos
    for (int j=0; j<n_threads; j++)
        pthread_join(p[j],NULL);  //Esperar a que todos los hilos terminen
    final_tiempo();  //Capturar tiempo de finalización y mostrar duración
    
    print_matrix(SZ, mC);	//Imprimir la matriz resultado C

    pthread_attr_destroy(&atrMM);  //Destruir atributos de hilos
    pthread_mutex_destroy(&MM_mutex);// Destruir mutex
    pthread_exit (NULL);  //Terminar el proceso principal de hilos

	//Finaliza exitosamente el programa
}
