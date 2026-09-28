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
    printf("i=21, j=24\n");
    for(int k=0; k<10; k++){
        //COSTO TOTAL
        //seria a i=20
        //j=20
        Grafo *grafo = generadorAleatorio(21, 24);

        // cola fibomacci
        clock_t inif = clock();
        Grafo *mstf = primFibonacci(grafo,0);
        clock_t finf = clock();
        double tiempof = (double)(finf-inif)/ CLOCKS_PER_SEC;
        tiempoA1F[k]=tiempof;
        printf("Tiempo [%d] fibonacci: %f segundos\n", k + 1, tiempof);
        fflush(stdout);
        double pesoFibonacci = pesoMST(mstf);
        printf("peso fibonacci : %f\n", pesoFibonacci);
        liberarGrafo(mstf);

        //cola binomial
        clock_t inib = clock();
        Grafo *mstb = PrimBinomial(grafo,0);
        clock_t finb = clock();
        double tiempob = (double)(finb-inib)/ CLOCKS_PER_SEC;
        tiempoA1B[k]=tiempob;
        printf("Tiempo [%d] binomial: %f segundos\n", k + 1, tiempob);
        fflush(stdout);
        liberarGrafo(grafo);
        double pesoBinomial = pesoMST(mstb);
        printf("peso binomial : %f\n", pesoBinomial);
        liberarGrafo(mstb);

        //serie b




        //COSTO AMORTIZADO
        //serie c

        //serie d
       
    }
    return 0;


    
}