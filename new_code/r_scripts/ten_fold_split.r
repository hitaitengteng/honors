#!/usr/bin/env Rscript

# Will Gantt
# 3/1/17

# A script to divide a data set (in csv format) into
# a training set and a test set

# for five-fold validation
require(caret)

# The user should specify an input, an output file for the
# training set, and an output file for the testing set
args = commandArgs(trailingOnly=TRUE)

# Exit if the user doesn't specify enough arguments
stopifnot(length(args) >= 2)

# Read in the input file
data.set = read.csv(args[1],header=TRUE,sep=",")
root = args[2]

num_inputs = nrow(data.set)
indices = sample(1:num_inputs,size=num_inputs,replace=FALSE)
folds = createFolds(indices,k=10,list = TRUE, returnTrain = FALSE)

test1 = data.set[indices[folds[[1]]],]
test2 = data.set[indices[folds[[2]]],]
test3 = data.set[indices[folds[[3]]],]
test4 = data.set[indices[folds[[4]]],]
test5 = data.set[indices[folds[[5]]],]
test6 = data.set[indices[folds[[6]]],]
test7 = data.set[indices[folds[[7]]],]
test8 = data.set[indices[folds[[8]]],]
test9 = data.set[indices[folds[[9]]],]
test10 = data.set[indices[folds[[10]]],]

train1 = data.set[setdiff(indices,indices[folds[[1]]]),]
train2 = data.set[setdiff(indices,indices[folds[[2]]]),]
train3 = data.set[setdiff(indices,indices[folds[[3]]]),]
train4 = data.set[setdiff(indices,indices[folds[[4]]]),]
train5 = data.set[setdiff(indices,indices[folds[[5]]]),]
train6 = data.set[setdiff(indices,indices[folds[[6]]]),]
train7 = data.set[setdiff(indices,indices[folds[[7]]]),]
train8 = data.set[setdiff(indices,indices[folds[[8]]]),]
train9 = data.set[setdiff(indices,indices[folds[[9]]]),]
train10 = data.set[setdiff(indices,indices[folds[[10]]]),]

for (i in 1:10) {
	
	testing.set = data.set[indices[folds[[i]]],]
	training.set = data.set[setdiff(indices,indices[folds[[i]]]),]

	training.name = paste(root,"train",i,"csv",sep=".")
	testing.name = paste(root,"test",i,"csv",sep=".")

	write.csv(training.set,file=training.name,quote=FALSE,row.names=FALSE)
	write.csv(testing.set,file=testing.name,quote=FALSE,row.names=FALSE)

}
