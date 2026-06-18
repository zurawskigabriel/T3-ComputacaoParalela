#!/bin/bash

if [ "$#" -lt 2 ]; then
    echo "Uso: ./benchmark.sh <tamanho_min> <tamanho_max> [passo]"
    echo "Exemplo: ./benchmark.sh 128 1024 128"
    exit 1
fi

MIN=$1
MAX=$2
PASSO=${3:-128}   # passo padrão: 128
SAIDA="resultados.txt"

echo "Benchmark: matrizes de $MIN a $MAX (passo $PASSO)"
echo "Resultados salvos em: $SAIDA"
echo ""

# Cabeçalho do arquivo de saída
echo "tamanho,tempo_segundos" > "$SAIDA"

for N in $(seq "$MIN" "$PASSO" "$MAX"); do
    echo -n "Executando N=$N ... "

    # Captura apenas a linha do tempo da saída do programa
    TEMPO=$(./mult "$N" | grep "Tempo de execução" | grep -oP '[0-9]+\.[0-9]+')

    echo "$N,$TEMPO" >> "$SAIDA"
    echo "tempo: ${TEMPO}s"
done

echo ""
echo "Concluído."
