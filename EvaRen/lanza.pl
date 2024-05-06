#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************/

$Path = `pwd`;  # Ejecuta el comando 'pwd' que imprime el directorio de trabajo actual y lo guarda en la variable $Path.
chomp($Path);   # Elimina cualquier carácter de nueva línea al final de la cadena en $Path.

$Nombre_Ejecutable = "MM_ejecutable";  # Asigna el nombre del ejecutable a la variable $Nombre_Ejecutable.
@Size_Matriz = ("200","300");  # Define un array con dos tamaños de matriz: 200 y 300.
@Num_Hilos = (1,2);  # Define un array con dos configuraciones de número de hilos: 1 y 2.
$Repeticiones = 2;  # Establece el número de repeticiones de cada ejecución a 2.

foreach $size (@Size_Matriz){  # Itera sobre cada tamaño de matriz en el array @Size_Matriz.
	foreach $hilo (@Num_Hilos) {  # Anida un bucle para iterar sobre cada configuración de hilos en el array @Num_Hilos.
		$file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";  # Construye el nombre del archivo para guardar los resultados de la ejecución.
		for ($i=0; $i<$Repeticiones; $i++) {  # Bucle para repetir la ejecución la cantidad de veces especificada en $Repeticiones.
#system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");  # Línea comentada que ejecutaría el comando para correr el ejecutable y redirigir la salida a $file.
			printf("$Path/$Nombre_Ejecutable $size $hilo \n");  # Imprime el comando que se ejecutaría, útil para depuración.
		}
		close($file);  # Cierra el archivo después de escribir todas las salidas (aunque en este contexto no es necesario y no tiene efecto).
	$p=$p+1;  # Incrementa una variable $p (no definida previamente) por cada combinación de tamaño de matriz y número de hilos.
	}
}


