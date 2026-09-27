#include "base.h"
#include <math.h>
typedef struct par{
    float costo;
    int nodo;
}par;

typedef struct arbolBk{
    par info;
    struct arbolBk *padre;
    struct arbolBk *hijo;
    struct arbolBk *hermano;
    int Bk;
}arbolBk;

typedef struct colaBinomial{
    arbolBk **lista;
    arbolBk **pos;
    arbolBk *minimo;
    int n;
    int maxArbol;
}colaBinomial;

arbolBk *crearArbol(float costo, int nodo){
    arbolBk *arbol = malloc(sizeof(arbolBk));
    arbol->info.costo = costo;
    arbol->info.nodo = nodo;
    arbol->Bk = 0;
    arbol->padre = NULL;
    arbol->hijo = NULL;
    arbol->hermano = NULL;
    return arbol;
}

arbolBk *unir(arbolBk *arbolA, arbolBk *arbolB){
    if(arbolB->info.costo>arbolA->info.costo){
        arbolB->padre = arbolA;
        arbolB->hermano = arbolA->hijo;
        arbolA->hijo = arbolB;
        arbolA->Bk++;
        return arbolA;
    }
    else{
        arbolA->padre = arbolB;
        arbolA->hermano = arbolB->hijo;
        arbolB->hijo = arbolA;
        arbolB->Bk++;
        return arbolB;
    }
}

void insertarArbol(colaBinomial *cola, arbolBk *arbol){
    int k = arbol->Bk;
    while(cola->lista[k]!=NULL){
        arbol = unir(cola->lista[k],arbol);
        cola->lista[k]=NULL;
        k++;
    }
    cola->lista[k] = arbol;
    if(cola->minimo == NULL || arbol->info.costo < cola->minimo->info.costo){
        cola->minimo = arbol;
    }
}

colaBinomial *crearCola(float *costos, int n){
    //crea una cola en base a la lista de costos(en inicio todos infinitos salvo 1), cada costo es de cada nodo (1,2,3...)
    colaBinomial *Q = malloc(sizeof(colaBinomial));
    Q->n = n;
    Q->minimo = NULL;
    Q->maxArbol = (int)floor(log2((double)n));
    Q->lista = malloc((Q->maxArbol + 1) * sizeof(arbolBk *));
    for(int i = 0; i<=Q->maxArbol; i++){
        Q->lista[i]=NULL;
    }

    Q->pos = malloc(n * sizeof(arbolBk *));
    for(int i = 0; i<n; i++){
        Q->pos[i]=NULL;
    }

    for(int i = 0; i<n; i++){
        arbolBk *arbol = crearArbol(costos[i], i);
        Q->pos[i] = arbol;
        insertarArbol(Q,arbol);
    }
    return Q;
}

par extractMinB(colaBinomial *Q){
    arbolBk *min = Q->minimo;
    par minimo = min->info;
    Q->pos[minimo.nodo] = NULL;
    Q->lista[min->Bk]=NULL;
    arbolBk *hijo = min->hijo;
    while(hijo != NULL){
        arbolBk *siguiente = hijo->hermano;
        hijo->hermano = NULL;
        hijo->padre = NULL;
        insertarArbol(Q, hijo);
        hijo = siguiente;
    }
    Q->minimo = NULL;
    Q->n--;
    for(int i = 0; i<Q->maxArbol+1; i++){
        if(Q->lista[i]!=NULL){
            if(Q->minimo==NULL||Q->lista[i]->info.costo < Q->minimo->info.costo){
                Q->minimo = Q->lista[i];
            }
        }
    }
    free(min);
    return minimo;
}

void reordenar(colaBinomial *Q, arbolBk *arbol){
    arbolBk *padre = arbol->padre;
    if (padre != NULL &&arbol->info.costo<padre->info.costo){
        par temporal = padre->info;
        padre->info = arbol->info;
        arbol->info = temporal;
        Q->pos[padre->info.nodo] = padre;
        Q->pos[arbol->info.nodo] = arbol;
        reordenar(Q,padre);
    }
    if(padre == NULL && Q->minimo->info.costo>arbol->info.costo){
        Q->minimo = arbol;
    }
}

void decreaseKeyB(colaBinomial *Q, int v, float c){
    arbolBk *arbol = Q->pos[v];
    arbol->info.costo=c;
    reordenar(Q, arbol);
}

Grafo *PrimBinomial(Grafo *g,int r){
    int n = g->numeroNodos;

    int *parent = (int*) malloc(n * sizeof(int)); 
    float *costos = (float*) malloc(n * sizeof(float));

    costos[r] = 0.0f;
    parent[r] = -1;

    for(int i=0; i<n; i++){
        if(i!=r){
            costos[i] = INFINITY;
            parent[i] = -1;
        }
    }

    colaBinomial *Q = crearCola(costos, n);
    Grafo *T = crearGrafo(n, n-1);

    while(Q->n > 0){
        par min = extractMinB(Q);
        float c = min.costo;
        int v = min.nodo;
        if (v != r){
            crearArista(T, parent[v], v, c);
        }
        nodoLista *actual = g->nodos[v].conexiones;
        while(actual != NULL){
            int u = actual->nodo;
            float wu = actual->costo;
            if(Q->pos[u]!=NULL && wu<costos[u]){
                costos[u] = wu; 
                parent[u] = v;
                decreaseKeyB(Q,u,wu);
            }
            actual = actual->siguiente;
        }
    }
    free(parent);
    free(costos);
    free(Q->lista);
    free(Q->pos);
    free(Q);
    return T;
}