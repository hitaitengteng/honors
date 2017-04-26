#!/bin/bash
#$ -cwd
#$ -j y

# Wine dataset, class 0
./classify new_wine_and_iris_data/wine/wine.train.1.csv new_wine_and_iris_data/wine/wine.test.1.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-1.txt 0
./classify new_wine_and_iris_data/wine/wine.train.2.csv new_wine_and_iris_data/wine/wine.test.2.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-2.txt 0
./classify new_wine_and_iris_data/wine/wine.train.3.csv new_wine_and_iris_data/wine/wine.test.3.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-3.txt 0
./classify new_wine_and_iris_data/wine/wine.train.4.csv new_wine_and_iris_data/wine/wine.test.4.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-4.txt 0
./classify new_wine_and_iris_data/wine/wine.train.5.csv new_wine_and_iris_data/wine/wine.test.5.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-5.txt 0
./classify new_wine_and_iris_data/wine/wine.train.6.csv new_wine_and_iris_data/wine/wine.test.6.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-6.txt 0
./classify new_wine_and_iris_data/wine/wine.train.7.csv new_wine_and_iris_data/wine/wine.test.7.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-7.txt 0
./classify new_wine_and_iris_data/wine/wine.train.8.csv new_wine_and_iris_data/wine/wine.test.8.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-8.txt 0
./classify new_wine_and_iris_data/wine/wine.train.9.csv new_wine_and_iris_data/wine/wine.test.9.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-9.txt 0
./classify new_wine_and_iris_data/wine/wine.train.10.csv new_wine_and_iris_data/wine/wine.test.10.csv test_data/wine/wine_quartiles_class_0.csv new_results/wine/wine.0-10.txt 0

# Wine dataset, class 1
./classify new_wine_and_iris_data/wine/wine.train.1.csv new_wine_and_iris_data/wine/wine.test.1.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-1.txt 1
./classify new_wine_and_iris_data/wine/wine.train.2.csv new_wine_and_iris_data/wine/wine.test.2.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-2.txt 1
./classify new_wine_and_iris_data/wine/wine.train.3.csv new_wine_and_iris_data/wine/wine.test.3.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-3.txt 1
./classify new_wine_and_iris_data/wine/wine.train.4.csv new_wine_and_iris_data/wine/wine.test.4.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-4.txt 1
./classify new_wine_and_iris_data/wine/wine.train.5.csv new_wine_and_iris_data/wine/wine.test.5.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-5.txt 1
./classify new_wine_and_iris_data/wine/wine.train.6.csv new_wine_and_iris_data/wine/wine.test.6.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-6.txt 1
./classify new_wine_and_iris_data/wine/wine.train.7.csv new_wine_and_iris_data/wine/wine.test.7.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-7.txt 1
./classify new_wine_and_iris_data/wine/wine.train.8.csv new_wine_and_iris_data/wine/wine.test.8.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-8.txt 1
./classify new_wine_and_iris_data/wine/wine.train.9.csv new_wine_and_iris_data/wine/wine.test.9.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-9.txt 1
./classify new_wine_and_iris_data/wine/wine.train.10.csv new_wine_and_iris_data/wine/wine.test.10.csv test_data/wine/wine_quartiles_class_1.csv new_results/wine/wine.0-10.txt 1

# Wine dataset, class 2
./classify new_wine_and_iris_data/wine/wine.train.1.csv new_wine_and_iris_data/wine/wine.test.1.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-1.txt 2
./classify new_wine_and_iris_data/wine/wine.train.2.csv new_wine_and_iris_data/wine/wine.test.2.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-2.txt 2
./classify new_wine_and_iris_data/wine/wine.train.3.csv new_wine_and_iris_data/wine/wine.test.3.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-3.txt 2
./classify new_wine_and_iris_data/wine/wine.train.4.csv new_wine_and_iris_data/wine/wine.test.4.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-4.txt 2
./classify new_wine_and_iris_data/wine/wine.train.5.csv new_wine_and_iris_data/wine/wine.test.5.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-5.txt 2
./classify new_wine_and_iris_data/wine/wine.train.6.csv new_wine_and_iris_data/wine/wine.test.6.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-6.txt 2
./classify new_wine_and_iris_data/wine/wine.train.7.csv new_wine_and_iris_data/wine/wine.test.7.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-7.txt 2
./classify new_wine_and_iris_data/wine/wine.train.8.csv new_wine_and_iris_data/wine/wine.test.8.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-8.txt 2
./classify new_wine_and_iris_data/wine/wine.train.9.csv new_wine_and_iris_data/wine/wine.test.9.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-9.txt 2
./classify new_wine_and_iris_data/wine/wine.train.10.csv new_wine_and_iris_data/wine/wine.test.10.csv test_data/wine/wine_quartiles_class_2.csv new_results/wine/wine.0-10.txt 2

