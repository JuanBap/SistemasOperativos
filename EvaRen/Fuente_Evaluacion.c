/**************************************************************
#               Pontificia Universidad Javeriana
#     Autor: Juan Pablo Cañón Contreras
#     Fecha: Mayo 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: Fuente_Evaluacion.c 
#	  Objetivo: Documentar el código y comprender su funcionamiento
                para la experimentación
#****************************************************************/

#include "functions.h"  //Incluye el archivo de cabecera que contiene las definiciones de funciones y estructuras necesarias
#include <stdio.h>      //Biblioteca estándar de entrada/salida para funciones como printf
#include <pthread.h>    //Biblioteca para el manejo de hilos POSIX
#include <unistd.h>     //Proporciona acceso a la funcionalidad del sistema operativo POSIX
#include <stdlib.h>     //Incluye funciones para manejo de memoria dinámica, control de procesos y conversiones
#include <time.h>       //Permite utilizar funciones relacionadas con el manejo del tiempo
#include <sys/time.h>   //Proporciona definiciones adicionales para estructuras y funciones de manejo de tiempo


//Inicio del programa principal
int main(int argc, char *argv[]) {
    if (argc < 3) {  //Verifica que se hayan ingresado suficientes argumentos al programa
        printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
        return -1;  //Finaliza el programa con un código de error si no hay suficientes argumentos
    }
    int SZ = atoi(argv[1]); //Convierte el primer argumento a entero para el tamaño de la matriz
    int n_threads = atoi(argv[2]); //Convierte el segundo argumento a entero para el número de hilos

    pthread_t p[n_threads]; //Array para almacenar los identificadores de los hilos.
    pthread_attr_t atrMM; //Variable para configurar atributos de los hilos.

    //Asigna memoria para las matrices A, B y C dentro del bloque de memoria reservado
    mA = MEM_CHUNK;
    mB = mA + SZ*SZ;
    mC = mB + SZ*SZ;

    llenar_matriz(SZ); //Llena las matrices A y B con valores y establece C en cero
    print_matrix(SZ, mA); //Imprime la matriz A
    print_matrix(SZ, mB); //Imprime la matriz B

    inicial_tiempo(); //Captura el tiempo de inicio del procesamiento
    pthread_mutex_init(&MM_mutex, NULL); //Inicializa el mutex
    pthread_attr_init(&atrMM); //Inicializa los atributos de los hilos
    pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); //Configura los hilos como joineables

    //Crea los hilos para realizar la multiplicación de matrices
    for (int j = 0; j < n_threads; j++) {
        struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros));
        datos->idH = j;
        datos->nH = n_threads;
        datos->N = SZ;
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos);
    }

    //Espera a que todos los hilos terminen su ejecución
    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL);

    final_tiempo(); //Captura el tiempo de finalización y calcula la duración total
    print_matrix(SZ, mC); //Imprime la matriz C resultante

    //Limpia los recursos utilizados.
    pthread_attr_destroy(&atrMM);
    pthread_mutex_destroy(&MM_mutex);
    pthread_exit(NULL); //Finaliza todos los hilos y limpia.
}
