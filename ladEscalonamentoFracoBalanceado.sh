#!/bin/bash

PROCESSOS=(1 3 7 15 31)
#TAMANHOS=(2048 2953 3915 5058 6435)
TAMANHOS=(2048 2580 3250 4096 5160)

for I in "${!PROCESSOS[@]}"; do
    P="${PROCESSOS[$I]}"
    N="${TAMANHOS[$I]}"
    NODOS=2
    if [ "$P" -eq 1 ]; then
        NODOS=1
    fi

    srun -N "$NODOS" -n "$P" ./mult_mpi_balanceado "$N"
done
