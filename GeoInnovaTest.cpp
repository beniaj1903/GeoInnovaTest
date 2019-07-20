// GeoInnovaTest.cpp : Programa de consola que verifica las N muestras cercanas de una lista de puntos 
//


/* LIBRERÍAS STANDARD UTILIZADAS*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>  
using namespace std;

/*DECLARACIONES DE VARIABLES*/
struct Point // ESTRUCTURA DE UN PUNTO O UNA MUESTRA
{
	double distance; // solo utilizada para las muestras
	double x;
	double y;
	double z;
	
};

// VARIABLES DE ENTRADA
uint16_t NUMBER_OF_NEAREST_NEIGHBORS;	
double SEARCH_RADIUS;					
string sampleInputFileName;				
string pointInputFileName;
string outputFileName;
string paramsFilePath;


/* FUNCIONES */

// obtiene distancia en 3D
double GetDistance(double x1, double x2, double y1, double  y2, double  z1, double  z2) {
	return (double)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2) + pow((z2 - z1), 2));
}

// Ejecuta la busqueda binaria dentro de un arreglo ordenado
uint16_t BinarySearch(vector<Point> vector,Point target, uint16_t arraySize)
{	
	uint16_t middle = 0;
	uint16_t left = 0;
	uint16_t rigth = arraySize;

	while (left < rigth) // mientras el vector auxiliar tenga tamaño mayor que 1
	{
		middle = (left + rigth) / 2;
			
		if (vector[middle].distance < target.distance)
		{
			left++;
		}
		else if (vector[middle].distance > target.distance)
		{
			rigth--;
		}
		else return middle; // se retorna el indice si las distancias son iguales
	}

	if (vector[middle].distance > target.distance) return middle;	// si la distancia nueva es menor, se retorna el indice anterior al encontrado
	return middle + 1;												// de lo contrario se retorna el indice siguiente
}

