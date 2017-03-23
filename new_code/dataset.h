#ifndef DATASET_H
#define DATASET_H

#include "rule.h"
/****************************************************************************
 * File:        dataset.h
 * Author:      Will Gantt
 * Description: Defines the Dataset class. [Description]
 *
 * TODO: 
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

		// the quantiles of each attribute, computed only from
		// examples from the target class
		std::vector<std::vector<double> > attribute_quantiles_;

		// the set of all data points
		std::vector<std::vector<double> > data_points_;

		// keeps track of which data points have already been covered by
		// a rule
		bool *examples_covered_;

	// FUNCTIONS
	
		// default constructor
		Dataset() : num_attributes_(0), num_classes_(0), num_data_points_(0) { }

		// custom constructor
		Dataset(std::vector<std::string> attribute_names, 
				std::vector<std::string> class_names,
				std::vector<std::vector<double> > data_points) {

			attribute_names_ = attribute_names;
			class_names_ = class_names;
			data_points_ = data_points;

			num_attributes_ = attribute_names.size();
			num_classes_ = class_names.size();
			num_data_points_ = data_points.size();
			num_quantiles_ = attribute_quantiles_[0].size(); 
			examples_covered_ = new bool[num_data_points_];
			for (int i=0; i<num_data_points_; i++)
				examples_covered_[i] = false;

		}

		// copy constructor
		Dataset(const Dataset &d) {

			attribute_names_ = d.attribute_names_;
			class_names_ = d.class_names_;
			data_points_ = d.data_points_;
			attribute_quantiles_ = d.attribute_quantiles_;

			num_attributes_ = d.num_attributes();
			num_classes_ = d.num_classes();
			num_data_points_ = d.num_data_points();
			num_quantiles_ = d.num_quantiles();

			examples_covered_ = new bool[num_data_points_];
			if (d.examples_covered_) {
				for (int i=0; i<num_data_points_; i++)
					examples_covered_[i] = d.examples_covered_[i];
			}
		}

		// destructor
		~Dataset() {
			// delete examples_covered_;
		}

		// function for reading in data from a csv file
		int readFromCSVFile(std::string file_name);

		// function for reading in quantile values
		int readQuantiles(std::string file_name);

		// function for printing all of the examples in the data set
		void printInfo();

		// prints a single data point;;
		static void printDataPoint(std::vector<double> data_point) {
			int num_attributes = data_point.size() - 1;
			for (int i=0; i<num_attributes; i++)
				printf("%.3f ", data_point[i]);
			printf("\n");
		}

		// indicates whether there are any examples in the training set
		bool empty() {return data_points_.empty();}

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
		int num_quantiles() const {return num_quantiles_;}

		// setters
		void setNumAttributes(int num_attributes) {num_attributes_ = num_attributes;}
		void setNumClasses(int num_classes) {num_classes_ = num_classes;}
		void setNumDataPoints(int num_data_points) {num_data_points_ = num_data_points;}
		void setNumQuantiles(int num_quantiles) {num_quantiles_ = num_quantiles;}

	private:

		int num_attributes_;  // number of attributes in a data point
		int num_classes_;     // number of classes
		int num_data_points_; // total number of data points
		int num_quantiles_;   // the number of quantiles into which the data are divided
};

#endif
