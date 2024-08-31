/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 3
   Autor: Eduardo Barros
*/
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

int main(int argc, char* argv[]){
    int i,j;//auxiliares de iteração
    int N,M, total; //dimensões da matriz
    float *matriz1, *matriz2; //matrizes de entrada
    float *saida; //matriz de saída
    double inicio, fim, delta;
    FILE *arquivoEntrada, *arquivoSaida;
    size_t retorno;

    GET_TIME(inicio);
    //Confere se todos os argumentos foram passados
    if(argc<3){
        fprintf(stderr, "Digite: %s <arquivo de entrada> <arquivo de saída>\n", argv[0]);
        return 1;
    }

    //Recebe e abre o arquivo, testando se o nome é válido
    arquivoEntrada = fopen(argv[1], "rb");
    if(!arquivoEntrada){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 2;
    }

    //Lê do arquivo a quantidade de linhas das matrizes
    retorno = fread(&N, sizeof(long int), 1, arquivoEntrada);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler tamanho dos vetores.\n");
     return 3;
    }

    //Lê do arquivo a quantidade de colunas das matrizes
    retorno = fread(&M, sizeof(long int), 1, arquivoEntrada);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler tamanho dos vetores.\n");
     return 3;
    }

    //Calcula o tamanho total das matrizes
    total = N*M;

    //Aloca memória para as matrizes
    matriz1 = (float *)malloc(total*sizeof(float));
    matriz2 = (float *)malloc(total*sizeof(float));
    if(!matriz1 || !matriz2){
        fprintf(stderr, "Erro ao alocar vetores.\n");
        return 4;
    }

    //Lê do arquivo os valores das matrizes
    retorno = fread(matriz1, sizeof(float), total, arquivoEntrada);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler valores da matriz 1.\n");
     return 5;
    }
    retorno = fread(matriz2, sizeof(float), total, arquivoEntrada);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler valores da matriz 2.\n");
     return 5;
    }

    //Aloca memória para a matriz de saída
    saida = (float *)malloc(total*sizeof(float));
    if(!saida){
        fprintf(stderr, "Erro ao alocar matriz de saída.\n");
        return 4;
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo para inicialização: %lf\n", delta);

    GET_TIME(inicio);
    //Faz a multiplicação das matrizes e salva na matriz de saída
    for(i=0;i<N;i++){
        for(j=0; j<M; j++){
            saida[i*M+j] = matriz1[i*M+j] * matriz2[i*M+j];
        }
    }

    //abre o arquivo de saída
    arquivoSaida = fopen(argv[2], "wb");
    if(!arquivoSaida){
        fprintf(stderr, "Erro ao criar arquivo.\n");
        return 2;
    }

    //Escreve no arquivo as dimensões da matriz e os seus valores
    retorno = fwrite(&N, sizeof(int),1,arquivoSaida);
    retorno = fwrite(&M, sizeof(int),1,arquivoSaida);
    retorno = fwrite(saida, sizeof(float),total,arquivoSaida);
    if(retorno<total){
        fprintf(stderr,"Erro ao escrever no arquivo\n");
        return 6;
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo para processamento: %lf\n", delta);

    GET_TIME(inicio);
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
    free(matriz1);
    free(matriz2);
    free(saida);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo para finalização: %lf\n", delta);

    return 0;
}