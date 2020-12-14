#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc

typedef struct
{
	int matriz[2];
	int puntosObligatorios;
	int* arreglo;
} DatosArchivo;


DatosArchivo LeerArchivo(char* NombreArchivo) {
	
	DatosArchivo datos;

	FILE* Archivo = fopen(NombreArchivo, "r");

	if (Archivo == NULL)
	{
		printf("Archivo no Existe\n");
		datos.matriz[0] = -1;
		datos.matriz[1] = -1;
		datos.puntosObligatorios = -1;
		datos.arreglo = NULL;
		return datos;
	}

	fscanf(Archivo, "%d %d\n", &datos.matriz[0], &datos.matriz[1]);
	fscanf(Archivo, "%d\n", &datos.puntosObligatorios);

	datos.arreglo = (int*)malloc(sizeof(int) * datos.puntosObligatorios);

	if (datos.puntosObligatorios != 0)
	{
		int contador = 0;

		while (feof(Archivo) == 0)
		{
			fscanf(Archivo, "%d %d\n", &datos.arreglo[contador], &datos.arreglo[contador + 1]);
			contador = contador + 2;
		}
	}

	fclose(Archivo);

	return datos;
}

void main() {
	
	DatosArchivo datosIniciales = LeerArchivo("Entrada3.in");

}

//PARA IMPRIMIR LOS DATOS INICIALES
//for (int i = 0; i < datosIniciales.puntosObligatorios * 2; i += 2)
//{
//	printf("%d %d\n", datosIniciales.arreglo[i], datosIniciales.arreglo[i + 1]);
//}