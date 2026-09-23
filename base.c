#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>



typedef struct nodoLista{
    float costo;
    int nodo;
    struct nodoLista *siguiente;
}nodoLista;

void insertarLista(nodoLista *lista, float costo_nodo, int nombre_nodo){
    if(lista->siguiente == NULL){
        nodoLista *nuevo = malloc(sizeof(nodoLista));
        nuevo->costo = costo_nodo;
        nuevo->nodo= nombre_nodo;
        nuevo->siguiente = NULL;
        lista->siguiente=nuevo;
    }
    else{
        insertarLista(lista->siguiente, costo_nodo, nombre_nodo);
    }
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


Grafo crearGrafo(int numNodos){
    struct Grafo grafo;
    grafo.numeroNodos = numNodos;
    grafo.nodos = (NodoGrafo*) malloc(numNodos * sizeof(NodoGrafo));
    for(int i = 0; i<numNodos; i++){
        grafo.nodos[i].nombre = i;
        grafo.nodos[i].conexiones = NULL; // REVISAR
    }
    return grafo;
}



void crearArista(Grafo *grafo, int nodoA, int nodoB, float peso){
    if (grafo->nodos[nodoA].conexiones==NULL){
        nodoLista *nuevo = malloc(sizeof(nodoLista));

        nuevo->costo = peso;
        nuevo->nodo= nodoB;
        nuevo->siguiente = NULL;
        grafo->nodos[nodoA].conexiones=nuevo;
    }
    else{
        insertarLista(grafo->nodos[nodoA].conexiones, peso, nodoB);
    }

    if (grafo->nodos[nodoB].conexiones==NULL){
        nodoLista *nuevo = malloc(sizeof(nodoLista));
        nuevo->costo = peso;
        nuevo->nodo= nodoA;
        nuevo->siguiente = NULL;
        grafo->nodos[nodoB].conexiones=nuevo;
    }
    else{
        insertarLista(grafo->nodos[nodoB].conexiones, peso, nodoA);
    }
}

float generarPeso(){
    return (float)(rand() + 1) / (RAND_MAX + 1.0f);
}


Grafo generadorAleatorio(int i, int j){
    int v = pow(2,i); //nodos
    int e = pow(2,j);  //aristas
    Grafo grafo = crearGrafo(pow(2,i));
    float peso = generarPeso();
    crearArista(&grafo, 0, 1, peso); //arista obligatoria
    for(int k = 0; k < e-1; k++){
        if(k<v-2){
            float peso = generarPeso();
            int nodoActual = k+2;
            int nodoConectado = rand()%nodoActual;
            crearArista(&grafo, nodoActual, nodoConectado, peso);
        }
        else{
            //tomamos dos numeros entre 0 y v-1, que no tengan conexion antes 
            float peso = generarPeso(); 
            bool valido = false;
            int valorA;
            int valorB;
            while (!valido){
                valorA = rand()% v;
                valorB = rand()% v;
                if(valorA != valorB) {
                    if(!buscarNodo(grafo.nodos[valorA].conexiones, valorB)){
                        valido = true;
                    }
                }
            }
            crearArista(&grafo, valorA, valorB, peso);
        }
    }
    return grafo;
}


