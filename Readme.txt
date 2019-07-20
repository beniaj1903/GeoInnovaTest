Programado en C++, con el visual studio para su compilación, un solo archivo cpp, con dependencias a librerías del standard C++.

EXPLICACIÓN DE LA SOLUCIÓN

-ESTRUCTURA DE ALMACENAMIENTO
estructura comun para puntos y muestras, donde se almacenaba de forma dinamica las coordenadas x, y, z, y la distancia entre la muestra y el punto enfocado,para los puntos la distancia tenia un valor de 0. Para la lista de muestras por punto, se utilizo un vector de punteros de tipo Point (nombre asignado a la estructura elegida), en este se guarda de manera dinamica las muestras que se consideren vecinas al punto actual, se actualiza en cada nuevo ciclo de verificación del punto.


-BUSQUEDA Y ORDENAMIENTO DE LAS MUESTRAS
búsqueda binaria del indice donde debe encontrarse la muestra actual, colocandola a la izquierda o a la derecha del indice encontrado, en el caso de tener distancias iguales, se coloca la nueva muestra a la derecha. Si el vector de muestras para el punto actual llega al número de vecinos cercanos, se descarta el que se encuentre mas a la derecha, sabiendo que es el de mayor distancia. Una vez recorrido todo el archivo de muestras se imprime el vector final ya ordenado con este algoritmo. Cabe acotar que se verifica en cada muestra que se encuentre dentro de rango de busqueda antes de ejecutar el algoritmo de busqueda y ordenamiento.
