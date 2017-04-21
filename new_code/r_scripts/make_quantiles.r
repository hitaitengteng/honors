#!/usr/bin/env Rscript

# Will Gantt
# 3/21/17

# A script to determine quantiles for all attributes

# The user must specify the following:
# 	1. an input file in CSV format, containing
#	   the data to be quantized.
#	2. an output file.
#	3. the desired number of quantiles.
#	4. the target class.

# NOTE: To switch between Mg/ha and Mkcal/ha, you have
#       to change the argument to the 'which' function below

args = commandArgs(trailingOnly=TRUE)

# For debugging
options(warn=1)

# Exit if the user doesn't specify enough arguments
stopifnot(length(args) >= 4)

# Read in the input file
data.set = read.csv(args[1],header=TRUE,sep=",")

# Pare the data set down to only those examples whose
# class attribute value matches the target class
target.class = as.integer(args[4])
data.set = data.set[which(data.set$tHA_3==target.class),]

# Get the number of quantiles
num_quantiles = as.integer(args[3])

# Create the probability vector for the quantiles
quantiles = rep(0,num_quantiles)
for (i in 1:num_quantiles) {
	quantiles[i] = i/num_quantiles
}

# Get the number of attributes (including class attribute)
num_attributes = ncol(data.set)

# For each attribute, compute the quantiles. Create a matrix where
# each row is an attribute and each column is a quantile value (except
# the first column, which is the minimum)
attribute_quantiles = matrix(data=NA,num_attributes,num_quantiles+1)
for (i in 1:num_attributes) {
	attribute_quantiles[i,] = c(min(data.set[,i]),quantile(data.set[,i],probs=quantiles,names=FALSE,type=7))
}

# Write attribute_quantiles to a file
write.table(attribute_quantiles,args[2],sep=",",row.names=FALSE,col.names=FALSE)
