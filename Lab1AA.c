#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc

// Estructura que guarda datos importantes del archivo de entrada
typedef struct
{
	int matriz[2];				// matriz[0] es X "ancho"  matriz[1] es Y "alto"
	int puntosObligatorios;		// Entero con la cantidad de puntos obligatorios que deben ir en la solucion
	int* arreglo;				// Arreglo de enteros que guarda los puntos obligatorios
} DatosArchivo;

// Estructura que es una lista enlazada doble con las coordenadas de los puntos resultados
typedef struct nodo
{
	struct nodo* siguiente;		// Puntero al siguiente nodo de la lista
	struct nodo* anterior;		// Puntero al anterior nodo de la lista
	int posX;					// Coordenadas X del punto
	int posY;					// Coordenadas Y del punto
} Lista;

DatosArchivo LeerArchivo(char* NombreArchivo);
int PosicionValida(int** Matriz, int CoordenadasX, int CoordenadasY, DatosArchivo Datos);
int** CrearMatriz(DatosArchivo Datos);
Lista* CrearLista(int CoordenadasX, int CoordenadasY);
Lista* AgregarNodo(Lista* Actual, int CoordenadasX, int CoordenadasY);
Lista* QuitarNodo(Lista* Actual);
Lista* CopiarLista(Lista* Actual);
int CompararListas(Lista* ResultadoNuevo, Lista* ResultadoViejo);
int LargoLista(Lista* List);
Lista* Baktraking(DatosArchivo Datos);
void EscribirResultados(Lista* Resultado, DatosArchivo Datos, char* NombreArchivoSalida);



// Entradas:
// -NombreArchivo: Puntero a caracteres que contiene con el nombre del archivo 

// Funcionamiento: Lee un archivo con el nombre ingresado y guarda los datos importantes en una estructura

// Salidas:
// -datos: Estructura tipo DatosArchivo que contiene los datos necesarios para encontrar la solucion al problema
DatosArchivo LeerArchivo(char* NombreArchivo) {
	
	DatosArchivo datos;								// Se declara una estructura DatosArchivo para almenar datos del archivo

	FILE* Archivo = fopen(NombreArchivo, "r");		// Se abre el archivo en modo lectura 

	// Si no se pudo leer el archivo, se termina la lectura y se devuelve una estructura con datos nulos
	if (Archivo == NULL)							
	{
		printf("Archivo no Existe\n");
		datos.matriz[0] = -1;
		datos.matriz[1] = -1;
		datos.puntosObligatorios = -1;
		datos.arreglo = NULL;
		return datos;
	}

	fscanf(Archivo, "%d %d\n", &datos.matriz[0], &datos.matriz[1]);			// Se leen las dimensiones de la matriz
	fscanf(Archivo, "%d\n", &datos.puntosObligatorios);						// Se lee la cantidad de puntos obligatorios

	datos.arreglo = (int*)malloc(sizeof(int) * datos.puntosObligatorios * 2);	// Se reserva espacio para un arreglo de enteros

	// Se leen y se guardan las coordenadas de los puntos obligatorios
	if (datos.puntosObligatorios != 0)
	{
		int contador = 0;

		while (feof(Archivo) == 0)
		{
			fscanf(Archivo, "%d %d\n", &datos.arreglo[contador], &datos.arreglo[contador + 1]);
			contador = contador + 2;
		}
	}

	fclose(Archivo);		// Se cierra el archivo

	return datos;
}


// Entradas:
// -Matriz: Matriz con los puntos de las posiciones disponibles y ocupadas
// -CoordenadasX: Coordenadas X de punto a verificar
// -CoordenadasY: Coordenadas Y de punto a verificar
// -Datos: Estructura con datos importantes

// Funcionamiento: Se verifica si el punto que se desea colocar no esta verticalmente, horizontalmente y
// en diagonal a otro punto de la matriz

