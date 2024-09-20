/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Lista de Exercícios: 1
   Autor: Eduardo Barros

   Questao 1(a) Escreva uma funcao em C para calcular o valor de pi usando a formula de
   Bailey-Borwein-Plouffe. A funcao deve receber como entrada o valor de n, indicando que
   os n primeiros termos da serie deverao ser considerados.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

/**
* @brief Função que calcula o π usando a fórmula de Bailey-Borwein-Plouffe.
* @param n Quantidade de termos da série a serem considerados
* @return Retorna a aproximação de π calculada calculado.
*/
double calculaPi(int n){
    int i; //variavel auxiliar de iteração
    double pi=0; //valor de π

    //Realiza o somatório dos termos da série
    for(i=0;i<n;i++){
        pi+= (1.0/(pow(16.0,i)))*((4.0/(8.0*i+1.0))-(2.0/(8.0*i+4.0))-(1.0/(8.0*i+5.0))-(1.0/(8.0*i+6.0)));
    }

    return pi;
}

int main(int argc, char *argv[]) {
    int n; //quantidade de termos da série a serem considerados
    double pi; //valor de π

    //Confere se todos os argumentos foram passados
    if(argc<2) {
        printf("Digite: %s <numero de termos da serie>\n", argv[0]);
        return 1;
    }
    //Recebe a quantidade de termos
    n = atoi(argv[1]);

    //Atribui o retorno da função à pi e printa na tela
    pi = calculaPi(n);
    printf("Aproximacao de Pi: %f",pi);

    return 0;
}