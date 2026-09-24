#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "base.h"


typedef struct nodoFibonacci{ /*Estructura que define a cada nodo de una cola de Fibonacci*/
    float peso; /* Peso de la arista mas barata hacia el arbol cobertor minimo*/ 
    int nombre; /* Nombre para identificar al nodo*/
    int cantHijos; /* Cantidad de hijos del nodo*/
    bool perdioHijo; /* Flag para marcar si el nodo ha perdido un hijo*/ 

    struct nodoFibonacci *padre; /*Puntero a el padre del nodo*/
    struct nodoFibonacci *hijo; /*Puntero a uno de los hijos del nodo*/
    struct nodoFibonacci *der; /*Puntero a hermano derecho del nodo*/
    struct nodoFibonacci *izq; /*Puntero a hermano izquierdo del nodo*/
} nodoFibonacci;

typedef struct colaFibonacci{ /*Estructura que representa a la cola de Fibonacci*/
    nodoFibonacci *minimo; /* Puntero a la raiz de menor peso*/
    int cantNodos; //cantidad de nodos en la cola
} colaFibonacci;



/**
 * @brief Función que inicializa y reserva memoria para una cola de Fibonacci con el
 *  mínimo vacío y sin nodos.
 * 
 * @return Un puntero a la cola de Fibonacci recién creada
 */
colaFibonacci* iniCola() { 
    colaFibonacci *Q = (colaFibonacci*) malloc(sizeof(colaFibonacci));
    Q->minimo = NULL;
    Q->cantNodos = 0;
    return Q;
}



//ahora inicializamos los nodos, es decir, insertarlos
/**
 * 
 * @brief Función que inserta un nodo a la cola de Fibonacci.
 * 
 * Crea un nodo con los datos dados y lo inicializa como una lista circular de un
 * solo elemento y inserta en la raiz de la cola . Si el peso del nodo es menor al 
 * minimo de la cola, entonces se actualiza el puntero.
 * 
 * @param Q Puntero a la cola de Fibonacci.
 * @param nombre_nodo Nombre para identificar al nodo.
 * @param peso Costo de cada nodo con el que se ordenara en la cola.
 * 
 * @return Puntero al nuevo nodo creado.
 * 
 */
nodoFibonacci* insertar(colaFibonacci *Q, int nombre_nodo, float peso){
    nodoFibonacci *nuevo= (nodoFibonacci*) malloc(sizeof(nodoFibonacci));
    nuevo->nombre = nombre_nodo;
    nuevo->peso = peso;
    nuevo->cantHijos = 0;
    nuevo->perdioHijo= false;
    nuevo->padre = NULL;
    nuevo->hijo= NULL;
    //Inicialmente se apuntará a si mismo en una lista circular
    nuevo->der = nuevo; 
    nuevo->izq = nuevo;

    //Si la cola esta vacia el nuevo nodo q insertemos sera el minimo
    if(Q->minimo == NULL){
        Q->minimo = nuevo;
    }
    //Si no esta vacia, insertamos al nuevo nodo a la izquierda del minimo
    else{
        nuevo->der = Q->minimo;
        nuevo->izq = Q->minimo->izq;
        Q->minimo->izq->der = nuevo;
        Q->minimo->izq = nuevo;

        //ahora vemos si el nuevo es menor que el minimo y lo actualizamos de ser así.
        if(nuevo->peso < Q->minimo->peso){
            Q->minimo =nuevo;
        }
    }
    Q->cantNodos++;
    return nuevo;
}



//funcion auxiliar para el extractmin
/**
 * @brief Función auxiliar que une los arboles para extractMin
 * 
 * Se crea un arreglo A con tamaño del grado máximo que puede tener un arbol que servirá
 * para detectar cuando hay dos arboles del mismo tamaño y así fusionarlos. Para ello se
 * cuentan las raices en total que hay y los copiamos en un arreglo auxiliar. Luego de la 
 * fusión de arboles se vuelve a reconstruir el arreglo de raices.
 * 
 * @param Q Puntero a la cola de Fibonacci.
 * 
 * 
 */
