/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 9
   @author: Eduardo Barros

   Objetivo: Usar os recursos de concorrencia de Go, com comunicação e sincronização
   via canais, para resolver o problema de encontrar números primos em uma lista de
   entrada.
   Descrição:
   1. Identificar todos os números primos de uma sequência gerada pelo fluxo principal
   (de 1 a N) e retornar a quantidade total de numeros primos encontrados.   
   2. O fluxo principal deverá atribuir/enviar os números para serem testados de forma
   concorrente por M goroutines.
   3. Use a função abaixo para verificar a primalidade de um número:
	int ehPrimo(long long int n) {
		if (n<=1) return 0;
		if (n==2) return 1;
		if (n%2==0) return 0;
		for (int i=3; i<sqrt(n)+1; i+=2)
			if(n%i==0) return 0;
		return 1;
	}
*/

package main
import "fmt"

func ehPrimo (n long long){
	if n<=1 {return false}
	if n==2 {return true}
	if n%2==0 {return false}
	for i:=0; i<sqrt(n)+1; i+=2{
		if n%i==0 {return false}
	}
	return true
}

func main(){

}