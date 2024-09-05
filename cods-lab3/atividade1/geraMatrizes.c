/* Programa que cria arquivo com duas matrizes de valores do tipo float, gerados aleatoriamente 
 * Entrada: dimensoes das matrizes (linhas e colunas) e nome do arquivo de saida
 * Saida: arquivo binario com as dimensoes (valores inteiros) das matrizes (linhas e colunas), 
 * seguido dos valores (float) de todas as celulas das matrizes gerados aleatoriamente
 * */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char*argv[]) {
   float *matriz1, *matriz2; //matriz que será gerada
   long int linhas, colunas; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saida>\n", argv[0]);
      return 1;
   }
   linhas = atoi(argv[1]); 
   colunas = atoi(argv[2]);
   tam = linhas * colunas;

   //aloca memoria para a matriz
   matriz1 = (float*) malloc(sizeof(float) * tam);
   matriz2 = (float*) malloc(sizeof(float) * tam);
   if(!matriz1 || !matriz2) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return 2;
   }

   //preenche a matriz com valores float aleatorios
   //randomiza a sequencia de numeros aleatorios
   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
       *(matriz1+i) = (rand() % 1000) * 0.3;
   }
   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
       *(matriz2+i) = (rand() % 1000) * 0.3;
   }

   //escreve as matrizes no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[3], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve numero de linhas e de colunas
   ret = fwrite(&linhas, sizeof(long int), 1, descritorArquivo);
   ret = fwrite(&colunas, sizeof(long int), 1, descritorArquivo);
   //escreve os elementos das matrizes
   ret = fwrite(matriz1, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }
   ret = fwrite(matriz2, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(matriz1);
   free(matriz2);
   return 0;
}