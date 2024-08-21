/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 1
   Autor: Eduardo Barros
*/

#include <pthread.h>

typedef struct{
    int* começo;
    int* fim;
} Intervalo;

int* criaVetor(int N){
    int* v;
    int i;
    v = malloc(N*sizeof(int));
    for(i=0;i<N;i++){
        v[i] =i;
    }
    return v;
}

void* soma(void* itvl){
    Intervalo* intervalo = (Intervalo*) itvl;
    for(int* p = intervalo->começo; p < intervalo->fim; ++p){
        (*p)++;
    }
    free(intervalo);
    pthread_exit(NULL);
}

int checaVetor(int* vetor){
    int resultado = 1, i;
    for(i=0;i<N;i++){
        if(vetor[i]!=i+1){
            resultado=0;
        }
    }
}

int main(int argc, char* argv[]){
    int M,N;
    int* vetor;
    int parte;

    M = atoi(argv[1]);
    N = atoi(argv[2]);
    vetor = criaVetor(N);

    pthread_t threads[M];
    parte = N/M;

    for(int i=0; i<M;i++){
        Intervalo* intervalo = (Intervalo*) malloc(sizeof(Intervalo));
        intervalo->começo = vetor + i*parte;
        if(i == M-1){
            intervalo->fim = vetor+N;
        }else{
            intervalo->fim = intervalo->começo + parte;
        }

        print("Cria a thread %d\n", i+1);
        if(pthread_create(&threads[i], NULL, soma, intervalo)){
            printf("Erro ao criar thread");
            return 2;
        }
    }

    if(checaVetor(vetor)){
        printf("Vetor somado com sucesso");
    }else{
        print("Erro ao somar vetor");
    }

    free(vetor);
    pthread_exit(NULL);
}