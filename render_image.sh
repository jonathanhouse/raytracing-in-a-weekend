#!/bin/bash

program_name=$1
figure_name="figures/$1.ppm"

make clean
make ${program_name}
./${program_name} > ${figure_name}
open ${figure_name}