#!/bin/bash
echo "make 20 MSE(each 1000 expr)"
for i in {1..20};
do
    (./regression coursework2-training.csv)
done
