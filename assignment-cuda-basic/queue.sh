qsub -q mamba -d $(pwd) -l nodes=1:ppn=7:gpus=1 polinomial.sh
