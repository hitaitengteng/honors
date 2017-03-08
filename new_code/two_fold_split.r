#!/usr/bin/env Rscript

# Will Gantt
# 3/1/17

# A script to divide a data set (in csv format) into
# a training set and a test set

# The user should specify an input, an output file for the
# training set, and an output file for the testing set
args = commandArgs(trailingOnly=TRUE)

# Exit if the user doesn't specify enough arguments
stopifnot(length(args) >= 3)

# Read in the input file
data.set = read.csv(args[1],header=TRUE,sep=",")

# Get the number of inputs
num_inputs = nrow(data.set)

# Create a random vector of unique integers in the range [1,num_inputs].
# The order of the integers in this vector will determine the order
# of the inputs in the output file
indices = sample(1:num_inputs,size=num_inputs,replace=FALSE)

# Create the training and test sets (Note that if there are
# an odd number of inputs, one of those inputs will be ommitted
# from both sets
training.set = data.set[2:(num_inputs/2),]
test.set = data.set[1:(num_inputs/2),]

# Reorder the inputs
for (i in 1:num_inputs/2) {
	training.set[i,] = data.set[indices[i],]
	test.set[i,] = data.set[indices[i+(num_inputs/2)],]
}	

# Write the training and testing sets to their corresponding
# output files
write.csv(training.set, file=args[2], quote=FALSE, row.names=FALSE)
write.csv(test.set, file=args[3], quote=FALSE, row.names=FALSE)

