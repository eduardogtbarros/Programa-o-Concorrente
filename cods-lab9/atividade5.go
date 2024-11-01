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
import (
		"fmt"
		"math"
		"sync"
	)

//Função que testa se o número recebido é primo
func ehPrimo (n int) bool{
	if n<=1 {return false}
	if n==2 {return true}
	if n%2==0 {return false}
	for i:=3; i<int(math.Sqrt(float64(n)))+1; i+=2{
		if n%i==0 {return false}
	}
	return true
}

//Função executada pelas goroutines para checar a primalidade dos números recebidos
func verificaPrimos (numeros <- chan int, resultados chan <- int, wg *sync.WaitGroup){
	//Indica o término da goroutine para o WaitGroup
	defer wg.Done()
	
	//Envia 1 para o canal de resultados se n for primo
	for n := range numeros{
		if ehPrimo(n){
			resultados <- 1
		}
	}
}

func main(){
	N := 100 //quantidade de números
	T := 5 //quantidade de goroutines

	//Cria o canal de números e de resultados
	numeros := make(chan int, N)
	resultados := make(chan int, N)

	//Cria um WaitGroup para a sincronização das goroutines
	var wg sync.WaitGroup

	//Envia T goroutines
	for i:=0; i<T; i++{
		wg.Add(1)
		go verificaPrimos(numeros, resultados, &wg)
	}

	//Envia os números de 1 a N para o canal numeros
	go func(){
		for i:= 1; i<=N; i++{
			numeros <- i
		}
		//Fecha o canal depois de enviar todos
		close(numeros)
	}()

	//Goroutine para fechar o canal de resultados
	go func(){
		//Espera o término de todas as goroutines
		wg.Wait()
		//Fecha o canal de resultados
		close(resultados)
	}()

	//Conta o total de primos
	primos := 0
	for i:= 0; i<N; i++{
		primos += <- resultados
	}

	fmt.Printf("Número de primos de 1 até %d: %d.\n", N, primos)
}