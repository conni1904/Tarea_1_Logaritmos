#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "base.h"


typedef struct nodoLista{
    float costo;
    int nodo;
    struct nodoLista *siguiente;
}nodoLista;

typedef struct NodoGrafo{
    int nombre; //ver posibilidad de interconexion entre grafos (malo)
    nodoLista *conexiones; 
}NodoGrafo;

//de esta forma se inserta al inicio y no tenemos q recorrer toda la lista de nodos para pegar uno nuevo al final
//ahora esto esta en O(1) y lo q teniamos antes estaba en O(N)
void insertarLista(NodoGrafo *nodoGrafo, float costo_nodo, int nombre_nodo){
    nodoLista *nuevo = (nodoLista*) malloc(sizeof(nodoLista));
    nuevo->costo = costo_nodo;
    nuevo->nodo = nombre_nodo;
    nuevo->siguiente = nodoGrafo->conexiones; 
    nodoGrafo->conexiones = nuevo;
}

bool buscarNodo(nodoLista *lista, int nombre_nodo){
    if(lista == NULL){
        return false;
    }
    else if(lista->nodo == nombre_nodo){
        return true;
    }
    else{
        return buscarNodo(lista->siguiente, nombre_nodo);
    }

}

/* Doumentacion */
typedef struct NodoGrafo{
    int nombre; //ver posibilidad de interconexion entre grafos (malo)
    nodoLista *conexiones; 
}NodoGrafo;

typedef struct Grafo{
    NodoGrafo *nodos;
    int numeroNodos;
}Grafo;

Grafo *crearGrafo(int numNodos){
    Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->numeroNodos = numNodos;
    grafo->nodos = (NodoGrafo*) malloc(numNodos * sizeof(NodoGrafo));
    for(int i = 0; i<numNodos; i++){
        grafo->nodos[i].nombre = i;
        grafo->nodos[i].conexiones = NULL; // REVISAR
    }
    return grafo;
}
 //como ahora se inserta al inicio, los ifs q teníamos antes ya no hacen falta
void crearArista(Grafo *grafo, int nodoA, int nodoB, float peso){
    insertarLista(&(grafo->nodos[nodoA]), peso, nodoB);
    insertarLista(&(grafo->nodos[nodoB]), peso, nodoA);
}

float generarPeso(){
    return (float)(rand() + 1) / (RAND_MAX + 1.0f);
}


Grafo *generadorAleatorio(int i, int j){
    int v = pow(2,i); //nodos
    int e = pow(2,j);  //aristas
    Grafo *grafo = crearGrafo(v);

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
            bool yaExiste = false;
            nodoLista *actual = grafo->nodos[valorA].conexiones;
            while(actual!=NULL){
                if(actual->nodo == valorB){
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
    
    // 1. Liberar la lista enlazada de conexiones de cada nodo
    for (int i = 0; i < grafo->numeroNodos; i++) {
        nodoLista *actual = grafo->nodos[i].conexiones;
        while (actual != NULL) {
            nodoLista *temp = actual;
            actual = actual->siguiente;
            free(temp); // Liberar cada nodoLista individualmente
        }
    }
    
    // 2. Liberar el arreglo de nodos y la estructura del grafo
    free(grafo->nodos);
    free(grafo);
}

