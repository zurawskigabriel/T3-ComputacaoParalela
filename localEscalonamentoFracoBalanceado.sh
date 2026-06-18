#!/bin/bash

PROCESSOS=(1 3 7 15 31)
TAMANHOS=(2048 2953 3915 5058 6435)

for I in "${!PROCESSOS[@]}"; do
    P="${PROCESSOS[$I]}"
    N="${TAMANHOS[$I]}"
    mpirun --oversubscribe -np "$P" ./mult_mpi_balanceado "$N"
done
