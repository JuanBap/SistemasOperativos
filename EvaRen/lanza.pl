#**************************************************************
#               Pontificia Universidad Javeriana
#     Autor: J. Corredor
#     Fecha: Febrero 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#****************************************************************

#**************************************************************
#               Pontificia Universidad Javeriana
#     Autor: Juan Pablo Cañón Contreras
#     Fecha: Mayo 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#	  Objetivo: Documentar el código y modificar los valores
#				para la experimentación y salida de datos
#****************************************************************

# Obtiene el directorio de trabajo actual y lo asigna a $Path
my $Path = `pwd`;

# Elimina cualquier salto de línea al final de la salida del comando 'pwd'
chomp($Path);  

# Define el nombre del ejecutable que se usará en las pruebas
my $Nombre_Ejecutable = "Fuente_Evaluacion";

# Define los tamaños de matrices para las pruebas en un arreglo
my @Size_Matriz = ("200", "300");

# Define los números de hilos que se usarán en las pruebas en otro arreglo
my @Num_Hilos = (1, 2);

# Establece el número de repeticiones que se realizarán para cada configuración
my $Repeticiones = 10;

# Ciclo anidado que itera sobre cada tamaño de matriz y número de hilos
foreach my $size (@Size_Matriz) {
    foreach my $hilo (@Num_Hilos) {

		# Asigna el nombre concatenando la ruta, nombre del ejecutable, tamaño e hilos
        my $file = "$Path/$Nombre_Ejecutable-$size-Hilos-$hilo.dat";

		# Abrir el archivo y mostrar mensaje de error en caso de que no se pueda
        open(my $fh, '>', $file) or die "No se pudo abrir el archivo '$file' $!";
        
		# Ciclo que itera por la cantidad de repeticiones
        for (my $i = 0; $i < $Repeticiones; $i++) {
            # Ejecutar linea de comando y sino se puede decir mensaje de error
            open(my $cmd, "-|", "$Path/$Nombre_Ejecutable $size $hilo") or die "No se pudo ejecutar el comando: $!";
            while (my $line = <$cmd>) {
                print $fh $line;  # Imprime la linea en el archivo
            }
            close $cmd; #Finaliza el uso de la consola
        }
        
        close $fh; #Cierra el archivo y finaliza el script
    }
}

