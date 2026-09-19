#include <base.c>
typedef struct par{
    float costo;
    int nodo;
}par;

typedef struct arbolBk{
    par info;
    arbolBk *padre;
    arbolBk **hijos;
    int Bk;
}arbolBk;

arbolBk *crearArbol(float costo, int nodo, int n){
    arbolBk *arbol = malloc(sizeof(arbolBk));
    arbol->info.costo = costo;
    arbol->info.nodo = nodo;
    arbol->Bk = 0;
    arbol->padre = NULL;
    arbol->hijos = malloc(n * sizeof(arbolBk *));
    return arbol;
}

arbolBk *unir(arbolBk *arbolA, arbolBk *arbolB){
    if(arbolB->info.costo>arbolA->info.costo){
        arbolB->padre = arbolA;
        arbolA->hijos[arbolA->Bk] = arbolB;
        arbolA->Bk++;
        return arbolA;
    }
    else{
        arbolA->padre = arbolB;
        arbolB->hijos[arbolB->Bk] = arbolA;
        arbolB->Bk++;
        return arbolB;
    }
}

typedef struct colaBinomial{
    arbolBk **lista;
    arbolBk *minimo;
    int n;
}colaBinomial;

colaBinomial *crearCola(float *costos, int n){
    //crea una cola en base a la lista de costos(en inicio todos infinitos salvo 1), cada costo es de cada nodo (1,2,3...)
    colaBinomial *Q = malloc(sizeof(colaBinomial));
    Q->n = n;
    Q->lista = malloc((n+1) * sizeof(arbolBk *));
    Q->minimo = NULL;

    for(int i = 0; i<=n; i++){
        Q->lista[i]=NULL;
    }
    for(int i = 0; i<n; i++){
        insertar(Q,costos[i],i);
    }
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
    if(cola->minimo == NULL || arbol->info.costo < cola->minimo->info.costo){
        cola->minimo = arbol;
    }
}

void insertar(colaBinomial *cola, float costo, int nodo){
    arbolBk *arbol = crearArbol(costo, nodo, cola->n);
    insertarArbol(cola,arbol);
}


par extractMin(colaBinomial *Q){
    arbolBk *min = Q->minimo;
    par minimo = min->info;
    Q->lista[min->Bk]=NULL;
    for(int i = 0; i<min->Bk; i++){
        arbolBk *hijo = min->hijos[i];
        hijo->padre = NULL;
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

void decreaseKey(colaBinomial *Q, int v, int c){
    
}