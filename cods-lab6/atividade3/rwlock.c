/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 6
   Autor: Eduardo Barros

   Implementar o padrão leitores e escritores com prioridade para escrita.
   Implemente sua versão de rwlock com prioridade para operações de escrita.
   Isso significa que sempre que uma operação de escrita for solicitada,
   novas operações de leitura não poderão começar (mesmo que outras operações de
   leitura já estejam acontecendo), até que a operação de escrita seja atendida.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../list_int.h"
#include "../timer.h"

#define QTDE_OPS 10000000 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

int leit=0, escr=0; //contadores de leitura e escrita
pthread_mutex_t mutex; //variável de lock para exclusão mútua
pthread_cond_t cond_leit, cond_escr; //variável de condição para a exclusão mútua
struct list_node_s* head_p = NULL; //lista compartilhada iniciada  
int nthreads; //qtde de threads no programa

/**
* @brief Função que faz a entrada na leitura, se não houver escrita sendo realizada.
* @return Contador de leitores incrementado.
*/
void read_lock(){
    pthread_mutex_lock(&mutex);
    while(escr>0){
        pthread_cond_wait(&cond_leit,&mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Função que faz a saída da leitura.
* @return Contador de leitores decrementado.
*/
void read_unlock(){
    pthread_mutex_lock(&mutex);
    leit--;
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Função que faz a entrada na escrita, se não houver escrita sendo realizada.
* @return Contador de escritores incrementado.
*/
void write_lock(){
    pthread_mutex_lock(&mutex);
    while(escr>0){
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Função que faz a saída da escrita, liberando a escrita e todas as leituras aguardando.
* @return Contador de escritores decrementado.
*/
void write_unlock(){
    pthread_mutex_lock(&mutex);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Realiza as tarefas de consulta, inserção e remoção
* @param arg Número da thread passado como argumento
* @return 
*/
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<98) {
	 read_lock(); /* lock de LEITURA */    
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	 read_unlock();     
	 read++;
      } else if(98<=op && op<99) {
	 write_lock(); /* lock de ESCRITA */    
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 write_unlock();     
	 in++;
      } else if(op>=99) {
	 write_lock(); /* lock de ESCRITA */     
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	 write_unlock();     
	 out++;
      }
   }
   //registra a qtde de operações realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   // aloca memória para o vetor de id das threads
   tid = malloc(sizeof(pthread_t)*nthreads);

   //insere os primeiros elementos na lista

   //tomada de tempo inicial
   GET_TIME(ini);
   
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);

   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}