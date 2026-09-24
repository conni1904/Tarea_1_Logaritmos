#include "base.h"
#include <math.h>
typedef struct par{
    float costo;
    int nodo;
}par;

typedef struct arbolBk{
    par info;
    struct arbolBk *padre;
    struct arbolBk **hijos;
    int Bk;
    int k;
}arbolBk;

typedef struct colaBinomial{
    arbolBk **lista;
    arbolBk *minimo;
    int n;
    int *orden;
}colaBinomial;

void insertarArbol(colaBinomial *cola, arbolBk *arbol);

arbolBk *crearArbol(float costo, int nodo, int n){
    arbolBk *arbol = malloc(sizeof(arbolBk));
    arbol->info.costo = costo;
    arbol->info.nodo = nodo;
    arbol->Bk = 0;
    arbol->k = 0;
    arbol->padre = NULL;
    arbol->hijos = malloc(n * sizeof(arbolBk *));
    return arbol;
}
void cambiarK(arbolBk *arbol, int s){
    arbol->k=s;
    for(int i = 0; i<arbol->Bk; i++){;
        cambiarK(arbol->hijos[i],s);
    }
}

arbolBk *unir(arbolBk *arbolA, arbolBk *arbolB){
    if(arbolB->info.costo>arbolA->info.costo){
        arbolB->padre = arbolA;
        arbolA->hijos[arbolA->Bk] = arbolB;
        arbolA->Bk++;
        cambiarK(arbolA,arbolA->k+1);
        return arbolA;
    }
    else{
        arbolA->padre = arbolB;
        arbolB->hijos[arbolB->Bk] = arbolA;
        arbolB->Bk++;
        cambiarK(arbolB,arbolB->k+1);
        return arbolB;
    }
}





void ordenArbol(colaBinomial *cola,arbolBk *arbol){
    cola->orden[arbol->info.nodo]=arbol->k;
    for(int i = 0; i<arbol->Bk; i++){
        ordenArbol(cola, arbol->hijos[i]);
    }
}
void insertarB(colaBinomial *cola, float costo, int nodo){
    arbolBk *arbol = crearArbol(costo, nodo, cola->n);
    insertarArbol(cola,arbol);
}

colaBinomial *crearCola(float *costos, int n){
    //crea una cola en base a la lista de costos(en inicio todos infinitos salvo 1), cada costo es de cada nodo (1,2,3...)
    colaBinomial *Q = malloc(sizeof(colaBinomial));
    Q->n = n;
    Q->lista = malloc((n+1) * sizeof(arbolBk *));
    Q->minimo = NULL;
    Q->orden = malloc(n * sizeof(int));

    for(int i = 0; i<n; i++){
        Q->lista[i]=NULL;
    }
    for(int i = 0; i<n; i++){
        insertarB(Q,costos[i],i);
    }
    //puede que innecesarios
    /*for(int i = 0 ; i<log2(Q->n); i++){
        if(Q->lista[i] != NULL){
            ordenArbol(Q,Q->lista[i]);
        }
    }*/
    return Q;
}

void insertarArbol(colaBinomial *cola, arbolBk *arbol){
    int k = arbol->Bk;
    while(cola->lista[k]!=NULL){
        arbol = unir(cola->lista[k],arbol);
        cola->lista[k]=NULL;
        k++;
    }
    cola->lista[k] = arbol;
    ordenArbol(cola,arbol);
    if(cola->minimo == NULL || arbol->info.costo < cola->minimo->info.costo){
        cola->minimo = arbol;
    }
}




par extractMinB(colaBinomial *Q){
    arbolBk *min = Q->minimo;
    par minimo = min->info;
    Q->lista[min->Bk]=NULL;
    for(int i = 0; i<min->Bk; i++){
        arbolBk *hijo = min->hijos[i];
        hijo->padre = NULL;
        cambiarK(hijo,i);
        insertarArbol(Q, hijo);
    }
    Q->minimo = NULL;
    for(int i = 0; i<Q->n; i++){
        if(Q->lista[i]!=NULL){
            if(Q->minimo==NULL||Q->lista[i]->info.costo < Q->minimo->info.costo){
                Q->minimo = Q->lista[i];
            }
        }
    }
    free(min->hijos);
    free(min);
    Q->n--;
    return minimo;
}

arbolBk *encontrar(arbolBk *arbol, int nodo){
    if(arbol->info.nodo == nodo){
        return arbol;
    }
    for(int i = 0; i<(arbol->Bk); i++){
        return encontrar(arbol->hijos[i],nodo);
        //agregar otro return de ser necesario
    }
}

void reordenar(arbolBk *arbol){
    arbolBk *padre = arbol->padre;
    if (padre != NULL &&arbol->info.costo<padre->info.costo){
        par temporal = padre->info;
        padre->info = arbol->info;
        arbol->info = temporal;
        reordenar(padre);
    }
}

void decreaseKeyB(colaBinomial *Q, int v, int c){
    int lugar = Q->orden[v];
    arbolBk *arbol = encontrar(Q->lista[lugar],v);
    arbol->info.costo=c;
    reordenar(arbol);
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
    Grafo *T = crearGrafo(n);

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
            if(wu<costos[u]){
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
    free(Q->orden);
    free(Q);
    return T;
}