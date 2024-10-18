/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 7
   Autor: Eduardo Barros

   Objetivo: Projetar e implementar um programa concorrente em C para
   ler, manipular e imprimir na tela os caracteres de um arquivo texto.
   Use semáforos para tratar as demandas de sincronização.
   Descrição: O programa devera ser implementado com 3 threads:
   1. a thread 1 carrega os caracteres do arquivo de entrada em blocos
   de tamanho N e os deposita no buffer1 de tamanho N.
   2. a thread 2 processa os caracteres do buffer1 acrescentando um caracter
   de final de linha a cada 2n + 1 caracteres lidos (n = 0, 1, 2..10) e
   deposita os caracteres no buffer2. Depois de n atingir o valor 10, o
   caracter de final de linha deverá ser inserido a cada bloco de 10
   caracteres lidos.
   3. a thread 3 imprime os caracteres do buffer2 na tela.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 20 //tamanho dos blocos de texto

char buffer1[N], buffer2[N+10]; //buffer para manipulação do arquivo
sem_t sem1, sem2; //semáforo para exclusão mútua

/**
* @brief Função que faz a leitura de N caracteres de um arquivo e os salva
* em um buffer.
* @param arg nome do arquivo
* @return Buffer preenchido de N caracteres.
*/
void* t1(void* arg){
    //Recebe e abre o arquivo, testando se o nome é válido
    FILE* arquivoEntrada = fopen((char*)arg, 'r');
    if(!arquivoEntrada){
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return 1;
    }

    //Salva N caracteres no buffer1, libera o uso dele e aguarda o
    //processamento para o buffer2.
    while(fgets(buffer1,N, arquivoEntrada)){
        sem_post(&sem1);
        sem_wait(&sem2);
    }
    fclose(arquivoEntrada);
    pthread_exit(NULL);
}

/**
* @brief Função que salva o conteúdo do buffer1 no buffer2, acrescentando
* caracteres de final de linha segundo uma lógica.
* @param arg argumento vindo da main
* @return Buffer com suas devidas separações de linha.
*/
void* t2(void* arg){
    int i, j; //auxiliar de iteração
    int contador=0; //contador de caracteres lidos
    
    while(1){
        //Aguarda o buffer1 ficar cheio
        sem_wait(&sem1);
        
        //Salva os caracteres do buffer1 no buffer2, acrescentando uma
        //quebra de linha se forem lidos 2*i + 1 caracteres ou 10 caracteres.
        j=0;
        for(i=0; buffer1[1] != '\0' && i<N;i++){
            buffer2[j] = buffer1[i];
            j++;
            contador++;
            if(contador == (2*i + 1)||contador==10){
                buffer2[j] = '\n';
                j++;
                contador=0;
            }
        }
        buffer2[j] = '\0';

        //Libera o uso do buffer2
        sem_post(&sem2);
    }
    pthread_exit(NULL);
}

/**
* @brief Função que imprime os caracteres do buffer2 na tela.
* @param arg argumento vindo da main
* @return Buffer impresso na tela.
*/
void* t3(void* arg){
    while(1){
        //Aguarda o buffer2 ficar pronto
        sem_wait(&sem2);

        //Imprime o conteúdo do buffer2 na tela
        printf("%s", buffer2);

        //Libera o buffer1 para ser preenchido novamente
        sem_post(&sem1);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int i; //auxiliar de iteração
    pthread_t tid[3]; //vetor de IDs de threads

    //Confere se todos os argumentos foram passados
    if(argc < 2){
        printf("Digite: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }
    //Inicia os semáforos
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    //Cria as threads
    pthread_create(&tid[0], NULL, t1, (void*)argv[1]);
    pthread_create(&tid[1], NULL, t2, NULL);
    pthread_create(&tid[2], NULL, t3, NULL);

    //Aguarda o fim das threads
    for(i=0; i<3; i++){
        pthread_join(tid[1], NULL);
    }

    //Destrói os semáforos
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}