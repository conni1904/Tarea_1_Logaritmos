#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* Documentacion */
typedef struct NodoGrafo{
    int nombre; //ver posibilidad de interconexion entre grafos (malo)
    float *conexiones; 
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
        grafo.nodos[i].conexiones = (float*) malloc(numNodos * sizeof(float));
        for(int j = 0 ; j<numNodos; j++){
            if(i == j){
                grafo.nodos[i].conexiones[j]= 0.0f;
            }
            else{
                grafo.nodos[i].conexiones[j] = INFINITY
            }
        }
    }
    return grafo;
}


void crearArista(Grafo *grafo, int nodoA, int nodoB, float peso){
    grafo->nodos[nodoA].conexiones[nodoB] = peso;
    grafo->nodos[nodoB].conexiones[nodoA] = peso;
}

float generarPeso(){
    return (float)(rand() + 1) / (RAND_MAX + 1.0f);
}

int indiceLista(int *lista, int cantidad, bool eliminacion){
    int indice = rand()%cantidad;
    int indice_elegido = lista[indice];
    if(eliminacion){
        lista[indice] = lista[cantidad-1];
    }
    return indice_elegido;

}

Grafo generadorAleatorio(int i, int j){
    int v = pow(2,i);
    int e = pow(2,j);
    Grafo grafo = crearGrafo(pow(2,i));
    int desconectados[v-1];
    int conectados[v];
    conectados[0] = 0;

    //inicializando las listas
    for(int k = 1; k < v; k++){
        desconectados[k-1] = k;
    }

    for(int k = 0; k < e; k++){
        if(k < v-1){
            float peso = generarPeso();
            int desconectado = indiceLista(desconectados, v-1-k, true); //hay q elegir elemento de desconectados 
            int conectado = indiceLista(conectados, k+1, false); //hay q elegir elemento de conectados
            conectados[k+1] = desconectado;
            crearArista(&grafo, desconectado, conectado, peso);
            
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
                    if(grafo.nodos[valorA].conexiones[valorB] == INFINITY){
                        valido = true;
                    }
                }
            }
            crearArista(&grafo, valorA, valorB, peso);
        }
    }
    return grafo;
}


//estructura de arbol binomial, lo sacamos de aux pss 

typedef struct abbTree{
	char* val;
	struct abbTree *left, *right;
}Tree;


Tree *initTree(char *value){
	Tree *ret = (Tree*)malloc(sizeof(Tree));
	ret->left = NULL;
	ret->right = NULL;
	ret->val = (char *)malloc(strlen(value)+1);
	strcpy(ret->val, value);
	return ret;
}
Tree *insertValue(Tree *root, char *value){
	if(root == NULL)
		return initTree(value);
	int cmp = strcmp(value,root->val);
	if(cmp<=0)
		root->left = insertValue(root->left, value);
	else
		root->right = insertValue(root->right, value);
	return root;
}