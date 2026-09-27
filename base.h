#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef struct nodoLista {
    float costo;
    int nodo;
    struct nodoLista *siguiente;
} nodoLista;

typedef struct NodoGrafo {
    int nombre;
    nodoLista *conexiones; 
    int grado;
} NodoGrafo;

typedef struct Grafo {
    NodoGrafo *nodos;
    int numeroNodos;
    nodoLista *pool;
    int pool_usado;
    int pool_capacidad;
} Grafo;

// Prototipos de funciones
void insertarLista(Grafo *g, NodoGrafo *nodoGrafo, float costo_nodo, int nombre_nodo);
bool buscarNodo(nodoLista *lista, int nombre_nodo);
Grafo *crearGrafo(int numNodos, int maxAristas);
void crearArista(Grafo *grafo, int nodoA, int nodoB, float peso);
float generarPeso(void);
float pesoMST(Grafo *g);
Grafo *generadorAleatorio(int i, int j);

#endif // BASE_H