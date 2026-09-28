#include "base.h"
#include <math.h>
#include <time.h>

/**
 * @brief Par que almacena el costo asociado a un nodo
 */
typedef struct par{
    double costo;
    int nodo;
}par;

/**
 * @brief Nodo de un arbol binomial
 */
typedef struct arbolBk{
    par info; /*información del nodo*/
    struct arbolBk *padre; /*puntero al padre del nodo*/
    struct arbolBk *hijo; /*puntero al primer hijo del nodo*/
    struct arbolBk *hermano; /*puntero al siguiente hermano*/
    int Bk; /*grado del arbol binomial*/
}arbolBk;

/** 
 * @brief La cola binomial
*/
typedef struct colaBinomial{
    arbolBk **lista; /* arreglo de punteros a cada arbol binomial de la cola, ej : lista[0] esta si existe el arbol B0*/
    arbolBk **pos; /*arreglo de punteros para localizar los nodos*/
    arbolBk *minimo; /*puntero al arbol que tiene el menor costo*/
    int n; /*cantidad de elementos en la cola*/
    int maxArbol; /*mayor grado de arbol Bk presente en la cola*/
}colaBinomial;

/**
 * @brief Crea un nuevo arbol B0
 * @param costo el costo de este nodo
 * @param nodo el nombre de este nodo
 * @return puntero al nuevo árbol creado
 */
arbolBk *crearArbol(double costo, int nodo){
    arbolBk *arbol = malloc(sizeof(arbolBk));
    arbol->info.costo = costo;
    arbol->info.nodo = nodo;
    arbol->Bk = 0;
    arbol->padre = NULL;
    arbol->hijo = NULL;
    arbol->hermano = NULL;
    return arbol;
}

/**
 * @brief Une dos arboles Bk para crear un arbol Bk+1
 * @param arbolA puntero a uno de los arboles Bk
 * @param arbolB puntero al otro  arbol Bk
 * @return un puntero al arbol Bk+1 resultante
 */
arbolBk *unir(arbolBk *arbolA, arbolBk *arbolB){
    if(arbolB->info.costo>arbolA->info.costo){ /*caso donde la raiz de A tiene menor costo*/
        arbolB->padre = arbolA;
        arbolB->hermano = arbolA->hijo;
        arbolA->hijo = arbolB;
        arbolA->Bk++;
        return arbolA;
    }
    else{  /*caso donde la raiz de B tiene menor costo*/
        arbolA->padre = arbolB;
        arbolA->hermano = arbolB->hijo;
        arbolB->hijo = arbolA;
        arbolB->Bk++;
        return arbolB;
    }
}

/**
 * @brief Inserta un arbol Bk a una cola binomial
 * @param cola la cola donde se insertara el arbol
 * @param arbol el arbol a insertar
 */
void insertarArbol(colaBinomial *cola, arbolBk *arbol){
    int k = arbol->Bk;
    while(cola->lista[k]!=NULL){ /*si la cola ya tiene un arbol del mismo Bk, se unen y se revisa el arbol Bk+1 sucesivamente*/
        arbol = unir(cola->lista[k],arbol);
        cola->lista[k]=NULL;
        k++;
    }
    cola->lista[k] = arbol;
    if(cola->minimo == NULL || arbol->info.costo < cola->minimo->info.costo){
        cola->minimo = arbol;
    }
}

/**
 * @brief Crea una cola binomial
 * @param costos arreglo de costos inicial para cada nodo
 * @param n cantidad de nodos que habran en la cola
 * @return Un puntero a la nueva cola binomial
 */
colaBinomial *crearCola(double *costos, int n){
    //crea una cola en base a la lista de costos(en inicio todos infinitos salvo 1), cada costo es de cada nodo (1,2,3...)
    colaBinomial *Q = malloc(sizeof(colaBinomial));
    Q->n = n;
    Q->minimo = NULL;
    Q->maxArbol = (int)floor(log2((double)n));
    Q->lista = malloc((Q->maxArbol + 1) * sizeof(arbolBk *));
    for(int i = 0; i<=Q->maxArbol; i++){ /*se inicializa la cola con solo NULL donde van los arboles*/
        Q->lista[i]=NULL;
    }

    Q->pos = malloc(n * sizeof(arbolBk *));
    for(int i = 0; i<n; i++){/*se inializa las posiciones de los nodos con solo NULL's*/
        Q->pos[i]=NULL;
    }

    for(int i = 0; i<n; i++){ /*se insertan uno por uno los nodos con su costo*/
        arbolBk *arbol = crearArbol(costos[i], i);
        Q->pos[i] = arbol;
        insertarArbol(Q,arbol);
    }
    return Q;
}

