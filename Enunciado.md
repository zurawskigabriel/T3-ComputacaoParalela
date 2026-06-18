# Enunciado do Trabalho 3

## Objetivo

Implementar, usando MPI, uma versao paralela seguindo o modelo de divisao e
conquista de um programa adequado para esse modelo.

Neste projeto, o problema escolhido continua sendo a multiplicacao de matrizes do
trabalho anterior. A adaptacao para D&C divide a matriz A em blocos de linhas,
distribui esses blocos pela arvore binaria de processos e recompoe a matriz C
respeitando a ordem original das linhas.

## Modelo divisao e conquista

O processo raiz recebe o problema inteiro. Enquanto houver filhos na arvore, o
processo divide seu bloco em duas partes e envia cada metade para um filho. As
folhas conquistam o problema calculando localmente o bloco de C. Em seguida, os
resultados sobem pela arvore ate o processo raiz.

A segunda versao (`mult_mpi_balanceado.cpp`) distribui as linhas de acordo com o
numero de folhas em cada subarvore, permitindo comparar o desempenho com um
mecanismo simples de balanceamento de carga.

## Itens para avaliacao

- Execucao da versao sequencial.
- Implementacao da versao paralela SPMD com MPI seguindo divisao e conquista.
- Medicao dos tempos para a versao sequencial e para a versao paralela no cluster
  Atlantica.
- Execucao com 1, 3, 7, 15 e 31 processos.
- Calculo de speed-up e eficiencia.
- Analise do balanceamento da carga nos diferentes niveis da arvore.
- Analise do ganho obtido com HT.
- Analise do ganho obtido com a versao balanceada.
- Clareza do codigo e relatorio em PDF.
