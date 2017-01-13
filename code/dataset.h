#ifndef DATASET_H
#define DATASET_H

#include "attribute.h"
/****************************************************************************
 * File:        dataset.h
 * Author:      Will Gantt
 * Description: Defines the Dataset class. [Description]
 *
 * TODO: 
 *
 * 	change vector implementation to generic types?
 * 	overload operators?
 ****************************************************************************/ 
using namespace std;

class Dataset {

	public:
		// function for reading in data from a csv file
		int readFromCSVFile(string fileName);

		// wrappers for relevant vector functions

		// getters 
		vector<string> getAttributeNames() {return attributeNames;}
		vector<string> getClassNames() {return classNames;}
		vector<pair<double,double>> getAttributeRanges() {return attributeRanges;}
		vector<pair<double,double>> getClassRanges() {return classRanges;}
		vector<vector<double>> getDataPoints() {return dataPoints;}

		int getNumAttributes() {return numAttributes;}
		int getNumClasses() {return numClasses;}
		int getNumDataPoints() {return numDataPoints;}

		// setters

	private:

		// the names of all the attributes
		vector<string> attributeNames;

		// the names for all the classes (i.e. the 
		// values that the class attribute can take)
		vector<string> classNames;

		// for each attribute, the range of 
		// values represented in the data
		vector<pair<double,double>> attributeRanges;

		// the range of values of the class attribute
		// denoted by each class
		vector<pair<double,double>> classRanges;

		// the set of all data points
		vector<vector<double>> dataPoints;

		int numAttributes; // number of attributes in a data point
		int numClasses;    // number of classes
		int numDataPoints; // total number of data points
};

#endif
