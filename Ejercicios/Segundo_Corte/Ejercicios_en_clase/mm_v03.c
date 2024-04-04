/*********************************************************
* Fecha: 4/04/2024
* Autor: Juan Pablo Cañón Contreras
* Asignatura: Sistemas Operativos
* Tema: Concurrencia
* Objetivo: Hacer una metodología para la implementación 
* de la multiplicación de matrices. La idea principal,
* es construir paso a paso la implementación para hacer uso
* de la biblioteca PTHREAD. Se implementa el Algoritmo Clásico
* de multiplicación de matrices, para matrices cuadradas, es decir,
* la dimensión de filas es igual a las columnas.
* A continuación se describen los pasos:
 * 	- Reserva de memoria
 * 	- Creación de punteros para matrices del tipo doble
 * 	- Asignación de memoria
 * 	- Ingreso de argumentos de entrada (Dimensión matriz, número de hilos)
 *	- Validación argumentos de entrada
 *	- Inicializar las matrices
 *  - Imprimir las matrices 
 *  - Función para inicializar las matrices 
 *  - Función para imprimir las matrices 
 *  - Algoritmo clásico de multiplicación matrices 
 *  - Se verifica el resultado
 *  - Función para multiplicación las matrices 
 *  - Declaración vector de hilos 
 *  - Creación de hilos según tamaño de vector de hilos 
 *  - Crear estructura de datos que encapsule los argumentos de entrada de la función MM
*
*******************************************************/


//Incluimos las bibliotecas con las que vamos a trabajar
#include <stdio.h>  //Entrada de datos y salida
#include <stdlib.h> //Memoria dinámica y otras funciones generales
#include <sys/time.h> //Manejo de tiempo
#include <pthread.h>  //Manejo de hilos

//Reservamos un espacio de memoria suficiente
#define RESERVA (1024*128*64*8)

//Declaramos de manera global el pedazo de memoria del tamaño de RESERVA
static double MEM_CHUNK[RESERVA];
//Es estático porque el uso de apuntadores requiere que este espacio de memoria sea fijo

//Creamos una estructura de datos
struct datosMM{
  int N;  //Cantidad de filas y columnas
  int H;  //Cantidad de hilos
  double *mA;  //Apuntador a la matriz A
  double *mB;  //Apuntador a la matriz B
  double *mC;  //Apuntador a la matriz C
};

//Función Inicializar Matrices
void initMatrices(int n, double *m1, double *m2, double *m3){
  //Si n es menor que 9 imprima
  if(n<9){
  //Llenamos los valores de la matriz de manera lineal con valores
    for(int i=0; i<n*n; i++){
      m1[i] = i*1.1;  //Llena la matriz A
      m2[i] = i*2.2;  //Llena la matriz B
      m3[i] = i;      //Llena la matriz C
    }
    printf("\n####################\n");  //Decoración
  }else{
    printf("\n####################\n");  //Decoración
  }
}

//Función imprimir matrices
void imprMatrices(int n, double *matriz){
  //Imprimimos los valores de la matriz
  for(int i=0; i<n*n; i++){
    if(i%n == 0) printf("\n"); //Hace un salto de linea
    printf(" %f ", matriz[i]);  //Imprime el contenido
  }
  printf("\n########################\n");
}

void *multiMatrices(int n, double *m1, double *m2, double *m3){
  //Recorremos filas
  for(int i=0; i<n; i++){
    //Recorremos columnas
    for(int j=0; j<n; j++){
      //Declaramos las variables para contener y desplazar
      double sumaTemp, *pA, *pB;
      sumaTemp = 0.0;
      //Realizamos las operaciones y aumentamos
      pA = m1 + i*n;
      pB = m2 + j;
      //Sumamos
      for(int k=0; k<n; k++, pA++, pB+=n){
        sumaTemp += *pA * *pB;
      }
      //Almacenamos en la matriz el resultado
      m3[j+i*n] = sumaTemp;
    }
  }
  return 0;
};


//Inicio de nuestro programa principal
int main(int argc, char *argv[]) {

  //Validamos si recibe el número correcto de argumentos
  if(argc<=2){
    //En caso de no ser correcto mostramos en pantalla
    printf("Número de argumentos incorrectos\n");
    //Imprime un ejemplo de como debe ser
    printf("\n\t $ejetucable.exe DIM NumHilos\n");
    return -1;
  }
  
  //Declaramos las variables N y H para que reciban los argumentos
  // y hacemos que se puedan convertir los tipos de ASCII a entero
  //Tanaño de la matriz
  int N = (int) atof(argv[1]);  //Recibe el primer argumento
  //Número de hilos con el que vamos a trabajar
  int H = (int) atof(argv[2]);  //Recibe el segundo argumento

  //Validación para revisar que los valores sean mayores a cero
  if(N<=0 || H <=0){
      //Si es menor que cero imprime este aviso y finaliza el programa
      printf("\n Valores deben ser mayor que cero\n");
    return -1;	
  };


  //Declaramos el vector con la cantidad de hilos
  pthread_t hilos[H];  //H es asignado por el usuario al momento de ingresarlo por consola
  
  //Declaramos los punteros de tipo doble para su matriz respectiva
  double *mA, *mB, *mC;
  
  //Definimos que mA va a apuntar al inicio del MEM_CHUNK
  mA = MEM_CHUNK;  //Reserva memoria para matriz A
  mB = mA + N*N;   //Reserva memoria para matriz B
  mC = mB + N*N;   //Reserva memoria para matriz C (Resultado)

  //Hacemos el llamado de la función para llenar la matriz
  initMatrices(N, mA, mB, mC);

  //Hacemos el llamado de la función para imprimir la matriz y enviamos las variables
  imprMatrices(N, mA);
  imprMatrices(N, mB);

  //Hacemos el llamado de la función para multiplicar las matrices
  multiMatrices(N, mA, mB, mC);

  //Hacemos el llamado para imprimir la matriz resultante
  imprMatrices(N, mC);

  //Imprimimos por pantalla indicando que el programa ha finalizado
  printf("\nFin del programa...\n");
  
  return 0;
}