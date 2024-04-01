/*
 *  Pontificia Universidad Javeriana
 *
 *  Autor: Juan Pablo Cañón Contreras
 *  Fecha: 2 de marzo del 2024
 *  Materia: Sistemas Operativos
 *  Tema: Bonus ejercicios punteros semana 6 de clase
 *
 */

//Incluimos la biblioteca con herramientas estándar
#include <stdio.h>
#include <stdlib.h>

//Inicio del programa principal
int main (int argc, char *argv[]){

  //Diapositivas - Punteros
  //Primer ejercicio
  int variable = 123;   //Declara variable y asigna contenido
  int *ptr = &variable; //Declara apuntador y asigna variable para apuntar 
  printf("Primer ejercicio\n\n");
  printf("Valor de la variable: %d \n",variable);
  printf("Dirección de memoria de la variable: %p\n",&variable);
  printf("Dirección a la que apunta el puntero: %p\n",(void*)ptr);
  printf("Valor de la variable mediante puntero: %d\n\n", *ptr);

  //Segundo ejercicio
  int *vectorPunteros[3];     // Declara un vector de punteros a int
  int p = 10, q = 20, r = 30; // Declara variables y asigna valores

  printf("Segundo ejercicio\n\n");
  vectorPunteros[0] = &p;  // La posición 0 del vector apunta a la dirección de memoria de p
  vectorPunteros[1] = &q;  // La posición 1 del vector apunta a la dirección de memoria de q
  vectorPunteros[2] = &r;  // La posición 2 del vector apunta a la dirección de memoria de r
  for(int i = 0; i < 3; i++) {                          //Aquí había un error, no tenía el (void*)
    printf("Para la dirección: %p \t el valor = %d\n", (void*)vectorPunteros[i], *vectorPunteros[i]);
  }
  printf(" \n\n");


  //Tercer ejercicio
  int valor1, valor2; //Declara variables de tipo entero
  int *ptr1, *ptr2;   //Declara punteros de tipo enter

  ptr1 = &valor1;     //Apunta a la dirección de memoria del valor 1
  ptr2 = &valor2;     //Apunta a la dirección de memoria del valor 2

  printf("Tercer ejercicio\n\n");
  printf("Ingrese los 2 numeros que desea sumar: ");  //Pide los dos numeros a sumar
  scanf("%d %d",&valor1, &valor2);                    //Guarda los dos numeros en sus respectivas variables
  printf("\nLa suma es de = %d\n\n",*ptr1+*ptr2);     //Accede al contenido de los apuntadores y los suma


  //Cuarto ejercicio
  int vector[] = {30,60,90,120};  //Vector con 4 casillas 
  int *ptrvec;                    //Declaramos apuntador
  
  ptrvec = vector;                //Puntero apunta a la primera posición del vector

  printf("Cuarto ejercicio\n\n");
  printf("Impresion por indices del vector\n\n");
  for(int i = 0; i<4; i++){
    printf("Por indice  vector[%d]: %d\n", i, vector[i]); //Imprime haciendo uso del indice
  }

  printf("\nImpresion por apuntador\n");
  for(int i = 0; i<4; i++){
    printf("Por apuntador vector[%d]: %d\n", i, *ptrvec); //Imprime haciendo uso del apuntador
    ptrvec++;       //Aumentamos el puntero para que apunte a la siguiente posición
  }
  printf(" \n");

  //Quinto ejercicio
  printf("Quinto ejercicio\n\n");

  int varX = 3108;
  int *apuntador1 = &varX;
  int **apuntador2= &apuntador1;

  printf("Valor de varX = \t%d \n", varX);
  printf("Valor de apuntador1 = \t%d \n", *apuntador1);
  printf("Valor de apuntador2 = \t%d \n", **apuntador2);

  printf(" \n");

  printf("Dirección de memoria de varX = \t%p\n",&varX);

  printf("Dirección de memoria de puntero1 = \t%p\n",&apuntador1);
  printf("Dir. de mem. de contenido puntero1 = \t%p\n",apuntador2);

  printf("Dirección de memoria de varX = \t%p\n",&apuntador2);
  printf("Dirección de memoria de varX = \t%p\n",apuntador2);

  //Sexto ejercicio
  int rows, cols, i, j;
  int **matrix;

  rows = 3;
  cols = 3;

  //Asignación de memoria para la matriz
  matrix = (int**)malloc(rows*sizeof(int*));
  for(int i = 0; i<rows; i++){
    matrix[i] = (int*)malloc(cols*sizeof(int*));
  }

  //Llenado de la matriz con valores iniciales
  for(int i = 0; i < rows; i++){
    for(int j= 0; j < cols; j++){
      matrix[i][j] = i * j;
    }
  }

  //Impresión de la matriz
  printf("\nSexto ejercicio\n\n");
  for(int i = 0; i < rows; i++){
    printf("| ");
    for(int j= 0; j < cols; j++){
      printf("%d | ",matrix[i][j]);
    }
    printf("\n");
  } 

  //Liberación de la memoria al Sistema operativo
  for(int i = 0; i < rows; i++){
    free(matrix[i]);
  }
  free(matrix);

  //Septimo ejercicio
  

  return 0;
}//niam
