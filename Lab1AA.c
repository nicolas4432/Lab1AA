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

int LargoLista(Lista* List) {
	Lista* listaCopia = List;
	int largo = 0;
	//Se vuelve al inicio de la lista, en caso de no estarlo
	while (listaCopia->anterior != NULL)
	{
		listaCopia = listaCopia->anterior;
	}
	//se cuentan las puntos recorridos
	while (listaCopia->siguiente != NULL)
	{
		listaCopia = listaCopia->siguiente;
		largo++;
	}
	return largo;

}

Lista* Baktraking(DatosArchivo Datos) {
	
	int** matriz = CrearMatriz(Datos);

	Lista* resultado = NULL;
	Lista* recorrido = NULL;

	//while (true)
	//{

	if (Datos.puntosObligatorios != 0)
	{
		for (int i = 0; i < Datos.puntosObligatorios; i++)
		{
			matriz[Datos.arreglo[i * 2]][Datos.arreglo[i * 2 + 1]] = 1;
		}
	}

	int x = 0;
	int y = 0;
	do
	{
		for (int i = x; i < Datos.matriz[0]; i++)
		{

			for (int j = y; j < Datos.matriz[1]; j++)
			{
				if (PosicionValida(matriz, i, j, Datos) == 1)
				{
					matriz[i][j] = -1;
					if (recorrido == NULL)
					{
						recorrido = CrearLista(i, j);
					}
					else
					{
						//Se evita el cado repetido al iniciar denuevo la busqueda de resultado
						if (recorrido->posX == -1 && recorrido->posY == -1)
						{
							recorrido->posX = i;
							recorrido->posY = j;
						}
						else {
							AgregarNodo(recorrido, i, j);
						}
						//AgregarNodo(recorrido, i, j);
					}
				}

				//printf("%d ", matriz[i][j]);
			}
			//printf("\n");
			y = 0;
		}

		for (int i = 0; i < Datos.matriz[0]; i++)
		{
			for (int j = 0; j < Datos.matriz[1]; j++) {
				//printf("%d ", matriz[i][j]);
			}
			//printf("\n");
		}
		//printf("\n");
		//printf("\n");

		if (recorrido->posX == -1 && recorrido->posY == -1)
		{
			break;
		}

		while (recorrido->siguiente != NULL)
		{
			recorrido = recorrido->siguiente;
		}

		matriz[recorrido->posX][recorrido->posY] = 0;


		//for (int i = 0; i < Datos.matriz[0]; i++)
		//{
		//	for (int j = 0; j < Datos.matriz[1]; j++) {
		//		printf("%d ", matriz[i][j]);
		//	}
		//	printf("\n");
		//}
		//printf("\n");
		//printf("\n");

		if (recorrido->posX < Datos.matriz[0] && recorrido->posY < Datos.matriz[1]) {
			//if (recorrido->posX == Datos.matriz[0] - 1)
			//{
			//	x = recorrido->posX ;
			//	y = recorrido->posY + 1;
			//}
			if (recorrido->posY == Datos.matriz[1] - 1 && recorrido->posX < Datos.matriz[0] - 1)
			{
				x = recorrido->posX + 1;
				y = 0;
			}
			else
			{
				x = recorrido->posX;
				y = recorrido->posY + 1;
			}


		}


		if (resultado == NULL)
		{
			resultado = (Lista*)malloc(sizeof(Lista));
			resultado = CopiarLista(recorrido);

		}

		else if (CompararListas(recorrido, resultado) == 1)
		{
			resultado = CopiarLista(recorrido);
		}


		if (LargoLista(recorrido) > 0)
		{
			recorrido = QuitarNodo(recorrido);
		}
		else {

			if (recorrido->posX == Datos.matriz[0] - 1 && recorrido->posY == Datos.matriz[1] - 1)
			{
				recorrido = NULL;
			}
			else if (recorrido->posX < Datos.matriz[0] && recorrido->posY < Datos.matriz[1]) {

				recorrido->posX = -1;
				recorrido->posY = -1;



			}
		}

	} while (recorrido != NULL);

	return resultado;
}

void main() {
	
	DatosArchivo datosIniciales = LeerArchivo("Entrada1.in");


	Lista* resultado = Baktraking(datosIniciales);

	int** matriz = CrearMatriz(datosIniciales);



	if (datosIniciales.puntosObligatorios != 0)
	{
		for (int i = 0; i < datosIniciales.puntosObligatorios; i++)
		{
			matriz[datosIniciales.arreglo[i * 2]][datosIniciales.arreglo[i * 2 + 1]] = 1;
		}
	}



	for (int i = 0; i < datosIniciales.matriz[0]; i++)
	{
		for (int j = 0; j < datosIniciales.matriz[1]; j++) {
			if (matriz[i][j] == -1)
			{
				matriz[i][j] = 0;
			}
		}
	}
	
	while (resultado != NULL)
	{
		matriz[resultado->posX][resultado->posY] = -1;
		resultado = resultado->siguiente;
	}

	for (int i = 0; i < datosIniciales.matriz[0]; i++)
	{
		for (int j = 0; j < datosIniciales.matriz[1]; j++) {
			printf("%d ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
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

//VER LA MATRIZ RESULTADO
//for (int i = 0; i < datosIniciales.matriz[0]; i++)
//{
//	for (int j = 0; j < datosIniciales.matriz[1]; j++) {
//		matriz[i][j] = 0;
//	}
//}
//
//while (resultado != NULL)
//{
//	matriz[resultado->posX][resultado->posY] = -1;
//	resultado = resultado->siguiente;
//}
//
//for (int i = 0; i < datosIniciales.matriz[0]; i++)
//{
//	for (int j = 0; j < datosIniciales.matriz[1]; j++) {
//		printf("%d ", matriz[i][j]);
//	}
//	printf("\n");
//}
//printf("\n");