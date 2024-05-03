#ifndef FUNCIONES_MMC_H
#define FUNCIONES_MMC_H

#include <sys/time.h>

#define DATA_SIZE (1024*1024*64*3)

struct parametros {
	int nH;
	int idH;
	int N;
};

extern double *mA, *mB, *mC;

void llenar_matriz(int SZ);
void print_matrix(int sz, double *matriz);
void inicial_tiempo();
void final_tiempo();
void *mult_thread(void *variables);

#endif
