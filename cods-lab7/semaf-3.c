#include <stdio.h>
#include <srtdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 8
#define N 20

FILE* arquivoEntrada;
char[N] buffer1;

sem

void* t1(void* arg){
    retorno = fread(&buffer1, sizeof(char), N, arquivoEntrada);
    if(!retorno) {
    fprintf(stderr, "Erro ao ler tamanho dos vetores.\n");
    return 1;
    }
}

void* t2(void* arg){

}

void* t3(void* arg){

}

int main(int argc, char* argv[]){
    pthread_t tid[NTHREADS];

    if(argc < 2){
        printf("Digite: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }
    if(!arquivoEntrada = fopen(argv[1], "r")){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 1;
    }

    // sem_init(&estado1, 0, 0);
    // sem_init(&estado2, 0, 0);

    return 0;
}