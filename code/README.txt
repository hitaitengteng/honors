Will Gantt
Last Updated: 3/3/17


To run, type "make", then:

classify <training set file> <test set file>

Note: Both the training and test set files must be in .csv format
and must have headers.

----------------------------------------------------------------------------

You will also notice that there are two R scripts in this directory. A
brief explanation of each:

shuffle_inputs.r: Given a csv file containing inputs, randomizes the order
of those inputs. A lot of times, data will be organized by the class
attribute, which isn't particularly helpful for generating rules. To run,
simply type:

	Rscript shuffle_inputs.r <input file> <output file>

two_fold_split.r: This does the same thing as shuffle_inputs.r, except it
splits the input file into a training set and a test set. To run, type:

	Rscript two_fold_split.r <input file> <training set output file> <test set output file>