# iris dataset, class 0
./classify new_wine_and_iris_data/iris/iris.train.1.csv new_wine_and_iris_data/iris/iris.test.1.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-1.txt 0
./classify new_wine_and_iris_data/iris/iris.train.2.csv new_wine_and_iris_data/iris/iris.test.2.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-2.txt 0
./classify new_wine_and_iris_data/iris/iris.train.3.csv new_wine_and_iris_data/iris/iris.test.3.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-3.txt 0
./classify new_wine_and_iris_data/iris/iris.train.4.csv new_wine_and_iris_data/iris/iris.test.4.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-4.txt 0
./classify new_wine_and_iris_data/iris/iris.train.5.csv new_wine_and_iris_data/iris/iris.test.5.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-5.txt 0
./classify new_wine_and_iris_data/iris/iris.train.6.csv new_wine_and_iris_data/iris/iris.test.6.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-6.txt 0
./classify new_wine_and_iris_data/iris/iris.train.7.csv new_wine_and_iris_data/iris/iris.test.7.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-7.txt 0
./classify new_wine_and_iris_data/iris/iris.train.8.csv new_wine_and_iris_data/iris/iris.test.8.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-8.txt 0
./classify new_wine_and_iris_data/iris/iris.train.9.csv new_wine_and_iris_data/iris/iris.test.9.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-9.txt 0
./classify new_wine_and_iris_data/iris/iris.train.10.csv new_wine_and_iris_data/iris/iris.test.10.csv test_data/iris/iris_quartiles_class0.csv new_results/iris/iris.0-10.txt 0

# iris dataset, class 1
./classify new_wine_and_iris_data/iris/iris.train.1.csv new_wine_and_iris_data/iris/iris.test.1.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-1.txt 1
./classify new_wine_and_iris_data/iris/iris.train.2.csv new_wine_and_iris_data/iris/iris.test.2.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-2.txt 1
./classify new_wine_and_iris_data/iris/iris.train.3.csv new_wine_and_iris_data/iris/iris.test.3.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-3.txt 1
./classify new_wine_and_iris_data/iris/iris.train.4.csv new_wine_and_iris_data/iris/iris.test.4.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-4.txt 1
./classify new_wine_and_iris_data/iris/iris.train.5.csv new_wine_and_iris_data/iris/iris.test.5.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-5.txt 1
./classify new_wine_and_iris_data/iris/iris.train.6.csv new_wine_and_iris_data/iris/iris.test.6.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-6.txt 1
./classify new_wine_and_iris_data/iris/iris.train.7.csv new_wine_and_iris_data/iris/iris.test.7.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-7.txt 1
./classify new_wine_and_iris_data/iris/iris.train.8.csv new_wine_and_iris_data/iris/iris.test.8.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-8.txt 1
./classify new_wine_and_iris_data/iris/iris.train.9.csv new_wine_and_iris_data/iris/iris.test.9.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-9.txt 1
./classify new_wine_and_iris_data/iris/iris.train.10.csv new_wine_and_iris_data/iris/iris.test.10.csv test_data/iris/iris_quartiles_class1.csv new_results/iris/iris.0-10.txt 1

# iris dataset, class 2
./classify new_wine_and_iris_data/iris/iris.train.1.csv new_wine_and_iris_data/iris/iris.test.1.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-1.txt 2
./classify new_wine_and_iris_data/iris/iris.train.2.csv new_wine_and_iris_data/iris/iris.test.2.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-2.txt 2
./classify new_wine_and_iris_data/iris/iris.train.3.csv new_wine_and_iris_data/iris/iris.test.3.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-3.txt 2
./classify new_wine_and_iris_data/iris/iris.train.4.csv new_wine_and_iris_data/iris/iris.test.4.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-4.txt 2
./classify new_wine_and_iris_data/iris/iris.train.5.csv new_wine_and_iris_data/iris/iris.test.5.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-5.txt 2
./classify new_wine_and_iris_data/iris/iris.train.6.csv new_wine_and_iris_data/iris/iris.test.6.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-6.txt 2
./classify new_wine_and_iris_data/iris/iris.train.7.csv new_wine_and_iris_data/iris/iris.test.7.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-7.txt 2
./classify new_wine_and_iris_data/iris/iris.train.8.csv new_wine_and_iris_data/iris/iris.test.8.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-8.txt 2
./classify new_wine_and_iris_data/iris/iris.train.9.csv new_wine_and_iris_data/iris/iris.test.9.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-9.txt 2
./classify new_wine_and_iris_data/iris/iris.train.10.csv new_wine_and_iris_data/iris/iris.test.10.csv test_data/iris/iris_quartiles_class2.csv new_results/iris/iris.0-10.txt 2
