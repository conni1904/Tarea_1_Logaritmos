#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "base2.c"
#include "colaFibonacci.c"
#include "binomial2.c"

double tiempoA1F[10];
double tiempoA1B[10];


int main (int argvc, char* argv[]){
    printf("i=18, j=22\n");
    for(int k=0; k<10; k++){
        //COSTO TOTAL
        //seria a i=20
        //j=20
        Grafo *grafo = generadorAleatorio(18, 22);

        // cola fibomacci
        
        Grafo *mstf = primFibonacci(grafo,0);
        fflush(stdout);
        liberarGrafo(mstf);

        //cola binomial
       
        Grafo *mstb = PrimBinomial(grafo,0);
        
        fflush(stdout);
        liberarGrafo(grafo);
        liberarGrafo(mstb);

        //serie b




        //COSTO AMORTIZADO
        //serie c

        //serie d
       
    }
    return 0;


    
}