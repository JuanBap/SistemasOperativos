/****************************
Fecha : 12/04/2024
Autor: Juan Pablo Cañón Contreras
 * Materia: Sistemas Operativos
 * Tema: Concurrencia
 * Objetivo: Hacer una metodología para la implementación de la multiplicación
de matrices.
 * La idea principal, es construir paso a paso la implementación para hacer uso
de la
 * biblioteca PTHREAD. Se implementa el Algoritmo Clásico de multiplicación de
matrices, para
 * matrices cuadradas, es decir, la dimensión de filas es igual a la de
columnas.
 * A continuación se describen los pasos:
 * 	- Reserva de memoria
 * 		- Creación de punteros para matrices del tipo doble
 * 		- Asignación de memoria
 * 		- Ingreso de argumentos de entrada (Dimensión matriz, número de
hilos) *		- Validación argumentos de entrada *		-
Inicializar las matrices
 *  	- Imprimir las matrices
 *  	- Función para inicializar las matrices
 *  	- Función para imprimir las matrices
 *  	- Algoritmo clásico de multiplicación matrices
 * 	- Se verifica el resultado
 *  	- Función para multiplicación las matrices
 *  	- Declaración vector de hilos
 *  	- Creación de hilos según tamaño de vector de hilos
 *  	- Crear estructura de datos que encapsule los argumentos de entrada de
la función MM
 *   Implementación de paralelismo: resolución de la multiplicación de matrices
 * 	- Se crea el vector de hilos
 * 	- Se tiene pendiente la exclusión de los hilos
 * 	- Se pasa a globales las matrices
 * 	- Encapsular los datos para enviarlos a la función MxM
 * 	- Se desencapsulan los datos dentro de la función MxM (descomprimen)
 *  - Se realizan las operaciones respectivas y el llamado de funciones
************************************************************************/

// VERSION FINAL ALGORITMO DE MULTIPLICACIÓN DE MATRICES

#include <pthread.h>  //Manejo de hilos
#include <stdio.h>    //Entrada de datos y salida
#include <stdlib.h>   //Memoria dinámica y otras funciones generales
#include <sys/time.h> //Manejo de tiempo
#include <unistd.h>   //Funciones de bajo nivel de gestión de procesos

// Reservamos un espacio de memoria suficiente
#define RESERVA (1024 * 128 * 64 * 8)

// Creamos una estructura de datos para encapsular su contenido
struct datosMM {
  int SZ;   // Tamaño de la matriz cuadrada
  int H;    // Cantidad de hilos
  int id_H; // ID del Hilo creado
};

// Declaramos de manera global el pedazo de memoria del tamaño de RESERVA
static double MEM_CHUNK[RESERVA];
// Es estático porque el uso de apuntadores requiere que este espacio de memoria
// sea fijo

// Declaramos apuntadores de manera global
double *mA, *mB, *mC;
// Cada apuntador va dirigido a las matrices que vamos a operar
// mA y mB son los multiplicandos y mC el producto

// Función inicializar matrices
void initMatrices(int SZ, double *ptrMA, double *ptrMB, double *ptrMC) {
  // Llena las matrices MA, MB y MC con valores específicos
  for (int i = 0; i < SZ * SZ; i += SZ) { // Itera por las columnas
    for (int j = 0; j < SZ; j++) {        // Itera por filas
      ptrMA[i + j] = 1.1 * (i + j);       // Inicializa la matriz A
      ptrMB[i + j] = 2.2 + (i + j);       // Inicializa la matriz B
      ptrMC[i + j] = i + j;               // Inicializa la matriz C
    }
  }
}

// Función imprimir matrices
void imprimirMatriz(int SZ, double *ptr) {
  //Primer ciclo recorre columnas columnas
  for (int i = 0; i < SZ * SZ; i++) {
    if (i % SZ == 0) {
      printf("\n");
      //Segundo ciclo recorre filas
      for (int j = 0; j < SZ;j++) { //Imprime margen
        printf("───────"); //Barra separadora
      }
      printf("\n"); //Salto de línea para próxina fila
    }
    printf(" %.2f ",ptr[i]); //Imprime contenido de matriz y solo muestra dos cifras decimales
  }

  printf("\n"); //Salto de línea

  for (int j = 0; j < SZ;j++) {
    printf("───────"); // Bara separadora
  }
  printf("\n\n"); // Finalización de la impresión
}

