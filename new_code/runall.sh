#!/bin/bash
#$ -cwd
#$ -j y

./classify test_data/wine/wine_training1.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_1.csv output1.txt 1
