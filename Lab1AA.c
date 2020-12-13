#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc



void main() {
	
	int matriz[2];
	int puntosObligatorios;

	FILE* sopaPtr = fopen("Entrada1.in", "r");							//Abrimos el archivo sopa.in con un Puntero FILE*

	for (int i = 0; i < 2; i++)
	{
		fscanf(sopaPtr, "%d\n", &matriz[i]);
	}

	fscanf(sopaPtr, "%d\n", &puntosObligatorios);

	int* arreglo = (int*)malloc(sizeof(int) * puntosObligatorios);

	if (puntosObligatorios != 0)
	{
		int contador = 0;

		while (feof(sopaPtr) == 0)
		{
			fscanf(sopaPtr, "%d\n", &arreglo[contador]);
			contador++;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		printf("%d", arreglo[i]);
	}

}