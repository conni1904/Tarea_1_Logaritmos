#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <base.c>
#include <colaFibonacci.c>
#include <binomial.c>
#include <time.h>
double tiempoA1F[10];

int main (int argvc, char* argv[]){
    for(int k=0; k<10; k++){
        //COSTO TOTAL
        //seria a i=20
        //j=20
        Grafo *grafo = generadorAleatorio( 20, 20);

        //cola binomial
        clock_t ini = clock();

        clock_t fin = clock();
        double tiempo = (double)(fin-ini)/ CLOCKS_PER_SEC;


        //cola fibomacci
        clock_t ini = clock();
        primFibonacci(grafo,0);
        clock_t fin = clock();
        double tiempo = (double)(fin-ini)/ CLOCKS_PER_SEC;
        tiempoA1F[k]=tiempo;
         printf(tiempo);
        //serie b


        //COSTO AMORTIZADO
        //serie c

        //serie d
       
    }
    return 0;


    
}