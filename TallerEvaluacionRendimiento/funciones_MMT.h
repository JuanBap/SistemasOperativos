#ifndef FUNCIONES_MMT_H
#define FUNCIONES_MMT_H
// Las directivas #ifndef y #define juntas con #endif al final del archivo
// evitan la inclusión múltiple del mismo archivo de cabecera, lo que puede causar
// problemas de definiciones redundantes y otros errores de compilación.

#include <sys/time.h>
// Incluye las funciones y estructuras necesarias para manejar operaciones de tiempo,
// tales como gettimeofday(), útil para medir tiempos de ejecución en programas.

#define DATA_SIZE (1024*1024*64*3)
// Define una macro para especificar un tamaño de datos. Esta constante es útil para
// definir el tamaño de los bloques de memoria para las matrices, en este caso,
// un total de 192 MB, asumiendo un doble usa 8 bytes.

struct parametros {
	int nH;  // Número de hilos que se utilizarán en el procesamiento.
	int idH; // Identificador único para cada hilo, útil para controlar accesos y asignaciones específicas.
	int N;   // Dimensiones de las matrices involucradas, asumiendo matrices cuadradas N x N.
};
// Define una estructura llamada 'parametros' que encapsula los detalles necesarios
// para controlar la ejecución de hilos en operaciones de matrices.

extern double *mA, *mB, *mC;
// Declara externamente los punteros a las matrices A, B, y C, lo que implica que
// las matrices están definidas en otro lugar (otro archivo fuente), permitiendo su
// uso compartido entre diferentes módulos del programa.

void llenar_matriz(int SZ);
// Declara una función para llenar las matrices A y B con valores determinados
// y establecer todos los elementos de la matriz C en cero.

void print_matrix(int sz, double *matriz);
// Declara una función para imprimir matrices de tamaño 'sz'. Es útil para
// verificar resultados en matrices de pequeña escala.

void inicial_tiempo();
// Declara una función que captura el tiempo de inicio de una operación, utilizada
// para calcular la duración de ejecución de la multiplicación de matrices.

void final_tiempo();
// Declara una función que captura el tiempo de finalización de una operación y calcula
// el tiempo transcurrido desde el tiempo inicial capturado por `inicial_tiempo`.

void *mult_thread(void *variables);
// Declara una función que será ejecutada por cada hilo. Esta función realiza
// la multiplicación de matrices para la sección de la matriz asignada al hilo.
// La función toma un puntero a 'struct parametros' que contiene la configuración del hilo.

#endif
// Finaliza la directiva de preprocesador, asegurando que el contenido interno
// del archivo de cabecera solo se procese una vez.