// Salidas:
// -1: Se retorna un 1 si es posible colocar el punto
// -0: Se retorna un 0 si no es posible colocar el punto
int PosicionValida(int** Matriz, int CoordenadasX, int CoordenadasY, DatosArchivo Datos) {

	int x = CoordenadasX;							// Entero con las coordenadas X del punto a verificar
	int y = CoordenadasY;							// Entero con las coordenadas Y del punto a verificar
	
	// Se comprueban todos los puntos verticales
	for (int i = 0; i < Datos.matriz[0]; i++)
	{
		if (Matriz[i][CoordenadasY] != 0)
		{
			return 0;
		}

	}

	// Se comprueban todos los puntos horizonatal
	for (int i = 0; i < Datos.matriz[1]; i++)
	{
		if (Matriz[CoordenadasX][i] != 0)
		{
			return 0;
		}

	}

	// Se comprueban las diagonales derecha abajo
	while (x < Datos.matriz[0] && y < Datos.matriz[1])
	{
		if (Matriz[x][y] != 0)
		{
			return 0;
		}
		x++;
		y++;
	}

	// Se restablecen los enteros x e y con las coordenadas del punto a comprobar
	x = CoordenadasX;
	y = CoordenadasY;

	// Se comprueban las diagonales izquierda arriba
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

	// Se comprueban las diagonales izquierda abajo
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
	// Se comprueban las diagonales derecha arriba
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


// Entradas:
// -Datos: Estructura con datos importantes

// Funcionamiento: Se crea un arrglo de arreglo de enteros para simular una matriz e se inicializan sus puntos con 0

// Salidas:
// -matriz: puntero doble a un arreglo de arreglos que contiene una representacion de una matriz de ceros
int** CrearMatriz(DatosArchivo Datos) {
	
	int** matriz = (int**)malloc(Datos.matriz[0] * sizeof(int*));	// Se reserva espacio para un arreglo de arreglos

	// Se crea la matriz e inicializa cada punto con un 0
	for (int i = 0; i < Datos.matriz[0]; i++)					
	{
		matriz[i] = (int*)malloc(Datos.matriz[1] * sizeof(int));	// Se reserva espacio para un arreglo 
		for (int j = 0; j < Datos.matriz[1]; j++)				
		{
			matriz[i][j] = 0;										// Se inicializa el punto del arreglo con 0
		}
	}

	return matriz;
}


// Entradas:
// -CoordenadasX: Coordenadas X 
// -CoordenadasY: Coordenadas Y 

// Funcionamiento: Se crea una lista enlazada doble con un nodo con las coordenadas ingresadas

// Salidas:
// -lista: Lista enlazada doble que guarda las coordenadas ingresadas 
Lista* CrearLista(int CoordenadasX, int CoordenadasY) {

	// Se reserva espacio para una estructura de tipo Lista y se inicializan sus variables.
	Lista* lista = (Lista*)malloc(sizeof(Lista));	
	lista->anterior = NULL;
	lista->siguiente = NULL;
	lista->posX = CoordenadasX;
	lista->posY = CoordenadasY;

	return lista;
}


// Entradas:
// -Actual: Lista enlazada doble
// -CoordenadasX: Coordenadas X 
// -CoordenadasY: Coordenadas Y 

// Funcionamiento: Se crea un nuevo nodo con las coordenadas ingresadas y se agrega a la lista ingresada

// Salidas:
// -listaInicial: Lista ingresada con el nuevo nodo agregado al final de la lista
Lista* AgregarNodo(Lista* Actual, int CoordenadasX, int CoordenadasY) {

	Lista* listaInicial = Actual;						// Puntero a la lista ingresada

	// Se posiciona el puntero en el ultimo nodo de la lista
	while (listaInicial->siguiente != NULL)
	{
		listaInicial = listaInicial->siguiente;
	}

	// Se crea un nuevo nodo y se agrega a la lista actual
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


// Entradas:
// -Actual: Lista enlazada doble

// Funcionamiento: Elimina el ultimo nodo de la lista ingresada

// Salidas:
// -listaInicial: Se retorna la lista ingresada sin el ultimo nodo 
Lista* QuitarNodo(Lista* Actual) {

	Lista* listaInicial = Actual;						// Puntero a la lista ingresada
	Lista* nodoFinal = (Lista*)malloc(sizeof(Lista));	// Puntero que apuntara al ultimo nodo de la lista

	// Se posiciona el puntero de la lista ingresada en el ultimo nodo de la lista
	while (listaInicial->siguiente != NULL)
	{
		listaInicial = listaInicial->siguiente;
	}

	nodoFinal = listaInicial;					// Se apunta nodoFinal al ultimo nodo de la lista
	listaInicial = listaInicial->anterior;		// Se devuelve una posicion el puntero de la lista para que no se pierda al borrar su ultimo nodo
	free(nodoFinal);							// Se libera el ultimo nodo de la lista ingresada
	listaInicial->siguiente = NULL;				// Se apunta el puntero ->siguiente de la lista a NULL
	
	// Se vuelve al inicio de la lista
	while (listaInicial->anterior != NULL)
	{
		listaInicial = listaInicial->anterior;
	}

	return listaInicial;
}


// Entradas:
// -Actual: Lista enlazada doble

// Funcionamiento: Retorna una lista nueva que es una copia de la lista ingresada

// Salidas:
// -nuevaLista: Lista copia de la lista ingresada
Lista* CopiarLista(Lista* Actual) {

	Lista* listaInicial = Actual;					// Puntero a la lista ingresada

	// Se vuelve al inicio de la lista, en caso de no estarlo
	while (listaInicial->anterior != NULL)
	{
		listaInicial = listaInicial->anterior;
	}

	Lista* nuevaLista = CrearLista(listaInicial->posX, listaInicial->posY);		// Se crea la nueva lista que guardara la copia 

	// Se agregan todos los nodos de la lista ingresada a la lista nueva
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


// Entradas:
// -ResultadoNuevo: Lista con el nuevo resultado
// -ResultadoViejo: Lista con el resultado viejo

// Funcionamiento: Compara 2 listas y retorna un entero dependiendo del tamaño de las listas

// Salidas:
// -1: Retorna 1 en caso de que la lista nueva sea mas larga que la vieja
// -0: Retorna 0 en caso de que la lista nueva sea de largo menor o igual a la vieja 
int CompararListas(Lista* ResultadoNuevo, Lista* ResultadoViejo) {

	Lista* listaNueva = ResultadoNuevo;						// Puntero a la lista ingresada
	Lista* listaVieja = ResultadoViejo;						// Puntero a la lista ingresada

	// Se comparan los largos de las listas
	if (LargoLista(listaNueva) > LargoLista(listaVieja))
	{
		return 1;
	}
	
	else
	{
		return 0;
	}
}


// Entradas:
// -Actual: Lista enlazada doble

// Funcionamiento: Se cuenta el largo de la lista ingresada

// Salidas:
// -largo: Entero con el largo de la lista, sin contar el nodo incial
int LargoLista(Lista* Actual) {

	Lista* listaCopia = Actual;
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


// Entradas:
// -Datos: Estructura con datos importantes

// Funcionamiento: Funcion implementada atraves de Backtracking que busca todas las posiciones donde se pueden 
// colocar locales dentro de la matriz.

// Salidas:
// -resultado: Lista que contiene las coordenadas de todos los puntos donde se puede posicionar un local
Lista* Baktraking(DatosArchivo Datos) {
	
	int** matriz = CrearMatriz(Datos);	//Se crea la matriz 

	Lista* resultado = NULL;		// Puntero a la lista que guardara el resultado optimo
	Lista* recorrido = NULL;		// Puntero a la lista que guardara el camino que se va recoriendo para buscar la solucion

	// Se marcan en la matriz los puntos de los locales obligatorios 
	if (Datos.puntosObligatorios != 0)
	{
		for (int i = 0; i < Datos.puntosObligatorios; i++)
		{
			matriz[Datos.arreglo[i * 2]][Datos.arreglo[i * 2 + 1]] = 1; // Se marcan las coordenadas con un 1
		}
	}

	int x = 0;		// Entero con las coordenadas X de donde se inicia y continuara la busqueda del resultado
	int y = 0;		// Entero con las coordenadas Y de donde se inicia y continuara la busqueda del resultado

	// Se realiza un do while hasta que recorrido sea Lista nula
	do
	{
		for (int i = x; i < Datos.matriz[0]; i++)				// Para cada coordenada X de la matriz
		{

			for (int j = y; j < Datos.matriz[1]; j++)			// Para cada coordenada Y de la matriz
			{
				if (PosicionValida(matriz, i, j, Datos) == 1)	// Se revisa si el punto actual es valido para colocar un local
				{
					matriz[i][j] = -1;							// Si el punto es valido se marca en la matriz con un -1

					// Si la Lista recorrido es una de crea con el primer punto valido encontrado
					if (recorrido == NULL)					
					{
						recorrido = CrearLista(i, j);
					}
					else
					{
						// En caso que el nodo de la matriz tenga coordenadas nulas se cambian las coordenadas por las coordenadas del punto valido
						if (recorrido->posX == -1 && recorrido->posY == -1)
						{
							recorrido->posX = i;
							recorrido->posY = j;
						}
						// En caso contrario se agrega un nuevo nodo con las coordenadas del punto valido a la lista del recorrido
						else 
						{	
							AgregarNodo(recorrido, i, j);
						}
					}
				}
			}

			y = 0;	// Se reinicia el valor de entero "y" a 0 
		}

		// Si se termino el recorrido de la solucion y la lista aun tiene valores nulos se termina la busqueda de soluciones
		if (recorrido->posX == -1 && recorrido->posY == -1)
		{
			break;
		}

		// Se posiciona la lista de recorridos en el ultimo nodo
		while (recorrido->siguiente != NULL)
		{
			recorrido = recorrido->siguiente;
		}

		// Se vuelve a marcar como 0 las coordenadas del ultimo nodo del recorrido
		// para realizar una busqueda de otra solucion usando como punto de partida el ultimo nodo de la lista
		matriz[recorrido->posX][recorrido->posY] = 0;

		// Si las coordenadas del recorrido estan dentro de los parametros 
		if (recorrido->posX < Datos.matriz[0] && recorrido->posY < Datos.matriz[1]) 
		{
			// Se guardan las coordenadas de la posicion siguiente del ultimo punto
			// de la lista de recorridos, dependiendo del caso
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

		// Se reserva espacio para la lista de resultados y se copia el primer resultado encontrado
		if (resultado == NULL)
		{
			resultado = (Lista*)malloc(sizeof(Lista));
			resultado = CopiarLista(recorrido);
		}

		// Si la lista resultado a sido inicializa se copiara la lista de recorridos solo si se encuentra un 
		// resultado que contenga mas puntos que el ultimo resultado optimo encontrado
		else if (CompararListas(recorrido, resultado) == 1)
		{
			resultado = CopiarLista(recorrido);
		}

		// Si el largo de la lista es mayor a 1 se elimina el ultimo nodo de esta
		if (LargoLista(recorrido) > 0)
		{
			recorrido = QuitarNodo(recorrido);
		}
		// Si el largo de la lista de recorridos es 1 se verifica las coordenadas del punto
		else {	

			// Si las coordenadas del punto se encuentran en los limites de la matriz, se a terminado la busqueda de soluciones
			if (recorrido->posX == Datos.matriz[0] - 1 && recorrido->posY == Datos.matriz[1] - 1)
			{
				recorrido = NULL;
			}
			// Si las coordenadas siguen dentro de los limites de la matriz, se asignan valores nulos a las coordenadas
			// para que el nodo pueda ser usado denuevo
			else if (recorrido->posX < Datos.matriz[0] && recorrido->posY < Datos.matriz[1]) {

				recorrido->posX = -1;
				recorrido->posY = -1;
			}
		}
	} while (recorrido != NULL);

	return resultado;
}


// Entradas:
// -Resultado: Lista con las coordenadas optima de las posiciones de los locales
// -Datos: Estructura con datos importantes
// -NombreArchivoSalida: Puntero a caracteres que contiene con el nombre del archivo de salida

// Funcionamiento: Funcion que se encarga de crear un archivo de salida y escribir los resultado en el

// Salidas: retorna vacio
void EscribirResultados(Lista* Resultado, DatosArchivo Datos, char *NombreArchivoSalida) {
	
	FILE* archivo = fopen(NombreArchivoSalida, "w");		// Se crea y se abre un archivo en modo escritura
	int** matriz = CrearMatriz(Datos);						// Se crea una matriz

	// Se agregan los puntos obligatorios a la lista de resultados
	if (Datos.puntosObligatorios != 0)
	{
		for (int i = 0; i < Datos.puntosObligatorios; i++)
		{
			Resultado = AgregarNodo(Resultado, Datos.arreglo[i * 2], Datos.arreglo[i * 2 + 1]);
		}
	}

	// Se vuelve al inicio de la lista, en caso de no estarlo
	while (Resultado->anterior != NULL)
	{
		Resultado = Resultado->anterior;
	}

	// Se recorre la lista de resultados y se marcan con -1 todos los puntos con un local dentro de la matriz
	while (Resultado != NULL)
	{
		matriz[Resultado->posX][Resultado->posY] = -1;
		if (Resultado->siguiente == NULL)
		{
			break;
		}
		Resultado = Resultado->siguiente;
	}

	fprintf(archivo, "%d\n", LargoLista(Resultado) + 1);	// Se escribe el numero de locales que se pueden colocar

	// Se recorre la matriz y por cada local se escriben sus coordenadas
	for (int i = 0; i < Datos.matriz[0]; i++)
	{
		for (int j = 0; j < Datos.matriz[1]; j++) {
			if (matriz[i][j] != 0)
			{
				fprintf(archivo, "%d-%d || ", i, j);
			}
		}

	}

	fprintf(archivo, "\n");	// Se escribe un salto de linea

	// Por cada coordenada de la matriz se escribe un "_" si esta libre y una "X" si posee un local
	for (int i = 0; i < Datos.matriz[0]; i++)
	{
		for (int j = 0; j < Datos.matriz[1]; j++) {
			if (matriz[i][j] == 0)
			{
				fprintf(archivo, "_ ");
			}
			else
			{
				fprintf(archivo, "X ");
			}
		}
		fprintf(archivo, "\n");
	}

	fclose(archivo);		// Se cierra el archivo
}


// Entradas:
// Funcionamiento:
// Salidas:
void main() {
	
	DatosArchivo datosIniciales = LeerArchivo("Entrada3.in");		//Se realiza la lectura del archivo y se guardan los datos importantes en la estructura DatosArchivo

	Lista* resultado = Baktraking(datosIniciales);					//Se realiza la busqueda del resultado optimo con implementando Backtracking

	EscribirResultados(resultado, datosIniciales, "puto3.out");		//Se escribe el resultado en un archivo
}