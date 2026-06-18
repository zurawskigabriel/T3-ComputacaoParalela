#!/bin/bash

for N in 2048 2953 3915 5058 6435; do
    srun -N 1 -n 1 ./mult "$N"
done
