#ifndef DATASET_H
#define DATASET_H

#include "attribute.h"
#include "rule.h"
/****************************************************************************
 * File:        dataset.h
 * Author:      Will Gantt
 * Description: Defines the Dataset class. [Description]
 *
 * TODO: 
 * 	- Ideally, the vector member variables would be private, but
 * 	  wrapper functions are such a pain to write.
 * 	- Fix printDataPoint so that the number of attributes are not
 *  	  required as a parameter.
 ****************************************************************************/ 

class Dataset {

	public:

	// MEMBER VARIABLES
	
		// the names of all the attributes
		std::vector<std::string> attribute_names_;

		// the names for all the classes (i.e. the values that
		// the class attribute can take). NOTE: this is currently
		// not used. 
		std::vector<std::string> class_names_;

		// for each attribute, the range of 
		// values represented in the data
		std::vector<std::pair<double,double> > attribute_ranges_;

		// the range of values of the class attribute
		// denoted by each class (not always relevant)
		std::vector<std::pair<double,double> > class_ranges_;

		// the set of all data points
		std::vector<std::vector<double> > data_points_;

	// FUNCTIONS
	
		// default constructor
		Dataset() : num_attributes_(0), num_classes_(0), num_data_points_(0) { }

		// custom constructor
		Dataset(std::vector<std::string> attribute_names, 
				std::vector<std::string> class_names,
				std::vector<std::pair<double,double> > attribute_ranges,
				std::vector<std::pair<double,double> > class_ranges,
				std::vector<std::vector<double> > data_points) {

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
		int readFromCSVFile(std::string file_name);

		// function for printing all of the data points
		// in the data set
		void printDataset();

		// prints a single data point
		static void printDataPoint(std::vector<double> data_point, int num_attributes) {
			for (int i=0; i<num_attributes; i++)
				printf("%.3f ", data_point[i]);
			printf("\n");
		}

		// create a random data set
		static Dataset random(int num_data_points);

		// create a random data point
		static std::vector<double> randomDataPoint(int num_attributes);

		// create a rule from a data point
		Rule createRuleFromDataPoint(int i, double range_scalar);

		// getters
		int num_attributes() const {return num_attributes_;}
		int num_classes() const {return num_classes_;}
		int num_data_points() const {return num_data_points_;}

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