void unirArboles(colaFibonacci *Q){
    //calculo de grado maximo q puede tener un arbol 
    int maxGrado = (int)(log2(Q->cantNodos)+2) + 1; //el +2 es un margen de seguridad

    // Puntero que sirve para detectar las colisiones, contendra las raices clasificadas 
    //por su grado
    nodoFibonacci **A = (nodoFibonacci**) malloc(maxGrado * sizeof(nodoFibonacci*));
    for (int i = 0; i< maxGrado; i++){
        A[i]=NULL; //inicialmente todos estaran vacios
    }
    //contamos la cantidad de raices (los recorremos con los izq y der)
    int numRaices = 0;
    if(Q->minimo != NULL){
        numRaices = 1;
        nodoFibonacci *actual = Q->minimo->der;
        while(actual != Q->minimo){
            numRaices++;
            actual = actual->der;
        }
    }

    //ahora los copiamos a un arreglo estatico, sirvee para tenerlo antes de fusionar
    nodoFibonacci **raices = (nodoFibonacci**) malloc(numRaices * sizeof(nodoFibonacci*));
    if(Q->minimo != NULL){
        raices[0]= Q->minimo;
        nodoFibonacci *actual = Q->minimo->der;
        int indice = 1;
        while(actual != Q->minimo){ 
            raices[indice] =actual;
            indice++;
            actual = actual->der;
        }
    }

    //Fusión de arboles de igual grado
    for(int i=0; i< numRaices; i++){
        nodoFibonacci *x = raices[i];
        int grado = x->cantHijos; 
        //miramos si en el arreglo A ya esta ocupado, si lo está fusionamos...
        while(grado < maxGrado && A[grado] != NULL){
            nodoFibonacci *y = A[grado];
            //Vemos si peso es menor o no que el que ya está en el arreglo y hacemos
            //que el x quede como el de menor peso e y el de mayor peso.
            if(x->peso > y->peso){
                nodoFibonacci *temp= x;
                x=y;
                y=temp;
            }
            //sacamos el y del arreglo de raices
            y->izq->der = y->der;
            y->der->izq = y->izq;
            //y ahora hacemos que y sea hijo de x
            y->padre=x;
            if(x->hijo ==NULL){ //si x no tenia hijos
                x->hijo = y;
                y->izq = y;
                y->der = y;
            }
            else{ //si x tenia hijos...
                y->der = x->hijo;
                y->izq = x->hijo->izq;
                x->hijo->izq->der = y;
                x->hijo->izq = y;
            }
            x->cantHijos++;
            y->perdioHijo= false;
            A[grado]=NULL;
            grado++;
        }
        //caso de q A[grado] esta vacio, solo lo agregamos al arreglo A sin hacer fusiones.
        if(grado< maxGrado){
            A[grado]= x;
        }
    }
    free(raices);

    //ahora hay q reconstruir la lista de raices con el resultado del arreglo A
    Q->minimo = NULL; //como la lista de raices cambio, desconectamos este minimo de las raices antiguas
    for(int i=0; i<maxGrado; i++){
        if (A[i] != NULL){ //para cada grado se ve si quedo un arbol en A
            //caso de que sea el primer arbol q sacamos de A
            if (Q->minimo == NULL){
                Q->minimo = A[i]; //ponemos q es el minimo por ahora y hacemos q izq y der apunten a si mismo
                Q->minimo->izq = Q->minimo;
                Q->minimo->der = Q->minimo;
            }
            else{ //caso de q no sea el primer arbol q sacamos de A
                //hacemos la insercion a la izquierda del nodo minimo
                A[i]->der =Q->minimo;
                A[i]->izq = Q->minimo->izq;
                Q->minimo->izq->der = A[i];
                Q->minimo->izq= A[i];
                //si el q acabamos de meter tiene menor peso, el puntero minimo pasa a apuntar a ese 
                if(A[i]->peso < Q->minimo->peso){
                    Q->minimo = A[i];
                }

            }
        }
    }
    free(A);
}



