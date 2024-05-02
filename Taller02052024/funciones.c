/********************
* Autores: Juan Pablo Cañón Contreras y Harry Esteban Sánchez Guevara
* Fecha: 02/05/2024
* Sistemas Operativos
* Tema: Taller en clase lectura de archivo y calculo de promedio:
* Pasos:
*  - Incluimos la declaración de la interfaz
*  - Función de calcular promedio recibe la sumatoria y la cantidad de datos
*  - Retornamos el double del cálculo
**********************/


//Incluir la interfaz para la declaración de la función calcular_promedio
#include "funciones.h"  

//Función para calcular el promedio
double calcular_promedio(int suma, int cantidad) {
    //Evitar la división por cero
    if (cantidad == 0) return 0;  
  
    //Calcular y retornar el promedio
    return (double)suma / cantidad;  
}
