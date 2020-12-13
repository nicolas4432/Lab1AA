#include <stdio.h>
#include <string.h>



void main(){
	//Variables a usar
	char	tamanio_char[10],										//Lee la primera linea con el tamaño de matriz
		caracter;												//Iterador de caracter leido
	int		posCaracter = 0;										//Iterador para la posición de la matriz a guardar

	int numero;
	
	int Tamanio = 1;

	int* Arreglo = malloc(sizeof(int) * Tamanio);

	FILE* sopaPtr = fopen("Entrada1.in", "r");							//Abrimos el archivo sopa.in con un Puntero FILE*

	//if (sopaPtr == NULL) {											//El archivo se pudo leer?
	//	sopa.SOPA = NULL;
	//	sopa.tam = 0;
	//	return sopa;
	//}

	//Obtenemos el tamaño de la sopa leyendo la primera fila



	while (feof(sopaPtr) == 0)
	{
		/*fgets(tamanio_char, 10, sopaPtr);*/
		fscanf(sopaPtr, "%d\n", &Arreglo[Tamanio - 1]);

		Tamanio++;
	}

	printf("%d", strlen(Arreglo));

}