/**
 * 
 * @brief Función que extrae la raiz con el costo mínimo de la cola.
 * 
 * Al extraer el mínimo, todos los hijos de esa raiz deben ser insertados a la lista de 
 * raices y debe eliminarse el extraido de esta lista.
 * @param Q Puntero a la cola de Fibonacci.
 * @return el nodo mínimo extraido
 */
nodoFibonacci* extractMin(colaFibonacci *Q){
    nodoFibonacci *min = Q->minimo;
    if(min != NULL){
        //hay q subir los hijos del minimo a la lista de raices
        if(min->hijo !=NULL){
            int numHijos = min->cantHijos;
            //guardaoms los hijos en un arreglo temp
            nodoFibonacci **hijos = (nodoFibonacci**) malloc(numHijos *sizeof(nodoFibonacci*));
            nodoFibonacci *hijoactual = min->hijo;
            for (int i=0; i<numHijos; i++){
                hijos[i] = hijoactual;
                hijoactual = hijoactual->der;
            }
            //insertamos cada hijo en raices
            for(int i=0; i<numHijos; i++){
                nodoFibonacci *h= hijos[i];
                h->der= min;
                h->izq= min->izq;
                min->izq->der = h;
                min->izq=h;
                h->padre = NULL;

            }
            free(hijos);

        }
        //Ahora hay q sacar a el minimo de la lista de raices
        min->izq->der= min->der;
        min->der->izq= min->izq;

        //vemos q pasa si min era el unico nodo en la cola (cola tiene que quedar vacia)
        if(min==min->der){
            Q->minimo= NULL;
        }
        else{
            //si no era el unico, apuntaremos temporalmente al vecino derecho y haremos la union de arboles
            Q->minimo= min->der;
            unirArboles(Q);
        }
        Q->cantNodos--;
    }
    return min; //retornamos el nodo minimo extraido
}


/**
 * @brief Funcion que corta el nodo de su padre.
 * 
 * Esta función elimina el nodo 'x' de la lista de hijos de 'y' y decrementa el grado de 'y',
 * y también traslada a 'x' a la lista de raices.
 * 
 * @param Q puntero a la cola de Fibonacci donde se reinsertará el nodo a la raiz
 * @param x puntero al nodo hijo que será cortado y subido a raiz
 * @param y puntero al nodo padre que perderá a hijo 'x'
 * 
 */
void cut(colaFibonacci *Q, nodoFibonacci *x, nodoFibonacci *y){
    // Sacar a 𝑥 de la lista de hijos de 𝑦 y decrementar 𝑦.degree
    if(x->der == x){
        y->hijo = NULL;
    }
    else{
        x->izq->der = x->der;
        x->der->izq = x->izq;
        if(y->hijo == x){
            y->hijo= x->der;
        }
    }
    y->cantHijos--;

    // Agregar 𝑥 a la lista de árboles de Q (la de raices)
    x->der = Q->minimo;
    x->izq = Q->minimo->izq;
    Q->minimo->izq->der = x;
    Q->minimo->izq = x;

    //el punto 3 de la func xd
    x->padre = NULL;
    x->perdioHijo=false;
}


/**
 * @brief Funcion que realiza corte en cascada de forma recursiva
 * 
 * Esta función asegura que se mantenga el equilibrio de la estructura del arbol. Si un
 * nodo padre ya ha perdido un hijo antes y pierde otro, el padre también es cortado de
 * su padre y subido a la raiz y así recursivamente hacia arriba.
 * 
 * @param Q puntero a la cola de Fibonacci
 * @param y puntero al nodo que perdió un hijo
 * 
 * @see cut
 */
