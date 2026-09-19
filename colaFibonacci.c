#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef struct nodoFibonacci{
    float peso; //peso de la arista mas barata hacia el arbol cobertor minimo
    int nombre;
    int cantHijos; //cantidad de hijos del nodo
    bool perdioHijo; // para marcar si nodo perdio hijo

    struct nodoFibonacci *padre;
    struct nodoFibonacci *hijo;
    struct nodoFibonacci *der;
    struct nodoFibonacci *izq;
} nodoFibonacci;

typedef struct colaFibonacci{
    nodoFibonacci *minimo; //puntero a la raiz de menor peso
    int cantNodos; //cantidad de nodos en la cola
} colaFibonacci;

//primero inicializamos la cola
colaFibonacci* iniCola() {
    colaFibonacci *Q = (colaFibonacci*) malloc(sizeof(colaFibonacci));
    Q->minimo = NULL;
    Q->cantNodos = 0;
    return Q;
}
//ahora inicializamos los nodos, es decir, insertarlos
nodoFibonacci* insertar(colaFibonacci *Q, int nombre_nodo, float peso){
    nodoFibonacci *nuevo= (nodoFibonacci*) malloc(sizeof(nodoFibonacci));
    nuevo->nombre = nombre_nodo;
    nuevo->peso = peso;
    nuevo->cantHijos = 0;
    nuevo->perdioHijo= false;
    nuevo->padre = NULL;
    nuevo->hijo= NULL;
    //inicialmente se va a apuntar a si mismo en una lista circular
    nuevo->der = nuevo; 
    nuevo->izq = nuevo;

    //si la cola esta vacia el nuevo nodo q insertemos sera el minimo
    if(Q->minimo == NULL){
        Q->minimo = nuevo;
    }
    else{
        //si no esta vacia, insertamos al nuevo nodo a la izquierda del minimo
        nuevo->der = Q->minimo;
        nuevo->izq = Q->minimo->izq;
        Q->minimo->izq->der = nuevo;
        Q->minimo->izq = nuevo;

        //ahora vemos si el nuevo es menor que el minimo y lo actualizamos
        if(nuevo->peso < Q->minimo->peso){
            Q->minimo =nuevo;
        }
    }
    Q->cantNodos++;
    return nuevo;
}

//funcion auxiliar para el extractmin
void unirArboles(colaFibonacci *Q){
    //calculo de grado maximo q puede tener un arbol y asi para el arreglo q guardara los arboles ocn ese tamaño
    int maxGrado = (int)(log2(Q->cantNodos)+2) + 1; //el +2 es un margen de seguridad (?)
    // el ** es un arreglo de punteros, sirve para detectar las colisiones, contendra las raices clasificadas por su grado
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

    //fusionamos arboles de igual grado
    for(int i=0; i< numRaices; i++){
        nodoFibonacci *x = raices[i];
        int grado = x->cantHijos; 
        //miramos si en el arreglo A ya esta ocupado, si lo está fusionamos...
        while(grado < maxGrado && A[grado] != NULL){
            nodoFibonacci *y = A[grado];
            if(x->peso > y->peso){
                //aca intercambiamos, el x quedara como el de menor peso ahora
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
        //caso de q A[grado] esta vacio...
        if(grado< maxGrado){
            A[grado]= x;
        }
    }
    free(raices);

    //ahora hay q reconstruir la lista de raices
    Q->minimo = NULL; //como la lista de raices cambio, desconectamos este minimo de las raices antiguas
    for(int i=0; i<maxGrado; i++){
        if (A[i] != NULL){ //para cada grado se ve si quedo un arbol en A
            //caso de que sea el primer arbol q sacamos de A
            if (Q->minimo == NULL){
                Q->minimo = A[i]; //ponemos q es el minimo por ahora... y hacemos q izq y der apunten a si mismo
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

        //vemos q pasa si min era el unico nodo en la cola (cola tiene q quedar vacia)
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




