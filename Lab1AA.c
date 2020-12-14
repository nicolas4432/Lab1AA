#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc

typedef struct
{
	int matriz[2];				//matriz[0] es X "ancho"  matriz[1] es Y "alto"
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

int PosicionValida(int** Matriz, int CoordenadasX, int CoordenadasY, DatosArchivo Datos) {

	int x = CoordenadasX;
	int y = CoordenadasY;
	
	//Comprobador Vertical
	for (int i = 0; i < Datos.matriz[0]; i++)
	{
		if (Matriz[i][CoordenadasY] != 0)
		{
			return 0;
		}

	}

	//Comprovador Horizonatal
	for (int i = 0; i < Datos.matriz[1]; i++)
	{
		if (Matriz[CoordenadasX][i] != 0)
		{
			return 0;
		}

	}

	//Comprovador vertical derecha abajo
	while (x < Datos.matriz[0] && y < Datos.matriz[1])
	{
		if (Matriz[x][y] != 0)
		{
			return 0;
		}
		x++;
		y++;
	}

	x = CoordenadasX;
	y = CoordenadasY;
	//Comprovador vertical izquierda arriba
	while (x > -1 && y > -1)
	{
		if (Matriz[x][y] != 0)
		{
			return 0;
		}
		x--;
		y--;
	}

	x = CoordenadasX;
	y = CoordenadasY;
	//Comprovador vertical izquierda abajo
	while (x < Datos.matriz[0] && y > -1)
	{
		if (Matriz[x][y] != 0)
		{
			return 0;
		}
		x++;
		y--;
	}

	x = CoordenadasX;
	y = CoordenadasY;
	//Comprovador vertical derecha arriba
	while (x > -1 && y < Datos.matriz[1])
	{
		if (Matriz[x][y] != 0)
		{
			return 0;
		}
		x--;
		y++;
	}

	return 1;
}

void main() {
	
	DatosArchivo datosIniciales = LeerArchivo("Entrada3.in");
	//SE CREA LA MATRIZ
	int** matriz = (int**)malloc(datosIniciales.matriz[0] * sizeof(int*));

	for (int i = 0; i < datosIniciales.matriz[0]; i++)
	{
		matriz[i] = (int*)malloc(datosIniciales.matriz[1] * sizeof(int));
		for (int j = 0; j < datosIniciales.matriz[1]; j++)
		{
			matriz[i][j] = 0;
		}
	}
	

	//PRUEBA DE FUNCIONAMIENTO
	matriz[1][0] = -1;
	for (int i = 0; i < datosIniciales.matriz[0]; i++)
	{
		
		for (int j = 0; j < datosIniciales.matriz[1]; j++) 
		{
			if (PosicionValida(matriz, i, j, datosIniciales) == 1)
			{
				matriz[i][j] = -1;
			}

			printf("%d ", matriz[i][j]);
		}
		printf("\n");

	}

}

//PARA IMPRIMIR LOS DATOS INICIALES
//for (int i = 0; i < datosIniciales.puntosObligatorios * 2; i += 2)
//{
//	printf("%d %d\n", datosIniciales.arreglo[i], datosIniciales.arreglo[i + 1]);
//}

//PARA IMRPRIMIR CONTIDO DE LA MATRIZ
//for (int i = 0; i < datosIniciales.matriz[0]; i++)
//{
//	for (int j = 0; j < datosIniciales.matriz[1]; j++) {
//		printf("%d ", matriz[i][j]);
//	}
//	printf("\n");
//}