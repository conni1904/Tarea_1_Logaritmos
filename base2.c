#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "base.h"


typedef struct nodoLista{ /*Estructura que representa a un nodo de una lista*/
    float costo; // Costo del nodo de la lista
    int nodo; // Número para identificar el nodo
    struct nodoLista *siguiente; //Puntero al siguiente nodo de la lista
}nodoLista;

typedef struct NodoGrafo{ /*Estructura que representa a un nodo de un grafo*/
    int nombre; /*Nombre para identificar el nodo del grafo*/
    nodoLista *conexiones; /*Puntero a una lista con las conexiones a otros nodos que tiene el nodo*/
    int grado;
}NodoGrafo;

typedef struct Grafo{ /*Estructura que representa a un grafo*/
    NodoGrafo *nodos; /*Puntero a nodo que conforma el grafo*/
    int numeroNodos; /* Cantidad total de nodos del grafo*/
    nodoLista *pool;
    int pool_usado;
    int pool_capacidad;
}Grafo;



/**
 * @brief Inserta un nuevo nodo al inicio de la lista de adyacencia de un nodo
 * Esta función inserta una arista o conexión al inicio de la lista de vecinos que tiene un nodo,
 * con complejidad temporal de O(1)
 * 
 * @param nodoGrafo como su nombre dice, es el nodo en donde queremos agregar la nueva conexión
 * @param costo_nodo costo asociado a la arista
 * @param nombre_nodo identificador del nodo destino
 */
void insertarLista(Grafo *g, NodoGrafo *nodoGrafo, float costo_nodo, int nombre_nodo){
    nodoLista *nuevo = &g->pool[g->pool_usado++];
    nuevo->costo = costo_nodo;
    nuevo->nodo = nombre_nodo;
    nuevo->siguiente = nodoGrafo->conexiones; 
    nodoGrafo->conexiones = nuevo;
    nodoGrafo->grado++;
}


/**
 * @brief Inicializa y reserva memoria para la estructura de un nuevo grafo
 * El grafo se crea con una cantidad fija de nodos, asignando a cada uno de ellos su nombre
 *  para identificarlo y sin conexiones.
 * 
 * @param numNodos Es la cantidad de nodos con el que se debe crear el grafo
 */
Grafo *crearGrafo(int numNodos, int maxAristas){
    Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->numeroNodos = numNodos;
    grafo->nodos = (NodoGrafo*) malloc(numNodos * sizeof(NodoGrafo));
    for(int i = 0; i<numNodos; i++){
        grafo->nodos[i].nombre = i;
        grafo->nodos[i].conexiones = NULL;
        grafo->nodos[i].grado = 0;
    }
    grafo->pool = malloc(2L * maxAristas * sizeof(nodoLista));
    grafo->pool_usado=0;
    grafo->pool_capacidad = 2*maxAristas;
    return grafo;
}
 
/**
 * @brief Función que crea una arista o conexión entre dos nodos.
 * Se crea una arista en los dos sentidos, es decir, si A--B se inserta a la lista de 
 * adyacencia de A a B y de B a A.
 * @param grafo Puntero al grafo donde se está creando la arista
 * @param nodoA Uno de los nodos que se quiere conectar
 * @param nodoB Uno de los nodos que se quiere conectar
 * @param peso Peso de la arista que se está creando
 */
void crearArista(Grafo *grafo, int nodoA, int nodoB, float peso){
    insertarLista(grafo, &(grafo->nodos[nodoA]), peso, nodoB);
    insertarLista(grafo, &(grafo->nodos[nodoB]), peso, nodoA);
}


/**
 * @brief Genera un peso aleatorio a una arista en el rango (0,1]
 * Esta función calcula un valor float "aleatorio" con distribución uniforme. Se le suma un 1
 * al resultado de rand para garantizar que no existan aristas de valor 0
 * 
 * @return un valor float dentro del rango (0,1]
 */
float generarPeso(){
    return (float)(rand() + 1) / (RAND_MAX + 1.0f);
}

float pesoMST(Grafo *g){
    float pesoTot = 0.0f;
    for(int i =0; i< g->numeroNodos ; i++){
        nodoLista *actual = g->nodos[i].conexiones;
        while(actual != NULL){
            pesoTot += actual->costo;
            //printf("peso actual: %f\n",actual->costo);
            //printf("nodo actual : %f\n",actual->nodo);
            //printf("--------------------------------\n");
            actual= actual->siguiente;
            

        }
    }
    pesoTot = pesoTot/2;
    return pesoTot;
}


Grafo *generadorAleatorio(int i, int j){
    int v = pow(2,i); //nodos
    int e = pow(2,j);  //aristas
    Grafo *grafo = crearGrafo(v,e);

    //hacemos arbol conexo
    for (int k =1; k<v; k++){
        int padre = rand()%k;
        crearArista(grafo, k, padre, generarPeso());
    }


    //ahora veamos las aristas restantes
    int restantes = e - v +1;
    for (int k = 0; k< restantes; k++){
        bool valido = false;
        int valorA;
        int valorB;
        while (!valido){
            valorA = rand()%v;
            valorB = rand()%v;
            if(valorA == valorB){ //esto es para que no se conecte consigo mismo el nodo
                continue; //con continue salta a la siguiente iteracion, no hace nada de lo de abajo
            }
            //ahora buscamos si la arista ya existe antes de crearla...
            int origenBusqueda = valorA;
            int objetivoBusqueda = valorB;

            if (grafo->nodos[valorB].grado < grafo->nodos[valorA].grado) {
                origenBusqueda = valorB;
                objetivoBusqueda = valorA;
            }
            bool yaExiste = false;
            nodoLista *actual = grafo->nodos[origenBusqueda].conexiones;
            while(actual!=NULL){
                if(actual->nodo == objetivoBusqueda){
                    yaExiste = true;
                    break;
                }
                actual = actual->siguiente;
            }
            if(!yaExiste){
                valido = true;
            }    
        }
        crearArista(grafo, valorA,valorB, generarPeso());
    }
    return grafo;
}

void liberarGrafo(Grafo *grafo) {
    if (grafo == NULL) return;
    free(grafo->pool);
    free(grafo->nodos);
    free(grafo);
}