void cascadingCut(colaFibonacci *Q, nodoFibonacci *y){
    nodoFibonacci *padre = y->padre;
    if(padre != NULL){
        //si es el primer hijo q se pierde, solo se marca
        if(!y->perdioHijo){
            y->perdioHijo = true;
        }
        else{ //si es el segundo hijo perdido, cortamos a y y propagamos hacia arriba
            cut(Q,y, padre);
            cascadingCut(Q,padre);
        }
    }
}



/**
 * @brief Función que decrementa el peso de un nodo de la cola.
 * 
 */
void decreaseKey(colaFibonacci *Q, nodoFibonacci *x, float nuevoPeso){
    if(nuevoPeso > x->peso){
        return;
    }
    x->peso = nuevoPeso;
    nodoFibonacci *y = x->padre;
    //si no se cumple que el hijo es mayor q el padre
    if(y != NULL && x->peso < y->peso){
        cut(Q,x,y);
        cascadingCut(Q,y);
    }
    //actualizar minimo de Q
    if(x->peso < Q->minimo->peso){
        Q->minimo = x;
    }
}

Grafo *primFibonacci(Grafo *g, int r){
    int n = g->numeroNodos; //obtenemos la cantidad de nodos en total del grafo
    Grafo *T = crearGrafo(n); //creamos un grafo con la misma cant de vertices pero sin conexiones (asi iremos construyendo el mst)
    //punteros auiliares
    int *padre = (int*) malloc(n * sizeof(int)); // arreglo q contendrá para cada nodo i el nodo padre q lo conecte al arbol MST (en decir va anotando las aristas mas baratas )
    float *clave = (float*) malloc(n * sizeof(float)); //costo minimo para conectar un nodo al MST
    bool *enMST = (bool*) malloc(n * sizeof(bool)); //para no revisar nodos nuevamente si ya forma parte del MST
    nodoFibonacci **nodosCola= (nodoFibonacci**) malloc(n * sizeof(nodoFibonacci*));  //lista de "direcciones de acceso rápido", para q el decreasekey se haga en O(1)

    //inicializamos la cola
    colaFibonacci *Q= iniCola();
    //asignamos valores iniciales a los punteros auxiliares
    for(int i=0; i<n; i++){
        padre[i]= -1;
        clave[i]= INFINITY;
        enMST[i]= false;
        nodosCola[i]= insertar(Q, i, INFINITY); //insertamos todos los vertices a la cola Fibo con costo infinito y guardamos la referencia en nodoCola
    }
    //modificamos al al nodo "origen" r con costo 0
    clave[r]= 0.0f;
    decreaseKey(Q, nodosCola[r], 0.0f); //ahora con esto cambiamos el csoto en la cola de fibonacci y asi puntero Q->minimo queda en ese nodo tmb

    //ahora buscamos el nodo con menor peso con extractmin
    //bucle principal
    while(Q->cantNodos > 0){
        nodoFibonacci *nodoMinimo = extractMin(Q);
        if(nodoMinimo == NULL){
            break;
        }
        int u= nodoMinimo->nombre; //guardamos el nombre del nodo
        enMST[u] = true; //lo marcamos dentro en el mst
        free(nodoMinimo);

        //ahora exploramos los vecinos de u
        nodoLista *actual = g->nodos[u].conexiones;
        while(actual != NULL){
            int v = actual->nodo;
            float pesoArista = actual->costo;
            if(!enMST[v] && pesoArista<clave[v]){ //evaluamos q aun no este en mst y q la arista (u,v) es mas barata q la opcion ya conocida hasta el momento para alcanzar v 
                    clave[v]= pesoArista;
                    padre[v]=u;
                    decreaseKey(Q, nodosCola[v], pesoArista);
            }
            actual= actual->siguiente;
        }
    }
    //ahora si constuimos el arbol cobertor minimo
    for (int i =0; i<n; i++){
        if(padre[i] != -1){ //recorremos el arreglo de padres y para cada nodo q tenga una conexion valida lo insertamos la albrol
            crearArista(T, padre[i], i, clave[i]);
        }
    }
    free(padre);
    free(clave);
    free(enMST);
    free(nodosCola);
    free(Q);
    return T;
}


