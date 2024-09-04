/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 3
   Autor: Eduardo Barros

   Projete uma solução concorrente para o problema de
   multiplicação de matrizes.
   As matrizes de entrada e saída serão do tipo float, com N 
   linhas e M colunas.
   As matrizes de entrada devem ser carregadas de arquivos 
   binários previamente gerados, onde os dois primeiros valores
   (do tipo inteiro) indicam as dimensões da matriz (N e M), e
   os demais elementos (do tipo float) sao a sequência de 
   valores da matriz.
   As matrizes deverão ser representadas internamente como 
   vetores de float (variável do tipo ponteiro, alocada 
   dinamicamente).
   A matriz de saída devera ser escrita em um arquivo binário,
   no mesmo formato dos arquivos de entrada.
   O programa devera receber como entrada, na linha de comando,
   os nomes dos arquivos de entrada e de saída, e a quantidade
   de threads de processamento.
   O programa deverá incluir chamadas de tomada de tempo de
   execução interna do programa, separando as partes de
   inicialização, processamento e finalização do programa.
*/
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <pthread.h>

int nthreads; //número de threads
float *matriz1, *matriz2; //matrizes de entrada
float *saida; //matriz de saída

typedef struct{
    int t_id;
    int tamanho;
} Args;


/**
* @brief Função que calcula a multiplicação das matrizes.
* @param linhas Quantidade de linhas da matriz
* @param colunas Quantidade de colunas da matriz
* @param matriz1 1ª matriz de entrada
* @param matriz2 2ª matriz de entrada
* @return Retorna a multiplicação salva em outra matriz.
*/
void *multMatrizes(void* arg){
    int i;//auxiliares de iteração
    int parte, inicio, fim; //Variável da parte a ser calculada
    Args* args = (Args*) arg; //Estrutura de argumentos
    
    //Prepara a parte, de modo que cada thread trabalhe balanceadamente
    parte = args->tamanho/nthreads;
    inicio = args->t_id * parte;
    if(args->t_id==(nthreads-1)){
        fim = args->tamanho;
    } else{
        fim = inicio + parte;
    }

    //Faz a multiplicação das matrizes e salva na matriz de saída
    for(i=inicio;i<fim;i++){
        saida[i] = matriz1[i] * matriz2[i];
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int i; //auxiliar de iteração
    int N,M, total; //dimensões da matriz
    double inicio, fim, delta;
    FILE *arquivoEntrada, *arquivoSaida;
    size_t retorno;
    pthread_t* threads;

    GET_TIME(inicio);
    //Confere se todos os argumentos foram passados
    if(argc<4){
        fprintf(stderr, "Digite: %s <arquivo de entrada> <arquivo de saída> <nº de threads>\n", argv[0]);
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

    //Recebe o número de threads e o limita ao tamanho da matriz
    nthreads = atoi(argv[3]);
    if(nthreads > total) nthreads = total;

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
    printf("Tempo para inicializacao: %lf\n", delta);

    GET_TIME(inicio);

    //Aloca espaço para o vetor de ids das threads e confere o resultado
    threads = (pthread_t *) malloc(nthreads* sizeof(pthread_t));
    if(!threads){
        fprintf(stderr, "Erro ao alocar vetor de threads.\n");
        return 6;
    }

    //Aloca espaço para a estrutura de argumentos e confere o resultado
    args = (Args *) malloc(nthreads* sizeof(Args));
    if(!args){
        fprintf(stderr, "Erro ao alocar estrutura de argumentos.\n");
        return 6;
    }

    //Cria as threads
    for(i=0; i<nthreads;i++){
        (args+i)->t_id = i;
        (args+i)->tamanho = total;

        if(pthread_create(&threads[i], NULL, multMatrizes, (void*) (args+i))){
            printf("Erro ao criar thread."); exit(-1);
        }
    }

    //Aguarda o término das threads
    for(i=0;i<nthreads;i++) pthread_join(*(&threads[i]));

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
    free(args);
    free(threads);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo para finalizacao: %lf\n", delta);

    return 0;
}