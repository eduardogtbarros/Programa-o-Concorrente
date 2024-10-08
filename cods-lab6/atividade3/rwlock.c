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
#include <pthread.h>

int leit=0, escr=0; //contadores de leitura e escrita
pthread_mutex_t mutex; //variável de lock para exclusão mútua
pthread_cond_t cond_leit, cond_escr; //variável de condição para a exclusão mútua

/**
* @brief Função que faz a entrada na leitura, se não houver escrita sendo realizada.
* @return Contador de leitores incrementado.
*/
void read_lock(){
    pthread_mutex_lock(&mutex);
    while(escr>0){
        pthread_cond_wait(&cond_escr,&mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

//------------------X-atualizar para se adaptar ao problema-X-------------------------//
/**
* @brief Função que faz a saída da leitura, liberando a escrita se não houver leitores.
* @return Contador de leitores decrementado.
*/
void read_unlock(){
    pthread_mutex_lock(&mutex);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Função que faz a entrada na escrita, se não houver escrita ou leitura sendo realizada.
* @return Contador de escritores incrementado.
*/
void write_lock(){
    pthread_mutex_lock(&mutex);
    while((leit>0)||(escr>0)){
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}

/**
* @brief Função que faz a saída da escrita, liberando a escrita e todas as leituras aguardando.
* @return Contador de leitores decrementado.
*/
void write_unlock(){
    pthread_mutex_lock(&mutex);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthrad_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

int main (int argc, char* argv[]){
    return 0;
}