/* FUNCION PRINCIPAL */
int main(int argc, char* argv[])
{
	// LECTURA DE PARAMETROS DE ENTRADA
	if (argc != 2) // no se ingreso archivo de parametros
	{
		cout << "Se requiere ingresar un archivo de parametros.\n";
		cout << "Ejecutando el programa de la forma:\n" << argv[0] <<" <ubicacion del archivo de parametros>\n";
		return 1;
	}
	else {	// se lee la ubicacion del archivo de parametros y se abre	
		ifstream paramsFlie;
		paramsFlie.open(argv[1]);
		paramsFilePath = argv[1];
		paramsFilePath = paramsFilePath.substr(0, paramsFilePath.find_last_of("/\\"));
		if (!paramsFlie.is_open())
		{
			cout << "No se pudo abrir el archivo de parámetros\n";
			return 1;
		}
		else { // se lee el archivo de parametros asignando las variables de entrada sus valores correspondientes
			string line;
			getline(paramsFlie, line);
			NUMBER_OF_NEAREST_NEIGHBORS = stoi(line);
			getline(paramsFlie, line);
			SEARCH_RADIUS = stof(line);
			getline(paramsFlie, line);
			sampleInputFileName = line;
			getline(paramsFlie, line);		
			pointInputFileName = line;
			getline(paramsFlie, line);
			outputFileName = line;
		}
		paramsFlie.close();
	}

	ifstream sampleInputFile, pointInputFile; // archivos de datos de los puntos y muestras
	pointInputFile.open(paramsFilePath + "\\" + pointInputFileName);
	if (!pointInputFile.is_open()) // se verifica la apertura del archivo de puntos
	{
		cout << "Problema abriendo el archivo de puntos, asegurese de que el archivo de parametros sea el correcto";
		return 1;
	}
	ofstream outputFile(outputFileName); // se verifica la creacion correcta del archivo de salida
	if (!outputFile.is_open())
	{
		cout << "Problema creando el archivo de salida";
		return 1;
	}

	string inputLine; // string donde se guardara la linea a leer de los archivos

	for (size_t i = 0; i < 6; i++) // se llega hasta la linea donde comienzan los puntos
	{
		getline(pointInputFile, inputLine);
	}

	do // recorrido del archivo de puntos
	{
		size_t sz1 = 0, sz2 = 0;
		Point pointFocus, sampleFocus; // punto y muestra actual
		
		// asignacion de las coordenadas del punto actual
		pointFocus.x = stod(inputLine, &sz1);
		pointFocus.y = stod(inputLine.substr(sz1), &sz2);
		pointFocus.z = stod(inputLine.substr(sz1 + sz2));

		vector <Point> sampleVector; // vector de punteros que guardara las N muestras cercanas

		sampleInputFile.open(paramsFilePath + "\\" + sampleInputFileName); // se verifica la apertura del archivo de muestras
		if (!sampleInputFile.is_open())
		{
			cout << "Problema abriendo el archivo de muestras, asegurese de que el archivo de parametros sea el correcto";
			return 1;
		}

		for (size_t i = 0; i < 6; i++) // se llega hasta la linea donde comienzan las muestras
		{
			getline(sampleInputFile, inputLine);
		}

		do // recorrido de muestras
		{
			// asignacion de las coordenadas de la muestra actual
			sampleFocus.x = stod(inputLine, &sz1);
			sampleFocus.y = stod(inputLine.substr(sz1), &sz2);
			sampleFocus.z = stod(inputLine.substr(sz1 + sz2));

			// se calcula la distancia de la muestra actual al punto actual
			sampleFocus.distance = GetDistance(pointFocus.x, sampleFocus.x, pointFocus.y, sampleFocus.y, pointFocus.z, sampleFocus.z);
			
			if (sampleFocus.distance < SEARCH_RADIUS) // si esta dentro del radio
			{
				if (sampleVector.size() == 0) // si el vector esta vacio
				{
					sampleVector.push_back(sampleFocus);
				}
				else if (sampleVector.size() == 1) // si el vector solo contiene un elemento
				{
					if(sampleFocus.distance < sampleVector[0].distance) // si la distancia del nuevo elemento es menor
						sampleVector.emplace(sampleVector.begin(), sampleFocus); // se coloca al inicio
					else
						sampleVector.push_back(sampleFocus); // sino se coloca al final
				}
				else // si el vector es de tamaño mayor a 2 //if(sampleVector.size() < NUMBER_OF_NEAREST_NEIGHBORS)
				{
					// se ejecuta una busqueda binaria dentro de el, para conocer la posicion del elementro a agregar
					uint16_t indexOfInput = BinarySearch(sampleVector, sampleFocus, sampleVector.size()); 

					// se coloca en su puesto y se corre hacia la derecha el resto de los valores
					sampleVector.emplace(sampleVector.begin() + indexOfInput, sampleFocus); 
					
					if (sampleVector.size() == NUMBER_OF_NEAREST_NEIGHBORS) // si se supero el limite de vecinos cercanos
					{
						sampleVector.pop_back(); // se extrae el vector con la mayor distancia
					}
				}
			}

		} while (getline(sampleInputFile, inputLine));

		outputFile << setprecision(4) << fixed << "vecinos_punto_( " << pointFocus.x << ", " << pointFocus.y << ", " << pointFocus.z << " )\n";
		if (sampleVector.size() == 0) outputFile << "\t" << "sin_muestras\n";
		for (size_t i = 0; i < sampleVector.size(); i++)
		{
			outputFile << "\t(" << sampleVector[i].x << ", " << sampleVector[i].y << ", " << sampleVector[i].z << ")\n";

		}

		sampleInputFile.close(); // se cierra el archivo de muestras

	} while (getline(pointInputFile, inputLine));

	// se cierran los archivos de salida y de puntos
	outputFile.close(); 
	pointInputFile.close();
}