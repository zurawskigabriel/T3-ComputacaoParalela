#!/bin/bash

N=2048

for P in 63 127 255 511 1023; do
    NODOS=2
    if [ "$P" -eq 1 ]; then
        NODOS=1
    fi

    srun --oversubscribe -N "$NODOS" -n "$P" ./mult_mpi_balanceado "$N"
done