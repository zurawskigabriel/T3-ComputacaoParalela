#!/bin/bash

N=2048

for P in 1 3 7 15 31; do
    mpirun --oversubscribe -np "$P" ./mult_mpi_balanceado "$N"
done
