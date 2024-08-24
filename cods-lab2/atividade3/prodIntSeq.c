/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 2
   Autor: Eduardo Barros

1. Escreva um programa sequencial em C que gere dois vetores
de entrada (tipo:float) de dimensão N, com valores randomicos
(gere números fracionários). Escreva em um arquivo binário o 
valor de N (tipo: inteiro) e os dois vetores
(N ∗ sizeof(float) + N ∗ sizeof(float)). Depois calcule o 
produto interno desses dois vetores e escreva o resultado
encontrado no mesmo arquivo binário (tipo: double).
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100

/**
* @brief Função que calcula o produto interno de dois vetores.
* @param vetor1 1º vetor a ser utilizado
* @param vetor2 2º vetor a ser utilizado
* @param N tamanho dos vetores
* @return Retorna o produto interno calculado.
*/
double produtoInterno(float* vetor1, float* vetor2,long int N){
    double prodInt = 0;
    int i;
    for(i=0; i<N;i++){
        prodInt += vetor1[i]*vetor2[i];
    }
    return prodInt;
}

int main (int argc, char* argv[]){
    long int N, i;
    int fator=1;
    float *vetor1, *vetor2, num;
    double prodInt;
    FILE* arquivo;
    size_t retorno;

    if(argc<3){
        fprintf(stderr, "Digite: %s <tamanho> <arquivo>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    vetor1 = (float *)malloc(N*sizeof(float));
    vetor2 = (float *)malloc(N*sizeof(float));

    if(!vetor1 || !vetor2){
        fprintf(stderr, "Erro ao alocar vetores.\n");
        return 2;
    }

    srand(time(NULL));
    for(i=0;i<N;i++){
        num = (rand()%MAX)/3.0*fator;
        vetor1[i]=num;
        num = (rand()%MAX)/3.0*fator;
        vetor2[i]=num;
    }

    arquivo = fopen(argv[2], "wb");
    if(!arquivo){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 3;
    }

    fwrite(&N, sizeof(long int), 1, arquivo);
    retorno = fwrite(vetor1, sizeof(float), N, arquivo);
    if(retorno < N) {
      fprintf(stderr, "Erro ao escrever no  arquivo.\n");
      return 4;
    }
    retorno = fwrite(vetor2, sizeof(float), N, arquivo);
    if(retorno < N) {
      fprintf(stderr, "Erro ao escrever no  arquivo.\n");
      return 4;
    }

    prodInt = produtoInterno(vetor1,vetor2,N);
    retorno = fwrite(&prodInt, sizeof(double), 1, arquivo);

    fclose(arquivo);  
    free(vetor1); 
    free(vetor2); 
    return 0;
}