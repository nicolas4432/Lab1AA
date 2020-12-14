#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc

typedef struct
{
	int matriz[2];				//matriz[0] es X "ancho"  matriz[1] es Y "alto"
	int puntosObligatorios;
	int* arreglo;
} DatosArchivo;

typedef struct nodo
{
	struct nodo* siguiente;
	struct nodo* anterior;
	int posX;
	int posY;
} Lista;

DatosArchivo LeerArchivo(char* NombreArchivo);
int PosicionValida(int** Matriz, int CoordenadasX, int CoordenadasY, DatosArchivo Datos);
int** CrearMatriz(DatosArchivo Datos);
Lista* CrearLista(int CoordenadasX, int CoordenadasY);
Lista* AgregarNodo(Lista* Actual, int CoordenadasX, int CoordenadasY);
Lista* QuitarNodo(Lista* Actual);
Lista* CopiarLista(Lista* Actual);
int CompararListas(Lista* ResultadoNuevo, Lista* ResultadoViejo);

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

int** CrearMatriz(DatosArchivo Datos) {
	
	int** matriz = (int**)malloc(Datos.matriz[0] * sizeof(int*));

	for (int i = 0; i < Datos.matriz[0]; i++)
	{
		matriz[i] = (int*)malloc(Datos.matriz[1] * sizeof(int));
		for (int j = 0; j < Datos.matriz[1]; j++)
		{
			matriz[i][j] = 0;
		}
	}

	return matriz;
}

Lista* CrearLista(int CoordenadasX, int CoordenadasY) {

	Lista* lista = (Lista*)malloc(sizeof(Lista));
	lista->anterior = NULL;
	lista->siguiente = NULL;
	lista->posX = CoordenadasX;
	lista->posY = CoordenadasY;

	return lista;
}

Lista* AgregarNodo(Lista* Actual, int CoordenadasX, int CoordenadasY) {

	Lista* listaInicial = Actual;

	while (listaInicial->siguiente != NULL)
	{
		listaInicial = listaInicial->siguiente;
	}

	Lista* nodoNuevo = CrearLista(CoordenadasX, CoordenadasY);
	listaInicial->siguiente = (Lista*)malloc(sizeof(Lista));
	listaInicial->siguiente = nodoNuevo;
	nodoNuevo->anterior = (Lista*)malloc(sizeof(Lista));
	nodoNuevo->anterior = listaInicial;
	nodoNuevo->siguiente = NULL;
	nodoNuevo->posX = CoordenadasX;
	nodoNuevo->posY = CoordenadasY;

	return listaInicial;
}

Lista* QuitarNodo(Lista* Actual) {

	Lista* listaInicial = Actual;
	Lista* nodoFinal = (Lista*)malloc(sizeof(Lista));

	while (listaInicial->siguiente != NULL)
	{
		listaInicial = listaInicial->siguiente;
	}
	nodoFinal = listaInicial;
	listaInicial = listaInicial->anterior;
	free(nodoFinal);
	listaInicial->siguiente = NULL;
	
	while (listaInicial->anterior != NULL)
	{
		listaInicial = listaInicial->anterior;
	}

	return listaInicial;
}

Lista* CopiarLista(Lista* Actual) {

	Lista* listaInicial = Actual;

	while (listaInicial->anterior != NULL)
	{
		listaInicial = listaInicial->anterior;
	}

	Lista* nuevaLista = CrearLista(listaInicial->posX, listaInicial->posY);

	while (listaInicial != NULL)
	{
		listaInicial = listaInicial->siguiente;
		if (listaInicial != NULL)
		{
			AgregarNodo(nuevaLista, listaInicial->posX, listaInicial->posY);
		}
		//AgregarNodo(nuevaLista, listaInicial->posX, listaInicial->posY);
	}

	return nuevaLista;
}

int CompararListas(Lista* ResultadoNuevo, Lista* ResultadoViejo) {

	Lista* listaNueva = ResultadoNuevo;
	Lista* listaVieja = ResultadoViejo;

	int contadorNuevo = 0;
	int contadorViejo = 0;

	while (listaNueva->anterior != NULL)
	{
		listaNueva = listaNueva->anterior;
	}

	while (listaVieja->anterior != NULL)
	{
		listaVieja = listaVieja->anterior;
	}

	while (listaNueva->siguiente != NULL)
	{
		listaNueva = listaNueva->siguiente;
		contadorNuevo++;
	}

	while (listaVieja->siguiente != NULL)
	{
		listaVieja = listaVieja->siguiente;
		contadorViejo++;
	}
	
	if (contadorNuevo > contadorViejo)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void main() {
	
	DatosArchivo datosIniciales = LeerArchivo("Entrada3.in");

	int** matriz = CrearMatriz(datosIniciales);

	Lista* lista = CrearLista(0, 0);
	//AgregarNodo(lista, 1, 1);

	//lista = lista->siguiente;
	//PRUEBA DE FUNCIONAMIENTO
	//matriz[lista->posX][lista->posY] = -1;
	//lista = lista->anterior;
	//QuitarNodo(lista);

	//Lista* copia = CopiarLista(lista);
	//AgregarNodo(copia, 0, 1);

	//printf("%d\n", CompararListas(copia, lista));


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