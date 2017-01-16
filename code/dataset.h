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
		vector<string> attribute_names_;

		// the names for all the classes (i.e. the 
		// values that the class attribute can take)
		vector<string> class_names_;

		// for each attribute, the range of 
		// values represented in the data
		vector<pair<double,double> > attribute_ranges_;

		// the range of values of the class attribute
		// denoted by each class
		vector<pair<double,double> > class_ranges_;

		// the set of all data points
		vector<vector<double> > data_points_;

	// FUNCTIONS
	
		// default constructor
		Dataset() {}

		// custom constructor
		Dataset(vector<string> attribute_names, vector<string> class_names,
				vector<pair<double,double> > attribute_ranges,
				vector<pair<double,double> > class_ranges,
				vector<vector<double> > data_points) {

			attribute_names_ = attribute_names;
			attribute_ranges_ = attribute_ranges;
			class_names_ = class_names;
			class_ranges_ = class_ranges;
			data_points_ = data_points;

			num_attributes_ = attribute_names.size();
			num_classes_ = class_names.size();
			num_data_points_ = data_points.size();

		}

		// function for reading in data from a csv file
		int readFromCSVFile(string file_name);

		// function for printing all of the data points
		// in the data set
		void print();

		// getters
		int getNumAttributes() const {return num_attributes_;}
		int getNumClasses() const {return num_classes_;}
		int getNumDataPoints() const {return num_data_points_;}

		// setters
		void setNumAttributes(int num_attributes) {num_attributes_ = num_attributes;}
		void setNumClasses(int num_classes) {num_classes_ = num_classes;}
		void setNumDataPoints(int num_data_points) {num_data_points_ = num_data_points;}

	private:

		int num_attributes_;  // number of attributes in a data point
		int num_classes_;     // number of classes
		int num_data_points_; // total number of data points
};

#endif
