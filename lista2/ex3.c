/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Lista de Exercícios: 2
   @author Eduardo Barros

   Questão 3 Implemente uma solução para o padrão produtor/consumidor
   (implementar as funções insere e retira e parâmetros globais) com
   a seguinte variação do problema: a cada execução de um consumidor,
   ele deve consumir o buffer inteiro, e nao apenas um único item
   (para isso ele deve esperar o buffer ficar completamente cheio).
   O produtor segue a logica convencional, isto é, insere um item de
   cada vez. A aplicação poderá ter mais de um produtor e mais de um
   consumidor. Use a linguagem C com semáforos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM 10 //tamanho do buffer

sem_t mutex; //exclusão mútua
sem_t cond_cons, cond_prod; //condicional
int buffer[TAM]; //área de dados compartilhada
int contador=0; i_ent=0, i_sai; //variável auxiliar

/**
 * @brief Função que insere um elemento no buffer
 * @return Retorna o buffer com mais um elemento
*/
void* insere(){
    int elemento; //element a ser adcionado

    //entrada na seção crítica
    sem_wait(mutex);
    //checa se o buffer está cheio e bloqueia se for o caso
    while(contador==TAM){
        sem_wait(cond_prod);
    }
    //preenche o buffer com um valor aleatório
    elemento = rand()%100;
    buffer[i_ent] = elemento;
    //incrementa o índice do buffer e o contador
    i_ent = (i_ent+1)%TAM;
    contador++;
    //sai da seção crítica
    sem_post(mutex);

    //libera a thread consumidora
    sem_post(cond_cons);

    pthread_exit(NULL);
}

/**
 * @brief Função que retira todos os elementos do buffer.
 * @return vetor de elementos retirados do buffer
*/
void* retira(){
    int* elementos;//vetor de elementos removidos do buffer

    //entra na seção crítica
    sem_wait(mutex);
    //checa se o buffer está completo e bloqueia se não estiver
    while(contador<TAM){
        sem_wait(cond_cons);
    }
    //decrementa o contador e retira os elementos
    for(i_sai=0;i_sai<TAM;i_sai++){
        contador--;
        elementos+i_sai = buffer[i_sai];
    }
    //sai da seção crítica
    sem_post(mutex);
    //libera a thread produtora
    sem_post(cond_prod);
    //retorna o vetor de elementos retirados
    pthread_exit((void*)elementos);
}

int main(int argc, char* argv[]){
    int i, j; //variável auxiliar
    int produtores; //quantidade de produtores
    int consumidores; //quantidade de consumidores
    int* retorno; //vetor de retorno da thread
    pthread_t *t_prod, *t_cons; //vetor de threads

    srand(time(NULL)); //inicializa a função time

    //checa se os argumentos foram enviados
    if(argc<3){
        printf("Digite: %s <nº de produtores> <nº de consumidores>\n", argv[0]);
        return 1;
    }
    //Recebe a quantidade de produtores
    produtores = atoi(argv[1]);
    //Recebe a quantidade de consumidores
    consumidores = atoi(argv[2]);

    //aloca memória para os vetores de threads
    t_prod = malloc(sizeof(pthread_t)*produtores);
    t_cons = malloc(sizeof(pthread_t)*consumidores);

    //inicializa os semáforos com valor 1
    sem_init(&mutex, 0, 1);
    sem_init(&cond_prod, 0 ,1);
    sem_init(&cond_cons, 0 ,1);

    //cria as threads
    for(i=0;i<produtores;i++){
        if(pthread_create(&t_prod[i], NULL, insere, NULL)){
            printf("Erro ao criar thread."); exit(-1);
        }
    }
    for(i=0;i<consumidores;i++){
        if(pthread_create(&t_cons[i], NULL, retira, NULL)){
            printf("Erro ao criar thread."); exit(-1);
        }
    }
    
    //recebe o retorno das threads
    for(i=0;i<produtores;i++){
        if(pthread_join(t_prod[i], NULL)){
            printf("Erro ao receber retorno da thread."); exit(-1);
        }
    }
    for(i=0;i<consumidores;i++){
        if(pthread_join(t_cons[i], (void *) &retorno)){
            printf("Erro ao receber retorno da thread."); exit(-1);
        }
        printf("Consumidos:\n");
        for(j=0;j<TAM;j++){
            printf("%d\n",*retorno+j);
        }
    }

    //destrói os semáforos
    sem_destroy(&mutex);
    sem_destroy(&cond_prod);
    sem_destroy(&cond_cons);
    
    return 0;
}