/**
 * @brief Extrae el par que tenga el costo menor en una cola binomial
 * @param Q la cola de donde se saca el minimo
 * @return El par coso y nodo del elemento extraido
 */
par extractMinB(colaBinomial *Q){
    arbolBk *min = Q->minimo; /*se guarda el minimo*/
    par minimo = min->info;
    Q->pos[minimo.nodo] = NULL; /*se le quita su posicion al nodo que tenia el minimo*/
    Q->lista[min->Bk]=NULL; /*se saca de la cola al minimo*/
    arbolBk *hijo = min->hijo; /*se guarda a su primer hijo*/
    while(hijo != NULL){ /*sucesivamente se va separando a los hijos del minimo y se agregan a la cola como nuevos arboles*/
        arbolBk *siguiente = hijo->hermano;
        hijo->hermano = NULL;
        hijo->padre = NULL;
        insertarArbol(Q, hijo);
        hijo = siguiente;
    }
    Q->minimo = NULL; /*ahora ya no hay minimo*/
    Q->n--; /*hay un nodo menos*/
    for(int i = 0; i<Q->maxArbol+1; i++){ /*se busca entre las raices de los arboles al nuevo minimo*/
        if(Q->lista[i]!=NULL){
            if(Q->minimo==NULL||Q->lista[i]->info.costo < Q->minimo->info.costo){
                Q->minimo = Q->lista[i];
            }
        }
    }
    free(min);
    return minimo;
}

/**
 * @brief Reordena un nodo despues de que se le haya modificado el costo
 * @param Q cola binomial que contiene al arbol
 * @param arbol nodo donde el costo fue modificado
 */
void reordenar(colaBinomial *Q, arbolBk *arbol){
    arbolBk *padre = arbol->padre;
    if (padre != NULL &&arbol->info.costo<padre->info.costo){ /*si tiene padre y su costo es menor de voltean */
        par temporal = padre->info;
        padre->info = arbol->info;
        arbol->info = temporal;
        Q->pos[padre->info.nodo] = padre;
        Q->pos[arbol->info.nodo] = arbol;
        reordenar(Q,padre);/*se sigue revisando si hay mas que ordenar*/
    }
    if(padre == NULL && Q->minimo->info.costo>arbol->info.costo){ /*se recalcula el minimo si es necesario*/
        Q->minimo = arbol;
    }
}

/**
 * @brief Disminuye el costo asociado a un nodo
 * @param  Q cola binomial al cual pertenece el nodo cambiado
 * @param v identificador del nodo
 * @param c nuevo costo del nodo
 */
void decreaseKeyB(colaBinomial *Q, int v, double c){
    arbolBk *arbol = Q->pos[v];
    arbol->info.costo=c;
    reordenar(Q, arbol);
}

/**
 * @brief Crea el MST usando el algoritmo de Prim y cola binomial
 * @param g el grafo al que se le sacara el MST
 * @param r el indentificador del primero nodo que se agregara al MST
 * @return un puntero al nuevo grafo que es el MST de g
 */
Grafo *PrimBinomial(Grafo *g,int r){
    int cuantosDecrease = 0; /*varible para serie C y D*/
    double tiempoAcum = 0.0; /*varible para serie C y D*/
    int n = g->numeroNodos;

    int *parent = (int*) malloc(n * sizeof(int)); 
    double *costos = (double*) malloc(n * sizeof(double));

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
        double c = min.costo;
        int v = min.nodo;
        if (v != r){
            crearArista(T, parent[v], v, c);
        }
        nodoLista *actual = g->nodos[v].conexiones;
        while(actual != NULL){
            int u = actual->nodo;
            double wu = actual->costo;
            if(Q->pos[u]!=NULL && wu<costos[u]){ 
                costos[u] = wu; 
                parent[u] = v;
                cuantosDecrease= cuantosDecrease + 1; /*seccion para serie C y D*/
                clock_t t_ini= clock(); /*seccion para serie C y D*/
                decreaseKeyB(Q,u,wu); 
                clock_t t_fin= clock();/*seccion para serie C y D*/
                tiempoAcum += (double)(t_fin-t_ini)/ CLOCKS_PER_SEC;/*seccion para serie C y D*/
            }
            actual = actual->siguiente;
        }

    }
    printf("Cantidad de decrease binomial : %d\n",cuantosDecrease);/*seccion para serie C y D*/
    printf("Tiempo acumulado binomial: %f\n",tiempoAcum);/*seccion para serie C y D*/

    free(parent);
    free(costos);
    free(Q->lista);
    free(Q->pos);
    free(Q);
    return T;
}