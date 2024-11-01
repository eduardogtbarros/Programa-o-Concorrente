/* Disciplina: Programação Concorrente
   Profa.: Silvana Rossetto
   Laboratório: 9
   @author: Eduardo Barros

   Estenda o programa fazendo o fluxo principal e a goroutine implementarem o
   seguinte diálogo:
   Main envia para Goroutine: ’Olá, Goroutine, bom dia!’ (impresso pela Goroutine)
   Goroutine envia para Main: ’Oi Main, bom dia, tudo bem?’ (impresso pela Main)
   Main envia para Goroutine: ’Tudo bem! Vou terminar tá?’ (impresso pela Goroutine)
   Goroutine envia para Main: ’Certo, entendido.’ (impresso pela Main)
   Goroutine imprime: finalizando
   Main imprime: finalizando
*/

package main

import (
	"fmt"
)

func tarefa(strMain chan string, strGoRoutine chan string) {
	var msg = "" //string vazia

	//Recebe a 1a mensagem
	msg = <- strMain
	fmt.Println("Main para o GoRoutine: ", msg)

	//Envia a 1a mensagem
	strGoRoutine <- "Oi Main, bom dia, tudo bem?"

	//Recebe a 2a mensagem
	msg = <- strMain
	fmt.Println("Main para o GoRoutine: ", msg)

	//Envia a 2a mensagem
	strGoRoutine <- "Certo, entendido."
	
	//Finaliza
	fmt.Println("finalizando")

	strGoRoutine <- ""
}

func main() {
	var msg = "" //string vazia

	//cria canais de comunicacao nao-bufferizado
	strMain := make(chan string)
	strGoRoutine := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(strMain, strGoRoutine)

	//Envia a 1a mensagem
	strMain <- "Olá, Goroutine, bom dia!"

	//Recebe a 1a mensagem
	msg = <- strGoRoutine
	fmt.Println("GoRoutine para a Main: ", msg)

	//Envia a 2a mensagem
	strMain <- "Tudo bem! Vou terminar tá?"
	
	//Recebe a 2a mensagem
	msg = <- strGoRoutine
	fmt.Println("GoRoutine para a Main: ", msg)

	<- strGoRoutine

	//Finaliza
	fmt.Println("finalizando")
}