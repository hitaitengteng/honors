#!/bin/bash
#$ -cwd
#$ -j y

# Wine dataset, class 0
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-1.txt 0
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-2.txt 0
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-3.txt 0
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training5.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-4.txt 0
./classify test_data/wine/wine_training5.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-5.txt 0
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-6.txt 0
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-7.txt 0
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-8.txt 0
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-9.txt 0
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_0.csv results/wine/wine.0-10.txt 0

# Wine dataset, class 1
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-1.txt 1
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-2.txt 1
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-3.txt 1
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training5.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-4.txt 1
./classify test_data/wine/wine_training5.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-5.txt 1
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-6.txt 1
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-7.txt 1
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-8.txt 1
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-9.txt 1
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_1.csv results/wine/wine.1-10.txt 1

# Wine dataset, class 2
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-1.txt 2
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-2.txt 2
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-3.txt 2
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training5.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-4.txt 2
./classify test_data/wine/wine_training5.csv test_data/wine/wine_training4.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-5.txt 2
./classify test_data/wine/wine_training4.csv test_data/wine/wine_training3.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-6.txt 2
./classify test_data/wine/wine_training3.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-7.txt 2
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-8.txt 2
./classify test_data/wine/wine_training1.csv test_data/wine/wine_training1.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-9.txt 2
./classify test_data/wine/wine_training2.csv test_data/wine/wine_training2.csv test_data/wine/wine_quartiles_class_2.csv results/wine/wine.2-10.txt 2

# Iris dataset, class 0
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-1.txt 0
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-2.txt 0
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-3.txt 0
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-4.txt 0
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-5.txt 0
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-6.txt 0
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-7.txt 0
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-8.txt 0
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-9.txt 0
./classify test_data/iris/iris_testing_1.csv test_data/iris/iris_training_2.csv test_data/iris/iris_quartiles_class0.csv results/iris/iris.0-10.txt 0

# Iris dataset, class 1
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-1.txt 1
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-2.txt 1
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-3.txt 1
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-4.txt 1
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-5.txt 1
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-6.txt 1
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-7.txt 1
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-8.txt 1
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-9.txt 1
./classify test_data/iris/iris_testing_1.csv test_data/iris/iris_training_2.csv test_data/iris/iris_quartiles_class1.csv results/iris/iris.1-10.txt 1

# Iris dataset, class 2
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-1.txt 2
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-2.txt 2
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-3.txt 2
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-4.txt 2
./classify test_data/iris/iris_training_1.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-5.txt 2
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-6.txt 2
./classify test_data/iris/iris_training_2.csv test_data/iris/iris_testing_3.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-7.txt 2
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_1.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-8.txt 2
./classify test_data/iris/iris_training_3.csv test_data/iris/iris_testing_2.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-9.txt 2
./classify test_data/iris/iris_testing_1.csv test_data/iris/iris_training_2.csv test_data/iris/iris_quartiles_class2.csv results/iris/iris.2-10.txt 2
