# Programação Concorrente
## Laboratório 6: 
  <p>Nas quatro funções implementadas, fazemos com que a escrita tenha prioridade sobre a leitura, não permitindo que novas leituras comecem enquanto uma escrita é realizada.</p>
    <p>read_lock: a função faz a entrada na leitura, mas somente se o contador de escritores for igual a 0. Depois incrementa o contador de leitores.</p>
    <p>read_unlock: a função faz a saída da leitura, decrementando o contador de leitores. Do jeito que está implementada, não há restrição sobre 2 ou mais leitores ao mesmo tempo.</p>
    <p>write_lock: a função faz a entrada na escrita, se não houver escrita sendo realizada. Depois incrementa o contador de escritores.</p>
    <p>write_unlock: a função faz a saída da escrita, decrementando o contador de escritores. Depois libera outra escrita e todas as leituras no aguardo.</p>
