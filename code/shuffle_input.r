#!/usr/bin/env Rscript

# Will Gantt
# 3/1/17

# A script to read in inputs from a CSV file and
# randomize their order

# The user should specify an input and an output file,
# in that orderf
args = commandArgs(trailingOnly=TRUE)

# Read in the input file
data.set = read.csv(args[1],header=FALSE,sep=",")

# Get the number of inputs
num_inputs = nrow(data.set)

# Create a random vector of unique integers in the range [1,num_inputs].
# The order of the integers in this vector will determine the order
# of the inputs in the output file
indices = sample(1:num_inputs,size=num_inputs,replace=FALSE)

# Create a copy of data set. This will contain the
# reordered inputs
data.set.randomized = data.set

# Reorder the inputs
for (i in 1:num_inputs) {
	data.set.randomized[i,] = data.set[indices[i],]
}	

# Write the reordered inputs to a csv file
write.csv(data.set.randomized, file=args[2], row.names=FALSE)
