#ifndef DATASET_H
#define DATASET_H

#include "attribute.h"
/****************************************************************************
 * File:        dataset.h
 * Author:      Will Gantt
 * Description: Defines the Dataset class. [Description]
 *
 * TODO: 
 * 	- Ideally, the vector member variables would be private, but
 * 	  wrapper functions are such a pain to write.
 ****************************************************************************/ 
using namespace std;

class Dataset {

	public:

	// MEMBER VARIABLES
	
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

	// FUNCTIONS
	
		// default constructor
		Dataset() {}

		// custom constructor
		Dataset(vector<string> attributeNames, vector<string> classNames,
				vector<pair<double,double>> attributeRanges,
				vector<pair<double,double>> classRanges,
				vector<vector<double>> dataPoints) {

			this->attributeNames = attributeNames;
			this->classNames = classNames;
			this->classRanges = classRanges;
			this->dataPoints = dataPoints;

			numAttributes = attributeNames.size();
			numClasses = classNames.size();
			numDataPoints = dataPoints.size();

		}

		// function for reading in data from a csv file
		int readFromCSVFile(string fileName);

		// getters
		int getNumAttributes() {return numAttributes;}
		int getNumClasses() {return numClasses;}
		int getNumDataPoints() {return numDataPoints;}

		// setters
		void setNumAttributes(int numAttributes) {this->numAttributes = numAttributes;}
		void setNumClasses(int numClasses) {this->numClasses = numClasses;}
		void setNumDataPoints(int numDataPoints) {this->numDataPoints = numDataPoints;}

	private:

		int numAttributes; // number of attributes in a data point
		int numClasses;    // number of classes
		int numDataPoints; // total number of data points
};

#endif
