#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "base.h"

/**
 * @brief Estructura que representa un elemento en la lista enlazada de adyacencia.
 */
typedef struct nodoLista{
    double costo; /**< Peso o costo asociado a la arista que conecta con el nodo vecino. */
    int nodo; /**< Identificador único del nodo vecino de destino (0 a V-1). */
    struct nodoLista *siguiente; /**< Puntero al siguiente vecino en la lista de adyacencia*/
} nodoLista;


/**
 * @brief Estructura que representa un nodo (vértice) dentro del grafo.
 */
typedef struct NodoGrafo{ 
    int nombre; /**< Nombre para identificar el nodo del grafo*/
    nodoLista *conexiones; /**< Puntero a una lista con las conexiones a otros nodos que tiene el nodo*/
    int grado; /**< Número de vecinos conectados al nodo.*/
} NodoGrafo;


/**
 * @brief Estructura principal que representa el Grafo y su gestión de memoria.
 */
typedef struct Grafo{ 
    NodoGrafo *nodos; /**< Puntero a nodo que conforma el grafo */
    int numeroNodos; /**<  Cantidad total de nodos del grafo */
    nodoLista *pool; /**< Bloque único de memoria contigua reservado para todas las aristas */
    int pool_usado; /**< Contador del número de bloques de aristas dentro de pool */
    int pool_capacidad; /**< Capacidad máxima de bloques nodoLista reservados en pool */
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
void insertarLista(Grafo *g, NodoGrafo *nodoGrafo, double costo_nodo, int nombre_nodo){
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
void crearArista(Grafo *grafo, int nodoA, int nodoB, double peso){
    insertarLista(grafo, &(grafo->nodos[nodoA]), peso, nodoB);
    insertarLista(grafo, &(grafo->nodos[nodoB]), peso, nodoA);
}


/**
 * @brief Genera un peso aleatorio a una arista en el rango (0,1]
 * Esta función calcula un valor double "aleatorio" con distribución uniforme. Se le suma un 1
 * al resultado de rand para garantizar que no existan aristas de valor 0
 * 
 * @return un valor double dentro del rango (0,1]
 */
double generarPeso(){
    return (double)(rand() + 1) / (RAND_MAX + 1.0);
}

double pesoMST(Grafo *g){
    double pesoTot = 0.0;
    for(int i =0; i< g->numeroNodos ; i++){
        nodoLista *actual = g->nodos[i].conexiones;
        while(actual != NULL){
            pesoTot += actual->costo;
            actual= actual->siguiente;
            

        }
    }
    pesoTot = pesoTot/2;
    return pesoTot;
}


/**
 * @brief Genera un grafo aleatorio conexo y no dirigido de tamaño 2^i nodos y 2^j aristas.
 * 
 * La generación se realiza en dos etapas principales: Primero se garantiza la conexidad del grafo 
 * construyendo un árbol cobertor inicial mediante la conexión del nodo k con un nodo padre aleatorio
 * en el rango [0, k-1]. Segundo, se agregan las atistas restantes (2^j - 2^i + 1) seleccionando
 * pares de nodos de forma aleatoria, evitando autociclos y aristas duplicadas.
 * 
 * @param i Exponente base 2 para determinar el número de nodos
 * @param j Exponente base 2 para determinar el número total de aristas.
 * 
 * @return Grafo* Puntero a la estructura del grafo aleatorio generado en memoria.
 */
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


/**
 * @brief Libera toda la memoria dinámica asociada a la estructura de un Grafo.
 * 
 * Libera de forma eficiente los tres componentes principales del grafo
 * 
 * @param grafo Puntero a la estructura `Grafo` que se desea liberar de la memoria RAM.
 */
void liberarGrafo(Grafo *grafo) {
    if (grafo == NULL) return;
    free(grafo->pool);
    free(grafo->nodos);
    free(grafo);
}

