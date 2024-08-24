/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 2
   Autor: Eduardo Barros

3. Escreva um programa concorrente em C que receba como
entrada o numero de threads T e um nome de arquivo, carregue
desse arquivo a dimensão N e dois vetores de entrada, execute 
o calculo do produto interno desses dois vetores dividindo a
tarefa entre as T threads de forma balanceada, e ao final
compare o valor calculado com o valor registrado no arquivo de
entrada.
4. Calcule a variação relativa considerando como valor de referência o
resultado do cálculo sequencial, ou seja: e = |(v_s - v_c)/v_s|, (onde
v_c: valor do programa concorrente e v_s: valor do programa sequencial).
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 

//Variáveis globais
int T; //Número de threads
long int N; //Tamanho dos vetores
float* vetor1, vetor2; /*vetor de elementos*/

/**
* @brief Função que calcula o produto interno de parte dos vetores.
* @param t_id Identificador da thread
* @return Retorna o produto interno calculado.
*/
double *produtoInterno(void* t_id){
    long int threadId = (long int) t_id; //ID da thread
    int parte, inicio, fim; //Variável da parte a ser calculada
    double prodInt=0; //Produto interno da parte
    double* retorno; //Ponteiro para o retorno
    
    //Prepara a parte, de modo que cada thread trabalhe balanceadamente
    parte = N/T;
    inicio = threadId * parte;
    if(id==(T-1)){
        fim = N;
    } else{
        fim = inicio + parte;
    }

    //Calcula o produto interno da parte
    for(i=inicio; i<fim;i++){
        prodInt += vetor1[i]*vetor2[i];
    }

    //Prepara, testa e envia o retorno
    retorno = malloc(sizeof(double));
    if(!retorno) { printf("Erro alocando retorno da thread.\n"); exit(-1); }
    *retorno = prodInt;
    pthread_exit((void*) retorno);
}

int main(int arc, char* argv[]){
    long int i;
    double prodIntSeq; //Produto interno calculado sequencialmente
    double prodIntCon=0; //Produto interno calculado concorrentemente
    double erro; //Variação relativa entre o produto interno sequencial e o concorrente
    double* prodIntThreads;
    FILE* arquivo; //Ponteiro do arquivo
    size_t retorno; //Variável de teste para leitura de arquivo
    pthread_t* threads; //Vetor de ids das threads

    //Confere se todos os argumentos foram passados
    if(argc<3){
        fprintf(stderr, "Digite: %s <nº de threads> <arquivo>\n", argv[0]);
        return 1;
    }

    //Recebe o número de threads da chamada
    T = atoi(argv[1]);

    //Recebe e abre o arquivo, testando se o nome é válido
    arquivo = fopen(argv[2], "rb");
    if(!arquivo){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 2;
    }

    //Lê do arquivo o tamanho dos vetores
    retorno = fread(&N, sizeof(long int), 1, arquivo);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler tamanho dos vetores.\n");
     return 3;
    }

    //Aloca memória para os vetores
    vetor1 = (float *)malloc(N*sizeof(float));
    vetor2 = (float *)malloc(N*sizeof(float));
    if(!vetor1 || !vetor2){
        fprintf(stderr, "Erro ao alocar vetores.\n");
        return 4;
    }

    //Lê do arquivo os vetores
    retorno = fread(vetor1, sizeof(float), N, arquivo);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler valores do vetor 1.\n");
     return 5;
    }
    retorno = fread(vetor2, sizeof(float), N, arquivo);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler valores do vetor 2.\n");
     return 5;
    }

    //Limita o número de threads ao tamanho dos vetores
    T>N? T = N : ;

    //Aloca espaço para o vetor de ids das threads e confere o resultado
    threads = (pthread_t *) malloc(T* sizeof(pthread_t));
    if(!threads){
        fprintf(stderr, "Erro ao alocar vetor de threads.\n");
        return 6;
    }

    //Cria as threads
    for(i=0; i<T;i++){
        if(pthread_create(&threads[i], NULL, produtoInterno, (void*) i)){
            printf("Erro ao criar thread."); exit(-1);
        }
    }

    //Recebe e soma os produto internos calculado pelas threads
    for(i=0; i<T;i++){
        if(pthread_join(threads[i], (void *)&prodIntThreads)){
            printf("Erro ao receber retorno das threads."); exit(-1);
        }
        prodIntCon += *prodIntThreads;
        free(prodIntThreads);
    }

    //Lê o produto interno calculado de forma sequencial do arquivo
    retorno = fread(&prodIntSeq, sizeof(long int), 1, arquivo);
    if(!retorno) {
     fprintf(stderr, "Erro ao ler produto interno sequencial.\n");
     return 7;
    }

    erro = calculaErro(prodIntSeq,prodIntCon);
    printf("Produto interno sequencial: %f\nProduto interno concorrente: %f\nErro(sequencial como base): %f\n", prodIntSeq,prodIntCon,erro);
    
    fclose(arquivo);  
    free(vetor1); 
    free(vetor2); 
    return 0;
}