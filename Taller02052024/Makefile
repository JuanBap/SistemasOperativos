# Variables
CC = gcc  
TARGET = promedios  

all: $(TARGET)

# Generar el ejecutable
$(TARGET): promedios.o funciones.o
	$(CC) -o $(TARGET) promedios.o funciones.o

# Compilar promedios.c
promedios.o: promedios.c funciones.h
	$(CC) -c promedios.c

# Compilar funciones.c
funciones.o: funciones.c funciones.h
	$(CC) -c funciones.c

# Limpiar los archivos objeto y el ejecutable
clean:
	rm -f $(TARGET) *.o

# Ejecutar el programa con el archivo de texto
run: all
	./promedios two_columns.txt
