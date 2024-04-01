/*
 *  Pontificia Universidad Javeriana
 *
 *  Autor: Juan Pablo Cañón Contreras
 *  Fecha: 2 de marzo del 2024
 *  Materia: Sistemas Operativos
 *  Tema: Bonus ejercicios semana 6 de clase
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *malloc(size_t size);
void exit(int status);
void free(void *ptr);
char *function(char *p) {
    // Tamaño de la cadena 'p' más 1 para el carácter nulo
    size_t len = strlen(p) + 1;
    // Asigna memoria dinámica para la cadena 'q'
    char *f = malloc(len);
    // Verifica si la asignación de memoria fue exitosa
    if (f != NULL) {
        // Copia la cadena 'p' en 'q'
        strcpy(f, p);
        printf("Desde f: el string es: %s\n", f);
    }
    // Retorna 'q'
    return f;
    }

int main() {
    //Primer ejercicio
    char *p = NULL; // Inicializa p como un puntero nulo
    char *q = NULL; // Inicializa q como un puntero nulo
    printf("Primer ejercicio\n\n");
    printf("Dirección de p = %p\n", (void *)p); // Imprime dirección de p
    p = "Hola, soy el mejor en sistemas operativos!!!"; // Asigna la cadena a p
    printf("%s\n", p); // Imprime el contenido de p
    printf("A punto de copiar \"Adios\" a q\n");
    q = "Adios"; // Asigna la cadena a q
    printf("String copiado\n");
    printf("%s\n", q); // Imprime el contenido de q

    //Segundo ejercicio
    char *l = NULL;

    printf("\n\nSegundo ejercicio\n\n");

    printf("Pidiendo espacio para \"Adios\" \n");

    // Asignación de memoria dinámica para la cadena "Adios"
    l = (char *)malloc(strlen("Adios") + 1);

    // Verificar si la asignación de memoria fue exitosa
    if (!l) {
        perror("Fallo la asignacion de espacio porque: ");
        exit(1); // Salir del programa con código de error 1 si la asignación de memoria falla
    }

    // Copiar la cadena "Adios" en la memoria asignada
    printf("Copiando \"Adios\" a q en la direccion: %p\n", (void *)l);
    strcpy(l, "Adios");

    // Imprimir que la cadena ha sido copiada exitosamente
    printf("String copiado\n");
    printf("%p\n", (void *)l); // Imprimir la dirección de memoria asignada para la cadena

    free(l); // Liberar la memoria asignada a l
    
    //Tercer ejercicio
    printf("\nTercer ejercicio\n\n");
    int *ptr;
    ptr = malloc(15*sizeof(*ptr)); //Reserva 15 espacios de memoria

    if(ptr != NULL){                //Verifica los espacios de memoria
        *(ptr + 5) = 480;           //Asignar 480 al sexto entero
    }
    printf("Numero asignado: %d",*(ptr+5));
    //Este ejercicio asigna memoria para un bloque de 15 enteros y asigna el valor de 480 al sexto
    
    //Cuarto ejercicio
    printf("\n\nCuarto ejercicio\n\n");

    int num_elements, index, *puntero, total_sum = 0;  //Declaramos variables
   
    printf("Ingrese el número de elementos: ");        //Pedimos datos
    scanf("%d", &num_elements);                        //Ingresamos datos
   
    puntero = (int*) calloc(num_elements, sizeof(int)); //Asigna memoria para 'num_elements' enteros
   
    if(puntero == NULL) {
        printf("Error! memoria no asignada.");
        exit(0); // Sale del programa si la asignación de memoria falla
    }
   
    printf("Ingrese los elementos: ");
    for(index = 0; index < num_elements; ++index) { //Recorre para sumar los datos
        scanf("%d", puntero + index);               //Lee los elementos e incrementa 'total_sum' en el valor actual
        total_sum += *(puntero + index);
    }
    printf("Suma = %d", total_sum);                 //Imprime la suma de los elementos
    free(puntero);                                  //Libera la memoria asignada
    
    //Quinto ejercicio
    printf("\n\nQuinto ejercicio\n\n");
    int *ptr2, index2, size1, size2; // Declaración de variables

    printf("Ingrese el tamaño inicial: "); // Solicita al usuario ingresar el tamaño inicial
    scanf("%d", &size1); // Lee el tamaño inicial desde la entrada estándar
    
    ptr2 = (int*) malloc(size1 * sizeof(int)); // Asigna memoria para 'size1' enteros
    
    printf("Direcciones de la memoria previamente asignada: "); // Imprime un mensaje indicando las direcciones de memoria asignadas previamente
    
    for(index2 = 0; index2 < size1; ++index2) // Bucle para imprimir las direcciones de memoria previamente asignadas
        printf("\n\np = %p\n", ptr2 + index2); // Imprime la dirección de memoria de cada elemento en 'ptr2'
    
    printf("\nIngrese el nuevo tamaño: \n"); // Solicita al usuario ingresar el nuevo tamaño
    scanf("%d", &size2); // Lee el nuevo tamaño desde la entrada estándar
    
    // Reasignar la memoria
    ptr2 = realloc(ptr2, size2 * sizeof(int)); // Reasigna memoria para 'size2' enteros
    
    printf("Direcciones de la nueva memoria asignada: "); // Imprime un mensaje indicando las direcciones de memoria asignadas nuevamente
    
    for(index2 = 0; index2 < size2; ++index2) // Bucle para imprimir las direcciones de memoria asignadas nuevamente
        printf("\n\np = %p\n", ptr2 + index2); // Imprime la dirección de memoria de cada elemento en 'ptr2' después de la realocación
    
    free(ptr2); // Libera la memoria asignada

    //Sexto ejercicio
    char *a = NULL;
    char *b = NULL;
    
    printf("\n\nSexto ejercicio\n\n");

    //llama a la función con la cadena y asigna el resultado 
    a = function("Hola, aprender es divertido");
    // Llama a la función con la cadena "systems engineer" y asigna el resultado a 'b'
    b = function("Ingenieria de sistmas");
    
    printf("Desde el main: %s %s\n", a, b);
    
    // Liberar la memoria asignada a 'a' y 'b'
    free(a);
    free(b);

    return 0;
}
