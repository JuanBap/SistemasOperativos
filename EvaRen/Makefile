# **************************************************************
#               Pontificia Universidad Javeriana
#     Autor: Juan Pablo Cañón Contreras
#     Fecha: Mayo 2024
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: Makefile 
#     Objetivo: Facilitar la compilación y ejecución del código
#                
# ****************************************************************

# Variables
GCC = gcc
OBJ = functions.c
TARGETS = mm_clasico mm_transpuesta

# Compilación por defecto
all: $(TARGETS)

# Reglas específicas para cada ejecutable
mm_clasico: mm_clasico.c $(OBJ)
	$(GCC) $^ -o $@ $(CFLAGS)

mm_transpuesta: mm_transpuesta.c $(OBJ)
	$(GCC) $^ -o $@ $(CFLAGS)

# Comandos para correr cada ejecutable
# Ejemplo: make run_mm_clasico TAM=1024 HILOS=4
run_mm_clasico:
	./mm_clasico $(TAM) $(HILOS)

run_mm_transpuesta:
	./mm_transpuesta $(TAM) $(HILOS)

# Limpieza
clean:
	rm -f $(TARGETS)

