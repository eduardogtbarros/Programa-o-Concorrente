/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Lista de Exercícios: 1
   Autor: Eduardo Barros

   Questao 1(b) Escreva uma versao concorrente da funcao em C para calcular o valor de pi
   usando a formula de Bailey-Borwein-Plouffe. A funcao deve receber como entrada o valor
   de n, indicando que os n primeiros termos da serie deverao ser considerados. Será
   executada por M threads, com balanceamento de carga entre as threads.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <pthread.h>

int n; //quantidade de termos da série a serem considerados
int M; //quantidade de threads
double pi=0; //valor de π
pthread_mutex_t mutex; //variavel de lock para exclusao mutua

/**
* @brief Função que calcula o π usando a fórmula de Bailey-Borwein-Plouffe.
* @param arg Número da thread passado como argumento
* @return Retorna a aproximação de π calculada calculado.
*/
double *calculaPi(void* arg){
    long int t_id = (long int) arg;
    int i; //variavel auxiliar de iteração
    int parte, inicio, fim; //Variável da parte a ser calculada

    //Prepara a parte, de modo que cada thread trabalhe balanceadamente
    parte = n/M;
    inicio = t_id * parte;
    if(t_id==(M-1)){
        fim = n;
    } else{
        fim = inicio + parte;
    }

    //Realiza o somatório dos termos da série
    for(i=inicio;i<fim;i++){
        //Entra na seção crítica
        pthread_mutex_lock(&mutex);
        //Calcula o pi daquela parte
        pi+= (1.0/(pow(16.0,i)))*((4.0/(8.0*i+1.0))-(2.0/(8.0*i+4.0))-(1.0/(8.0*i+5.0))-(1.0/(8.0*i+6.0)));
        //Sai da seção crítica
        pthread_mutex_unlock(&mutex);
    }

    return pi;
}

int main(int argc, char *argv[]) {
    int i; //variável auxiliar de iteração
    pthread_t* threads;

    //Confere se todos os argumentos foram passados
    if(argc<3) {
        printf("Digite: %s <nº de termos da serie> <nº de threads>\n", argv[0]);
        return 1;
    }
    //Recebe a quantidade de termos
    n = atoi(argv[1]);
    //Recebe a quantidade de threads
    M = atoi(argv[2]);

    //Limita o número de threads ao número de termos
    if(M>n) M = n;

    //Aloca espaço para o vetor de ids das threads e confere o resultado
    threads = (pthread_t *) malloc(M* sizeof(pthread_t));
    if(!threads){
        fprintf(stderr, "Erro ao alocar vetor de threads.\n");
        return 1;
    }

    //Inicia o mutex
    pthread_mutex_init(&mutex, NULL);

    //Cria as threads
    for(i=0; i<M;i++){
        if(pthread_create(&threads[i], NULL, calculaPi, (void*) i)){
            printf("Erro ao criar thread."); exit(-1);
        }
    }
    //Aguarda o fim das threads
    for(i=0; i<M;i++){
        if(pthread_join(threads[i], NULL)){
            printf("Erro ao receber retorno das threads."); exit(-1);
        }
    }

    //Finaliza o mutex
    pthread_mutex_destroy(&mutex);
    
    printf("Aproximacao de Pi: %f",pi);

    free(threads);
    return 0;
}