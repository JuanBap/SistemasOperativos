# Evaluación de Rendimiento de Algoritmos de Multiplicación de Matrices

## Descripción
Este proyecto evalúa el rendimiento de algoritmos de multiplicación de matrices en C++ en tres entornos de cómputo distintos: Workspaces Pro, Replit y CoCalc. Se comparan las implementaciones en serie y en paralelo para diferentes tamaños de matriz y configuraciones de hilos. Los algoritmos se implementaron utilizando la biblioteca `pthread` para la gestión de hilos, y se midió el tiempo de ejecución y el uso de memoria como métricas principales.

Para cada configuración, se ejecutaron 30 repeticiones para obtener medidas representativas, aplicando la ley de los grandes números. Los tamaños de matriz seleccionados varían desde matrices pequeñas (64x64) hasta grandes (2048x2048), y se probaron diferentes números de hilos según la capacidad de cada sistema. Los resultados se recopilaron y analizaron para determinar la eficiencia y escalabilidad de los algoritmos en cada entorno.

Este trabajo proporciona una comparación detallada del rendimiento de la multiplicación de matrices en serie y paralela, destacando las ventajas de la paralelización en sistemas con múltiples núcleos y la importancia de optimizar el uso de recursos en entornos de alto rendimiento. Las conclusiones ofrecen recomendaciones sobre la selección de algoritmos y configuraciones de hilos adecuadas para diferentes escenarios de cómputo.

## Requisitos
- GCC (GNU Compiler Collection)
- Biblioteca pthread
- Sistema operativo POSIX (Linux)

## Archivos
- `functions.h`: Archivo de cabecera con definiciones de funciones y variables.
- `functions.c`: Implementación de funciones auxiliares y de manejo de hilos.
- `mm_clasico.c`: Implementación del algoritmo de multiplicación de matrices clásico.
- `mm_transpuesta.c`: Implementación del algoritmo de multiplicación de matrices con transposición.
- `lanzador.pl`: Script Perl para automatizar la ejecución de experimentos.

## Compilación
Para compilar los algoritmos, ejecute los siguientes comandos en la terminal:


gcc functions.c -o functions -lpthread
gcc mm_clasico.c -o mm_clasico -lpthread
gcc mm_transpuesta.c -o mm_transpuesta -lpthread


## Ejecución
Para ejecutar los algoritmos con diferentes tamaños de matriz y número de hilos, utilice los siguientes comandos:

./mm_clasico <tamano_matriz> <num_hilos>
./mm_transpuesta <tamano_matriz> <num_hilos>

Por ejemplo, para ejecutar el algoritmo clásico con una matriz de tamaño 1024x1024 y 4 hilos:

./mm_clasico 1024 4

## Replicación del Experimento

Para replicar el experimento y obtener medidas representativas, siga estos pasos:

> Preparación del Entorno:

Asegúrese de tener GCC y la biblioteca pthread instalados en su sistema. Configure su entorno para compilar y ejecutar programas en C++.

## Compilación de Algoritmos:

Compile los archivos functions.c, mm_clasico.c, y mm_transpuesta.c utilizando los comandos mencionados en la sección de compilación.

## Ejecución Automatizada:

Utilice el script lanzador.pl para automatizar la ejecución de los algoritmos con diferentes configuraciones de tamaño de matriz y número de hilos. Ejecute el script de la siguiente manera:

./lanzador.pl


## Recolección de Datos:
El script Perl recopilará los tiempos de ejecución y los guardará en archivos .dat. Estos archivos pueden ser exportados a hojas de cálculo para análisis posterior.

## Análisis de Resultados:
Calcule el promedio de los tiempos de ejecución y la desviación estándar para cada configuración. Utilice herramientas de análisis de datos para comparar el rendimiento de los algoritmos en diferentes entornos y configuraciones.

## Entornos de Ejecución
Los experimentos se llevaron a cabo en tres diferentes entornos de cómputo:

> Workspaces Pro: Un entorno de desarrollo proporcionado por GitHub, optimizado para desarrollo y pruebas de software en contenedores.
> Replit: Una plataforma en línea que facilita el desarrollo y ejecución de código, especialmente útil para pruebas rápidas y despliegue.
> CoCalc: Un entorno de computación en la nube robusto y flexible, ideal para tareas de alta carga computacional y experimentación automatizada.

## Selección de Tamaños de Matriz y Hilos
Workspaces Pro (1, 2, y 4 hilos):
Tamaños de matriz: 128x128, 256x256, 512x512, 1024x1024, 2048x2048

Replit (1, 2, y 4 hilos):
Tamaños de matriz: 64x64, 128x128, 256x256, 512x512, 1024x1024

CoCalc (1, 2, 4, 8, y 16 hilos):
Tamaños de matriz: 128x128, 256x256, 512x512, 1024x1024, 2048x2048

## Métrica de Rendimiento Usada

Las métricas principales utilizadas para evaluar el rendimiento de los algoritmos son:

> Tiempo de Ejecución:

Por qué: Refleja directamente la eficiencia de los algoritmos en términos de velocidad de procesamiento.
Cómo: Se mide el tiempo total que toma el algoritmo para completar la multiplicación de las matrices para cada configuración de tamaño y número de hilos. Este tiempo se registra utilizando funciones de temporización de alta resolución en C++.
Cuándo y Dónde: Durante cada ejecución de los experimentos en los entornos Workspaces Pro, Replit, y CoCalc.
Uso de Memoria:

Por qué: Es importante evaluar cómo los algoritmos gestionan los recursos de memoria, especialmente en configuraciones de alto paralelismo.
Cómo: Se monitorea el uso de memoria durante la ejecución de los algoritmos utilizando herramientas del sistema como top y free.
Cuándo y Dónde: Durante cada ejecución de los experimentos en los entornos Workspaces Pro, Replit, y CoCalc.

# Contacto
Para cualquier pregunta o sugerencia, por favor contacte a:

Autor: Juan Pablo Cañón Contreras
Email: canonjp@javeriana.edu.co