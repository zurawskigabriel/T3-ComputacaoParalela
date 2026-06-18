#!/bin/bash

N=2048

for P in 1 3 7 15 31; do
    NODOS=2
    if [ "$P" -eq 1 ]; then
        NODOS=1
    fi

    srun -N "$NODOS" -n "$P" ./mult_mpi "$N"
done