// Función que multiplica las matrices
void *multiplicarMatrices(void *argMM) { // Recibe datos encapsulados
  double sumaTemp, *pA, *pB, *pC; // Inicializamos los operadores que   
  //Vamos a utilizar para la multiplicación
  // sumaTemp llevará la cuenta y los demás estarán apuntando a las
  // casillas de las distintas matrices

  // Realizamos un casteo de datos a la estructura
  struct datosMM *intValor = (struct datosMM *)argMM;

  // Desencapsulamos los datos de la estructura y los asignamos
  int SZ = intValor->SZ;    // SZ para el tamaño
  int H = intValor->H;      // H para la cantidad de hilos
  int nHi = intValor->id_H; // nHi para el ID del hilo

  int ini = nHi * (SZ / H);            // Indica el inicio del hilo
  int fin = nHi * (SZ / H) + (SZ / H); // Indica el final del hilo

  // Imprimimos el respectivo hilo con su inicio fin e ID
  printf("\nNúmero de Hilo: %d, Inicio: %d, Fin: %d", nHi, ini, fin);

  for (int i = ini; i < fin; i++) { //Itera sobre las filas
    for (int j = 0; j < SZ; j++) {  //Itera sobre las columnas
      sumaTemp = 0.0;               //Inicializa desde 0 para evitar basura
      pA = mA + i * SZ;             //Apunta inicio de matriz A
      pB = mB + j;                  //Apunta a la primera columna de matriz B
      //Itera de uno a uno los elementos de fila y columnas de matriz A y B
      for (int k = 0; k < SZ; k++, pA++, pB += SZ) { 
        //Multiplica A y B fila por columna y almacena resultado
        sumaTemp += *pA * *pB; 
      }
      //Se asigna el resultado almacenado en la matriz C (i,j)
      mC[j + i * SZ] = sumaTemp; 
    }
  }
  pthread_exit(NULL); // Finaliza el proceso de hilos
}

// Inicio del programa principal
int main(int argc, char *argv[]) {

  // Creamos un arreglo de apuntadores
  double *matrices[3];
  // Declaramos las variables SZ y NH para que reciban los argumentos
  int SZ; // Tamaño de matriz A y B, asímismo para C. Dado que son matrices
          // cuadradas
  int NH; // Cantidad de hilos

  if (argc > 2) { // Si se han recibido dos argumentos verifica entonces:

    // Declaramos las variables SZ y NH para que reciban los argumentos
    SZ = atoi(argv[1]); // Casteamos el número a entero para el tamaño
    NH = atoi(argv[2]); // Casteamos el número a entero para la cantidad de
                        // hilos

    // Verificamos la cantidad de hilos
    if (NH % 2 != 0) {
      printf("ERROR: El valor del numero de hilos debe ser par");
      return 1; // Sale del programa
    }

    // Verificamos que la cantidad de hilos sea mayor a 0
    if (SZ < 1 || NH < 1) {
      printf("ERROR: Cantidad de hilos debe ser mayor a 0\n");
      return 1; // Salida no exitosa
    }

  } else { // Si no se reciben los argumentos esperados
    // En caso de no ser correcto mostramos en pantalla
    printf("ERROR: Número de argumentos incorrectos\n");
    // Imprime un ejemplo de como debe ser
    printf("\n\t $ejetucable.exe MATRIZ_SIZE THREADS_QUANTITY \n");
    return -1;
  }

  //Declaramos el vector con la cantidad de hilos
  pthread_t hilos[NH]; //NH es asignado por el usuario al momento de ingresarlo por consola


  //Definimos que mA va a apuntar al inicio del MEM_CHUNK
  mA = MEM_CHUNK;          //Reserva memoria para matriz A
  mB = mA + (SZ * SZ + 1); //Reserva memoria para matriz B
  mC = mB + (SZ * SZ);     //Reserva memoria para matriz C (Resultado)

  //Hacemos el llamado de la función para llenar la matriz
  initMatrices(SZ, mA, mB, mC);

  

  //Hacemos el llamado de la función para imprimir la matriz y enviamos las variables
  printf("\nMatriz A: \n");
  imprimirMatriz(SZ, mA);
  printf("\nMatriz B: \n");
  imprimirMatriz(SZ, mB);

  //Imprime la información de los hilos llamados a continuación
  printf("\nInformación hilos: \n");

  //Recorre la cantidad de hilos
  for (int h = 0; h < NH; h++) { 
    
    //Se reserva memoria dinámicamente para una estructura de datos
    struct datosMM *valoresMM = (struct datosMM *)malloc(sizeof(struct datosMM));

    //Se asignan los valores relevantes a la estructura de datos
    valoresMM->SZ = SZ;  //Tamaño de las matrices
    valoresMM->H = NH;  //Cantidad total de hilos
    valoresMM->id_H = h;  //Identificador del hilo actual

    //Creación del hilo para ejecutar la función multiplicarMatrices
    pthread_create(&hilos[h], NULL, multiplicarMatrices, valoresMM);
  }

  
  //Recorre la cantidad de hilos
  for (int h = 0; h < NH; h++) {
    //Espera a que el hilo[h] termine su ejecución
    pthread_join(hilos[h], NULL);
  }

  //Hace el llamado para imprimir la matriz resultante
  printf("\n\n\nMatriz C: \n");
  imprimirMatriz(SZ, mC);

  //Fin del programa !!!
  printf("Fin del programa!!! \nAutor: Juan Pablo Cañón Contreras\n");
  
  //Termina el hilo actual y devuelve un valor de salida nulo
  pthread_exit(NULL);

  
}