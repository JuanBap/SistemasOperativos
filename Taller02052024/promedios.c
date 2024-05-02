/********************
* Autores: Juan Pablo Cañón Contreras y Harry Esteban Sánchez Guevara
* Fecha: 02/05/2024
* Sistemas Operativos
* Tema: Taller en clase lectura de archivo y calculo de promedio:
* Pasos:
*  - Abrimos el archivo, si no es posible la apertura del archivo manda un mensaje de error
*  - Inicializamos las variables, para contar el promedio, la cantidad y la lectura
*  - Mediante un ciclo while (mientras hayan datos para leer, almacenamos los datos)
*  - Se cierra el archivo
*  - Hacemos el llamado de la función
*  - Imprimimos los resultados de los promedios
**********************/

// se incluyen las librerias
#include <stdio.h>
#include <stdlib.h>
//Incluimos nuestra interfaz
#include "funciones.h" 

//Colores de consola
const char *GREEN = "\033[32m";
const char *RESET = "\033[0m";
const char *BLUE = "\033[34m";
const char *RED = "\033[31m";


//Inicio del programa principal que lee los datos del archivo
int main(int arg, char *argv[]) {
  
  //Ruta fija del archivo recibida por linea de comando
  const char *file_path = argv[1];

  //Imprimimos el archivo
  printf("%s\nNombre del archivo:%s %s\n\n",GREEN,RESET,file_path);
  
  //El primer argumento de la función fopen() recibe el nombre del archivo
  //El segundo argumento de la función fopen() recibe si va a leer o escribir
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
      printf("%sAlerta: No se pudo abrir el archivo.%s\n\n",RED, RESET);
      return 1;
  }
  
    //Variables para almacenar la suma de las dos columnas
    int sum1 = 0, sum2 = 0;  
  
    //Variables para almacenar la cantidad de números en cada columna
    int count1 = 0, count2 = 0;  
    
    //Variables temporales para leer números del archivo
    int num1, num2;  

    //Leemos datos del archivo y procesarlos
    //Leemos dos números por línea
    while (fscanf(file, "%d %d", &num1, &num2) == 2) { 
        //Sumamos a la suma de la primera columna
        sum1 += num1; 
      
        //Sumamos a la suma de la segunda columna
        sum2 += num2;
      
      //Incrementamos el contador de la primera columna
        count1++;  
      
      // Incrementar el contador de la segunda columna
        count2++;  
    }

    //Cerramos el archivo
    fclose(file);  

    //Calculamos y mostrar los promedios
    double promedio1 = calcular_promedio(sum1, count1); 
    double promedio2 = calcular_promedio(sum2, count2); 

    //Mostrar el promedio de cada columna
    printf("%sPromedio de la primera columna:%s %.2f\n", BLUE, RESET,promedio1); 
    printf("%sPromedio de la segunda columna:%s %.2f\n\n", BLUE, RESET,promedio2); 

    //Imprimimos los autores
    printf("%sHecho por: Juan Pablo Cañón y Harry Esteban Sánchez\n\n",RED);
  
    //Finalizamos el programa
    return 0;
}

