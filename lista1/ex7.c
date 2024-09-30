/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Lista de Exercícios: 1
   Autor: Eduardo Barros

   Questão 7 Implemente uma solução concorrente em C para o problema
   dos produtores e consumidores (implementar as funções insere e retira
   e parâmetros globais) com a seguinte variação do problema: a cada
   execução de um produtor, ele deve preencher o buffer inteiro, e não
   apenas um único item (para isso ele deve esperar o buffer ficar
   completamente vazio). O consumidor segue a lógica convencional, isto é,
   insere um item de cada vez. A aplicação poderá ter mais de uma thread 
   produtora e mais de uma thread consumidora. Use variáveis de condição e
   locks para implementar os requisitos de sincronização.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 5 //tamanho do buffer

pthread_mutex_t mutex; //variável de lock para exclusão mútua
pthread_cond_t cond_cons, cond_prod; //variável de condição para a exclusão mútua
int buffer[TAM]; //área de dados compartilhada
int contador=0, i_ent, i_sai=0; //variável auxiliar para as operações

/**
* @brief Função que insere um elemento em todo o buffer.
* @return Retorna o buffer preenchido.
*/
void* insere(){
    int elemento; //elemento a ser adcionado no buffer
    
    //entra na seção crítica
    pthread_mutex_lock(&mutex);
    //checa se o buffer já não está preenchido e bloqueia se for o caso
    while(contador!=0){
        pthread_cond_wait(&cond_prod,&mutex);
    }
    //preenche o buffer com valores aleatórios
    for(i_ent=0;i_ent<TAM;i_ent++){
        elemento = rand() % 100;
        buffer[i_ent] = elemento;
        contador++;
    }
    //sai da seção crítica
    pthread_mutex_unlock(&mutex);

    //libera a thread consumidora
    pthread_cond_signal(&cond_cons);

    pthread_exit(NULL);
}

/**
* @brief Função que retira um elemento do buffer.
* @return Retorna o elemento retirado.
*/
void* retira(){
    int elemento; //elemento a ser retirado do buffer
    
    //entra na seção crítica
    pthread_mutex_lock(&mutex);
    //checa se o buffer está vazio e bloqueia se for o caso
    while(contador==0){
        pthread_cond_wait(&cond_cons,&mutex);
    }
    //decrementa o contador de elementos do buffer
    contador--;
    //retira o elemento do buffer
    elemento = buffer[i_sai];
    //incrementa o índice do elemento
    i_sai = (i_sai+1)%TAM;
    //sai da seção crítica
    pthread_mutex_unlock(&mutex);
    
    //libera a thread produtora
    pthread_cond_signal(&cond_prod);

    //retorna o elemento retirado
    pthread_exit((void*)elemento);
}

int main(int argc, char *argv[]){
    return 0;
}