#!/bin/bash

set -e

N="${1:-2048}"
OUT="Benchmark/local_forte_${N}.csv"

g++ -O2 -std=c++17 -o mult mult.cpp
mpic++.openmpi -O2 -std=c++17 -o mult_mpi mult_mpi.cpp
mpic++.openmpi -O2 -std=c++17 -o mult_mpi_balanceado mult_mpi_balanceado.cpp

echo "versao,processos,tamanho,tempo_segundos" > "$OUT"

tempo=$(./mult "$N" | grep "Tempo de exec" | grep -oE "[0-9]+\\.[0-9]+")
echo "sequencial,1,$N,$tempo" >> "$OUT"

for p in 1 3 7 15 31; do
    tempo=$(mpirun.openmpi --oversubscribe -np "$p" ./mult_mpi "$N" |
        grep "Tempo de exec" |
        grep -oE "[0-9]+\\.[0-9]+")
    echo "dc_simples,$p,$N,$tempo" >> "$OUT"
done

for p in 1 3 7 15 31; do
    tempo=$(mpirun.openmpi --oversubscribe -np "$p" ./mult_mpi_balanceado "$N" |
        grep "Tempo de exec" |
        grep -oE "[0-9]+\\.[0-9]+")
    echo "dc_balanceado,$p,$N,$tempo" >> "$OUT"
done

cat "$OUT"
