# Variables para los nombres de los ejecutables y archivos fuente
GCC = gcc
MONITOR_C = monitor.c
SENSOR_C = sensor.c
MONITOR = monitor
SENSOR = sensor

# Compilar monitor
$(MONITOR): $(MONITOR_C)
		$(GCC) $(MONITOR_C) -o $(MONITOR)

# Compilar sensor
$(SENSOR): $(SENSOR_C)
		$(GCC) $(SENSOR_C) -o $(SENSOR)

# Ejecutar monitor y sensor de temperatura
run_temp: $(MONITOR) $(SENSOR)
		./$(MONITOR) -b 100 -t file-temp.txt -h file-ph.txt -p pipe_nominal & \
		sleep 1 && ./$(SENSOR) -s 1 -t 1 -f temperatura.txt -p pipe_nominal

# Ejecutar monitor y sensor de pH
run_ph: $(MONITOR) $(SENSOR)
		./$(MONITOR) -b 100 -t file-temp.txt -h file-ph.txt -p pipe_nominal & \
		sleep 1 && ./$(SENSOR) -s 2 -t 1 -f phdata.txt -p pipe_nominal

# Ejecutar solo el monitor
run_monitor: $(MONITOR)
		./$(MONITOR) -b 100 -t file-temp.txt -h file-ph.txt -p pipe_nominal

# Ejecutar solo el sensor de temperatura
run_sensor_temp: $(SENSOR)
		./$(SENSOR) -s 1 -t 1 -f temperatura.txt -p pipe_nominal

# Ejecutar solo el sensor de pH
run_sensor_ph: $(SENSOR)
		./$(SENSOR) -s 2 -t 1 -f phdata.txt -p pipe_nominal

# Limpiar archivos generados
clean:
		rm -f $(MONITOR) $(SENSOR) file-temp.txt file-ph.txt pipe_nominal

# Reglas para compilar todo y ejecutar casos específicos
all: $(MONITOR) $(SENSOR)


# Siendo esto así los posibles comandos son:
# make all
# make run_temp
# make run_ph
# make run_monitor
# make run_sensor_temp
# make run_sensor_ph
