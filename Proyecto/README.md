# Sistema de Monitoreo de Sensores de PH y Temperatura

## Descripción
Este sistema simula la medición de dos parámetros importantes en una reserva de agua: PH y temperatura. Utiliza procesos e hilos para simular sensores que envían datos a un proceso monitor. Los datos son enviados a través de pipes nominales y los hilos se sincronizan mediante el uso de semáforos. Se realizó como proyecto final para la matería de Sistemas Operativos en la Pontificia Universidad Javeriana, periodo 2310.

## Requisitos
- GCC para compilar los programas.
- Sistema operativo UNIX o compatible con POSIX para soporte de pthreads y semáforos.

## Estructura de Archivos
- `monitor.c`: Código fuente del proceso monitor que recibe y procesa los datos.
- `sensor.c`: Código fuente que simula los sensores de medición de temperatura y PH.
- `Makefile`: Archivo para compilar y ejecutar fácilmente los programas.

## Compilación y Ejecución
Para compilar los programas, asegúrese de que GCC esté instalado y utilice el archivo `Makefile` proporcionado:

```bash
make all
```

Para ejecutar el sistema completo con simulaciones de sensores y el monitor:

```bash
make run_temp
make run_ph
```

O puede ejecutar componentes individuales con:

```bash
make run_monitor
make run_sensor_temp
make run_sensor_ph
```

## Funcionamiento
- El proceso `monitor` espera datos de los sensores a través de un pipe nominal.
- Los procesos `sensor` simulan la lectura de datos de temperatura o PH desde un archivo y los envían al monitor a través del pipe.
- Los datos fuera de los rangos especificados generan alertas en la consola.
- Se recomienda inicializar el monitor en una shell y posteriormente sensores al mismo tiempo en dos shell por separado.

## Limpieza
Para limpiar los ejecutables y otros archivos generados, use:

```bash
make clean
```

## Autores
- Juan Pablo Cañón Contreras
- Maria Paula Rodríguez Mancera
- Natalia Echeverry Salcedo
- Daniel Carvajal Gutiérrez

## Profesor
- PhD. John Corredor


## Agradecimientos
Agradecimientos a la Pontificia Universidad Javeriana y a los profesores del curso de Sistemas Operativos por su apoyo y orientación en este proyecto.