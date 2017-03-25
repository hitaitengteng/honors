#include "dataset.h"

/****************************************************************************
 * File:        dataset.cpp
 * Author:      William Gantt
 * Description: Implementations of various functions for the Dataset class.
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Input:       
 *              file_name: the name of the file to be read in
 * Output:      the number of data points successfully read in
 * Description: reads in vectors of attributes from a text file
 ****************************************************************************/
int Dataset::readFromCSVFile(string file_name) {

	fstream file_stream;                 // the file stream
	file_stream.open(file_name.c_str()); // try to open the file

	// a hashmap for keeping track of the number of classes
	unordered_map<int,int> classes;       

	// holds the class of the current data instance
	int curr_class = NO_CLASS;            

	// if it opened successfully, start reading
	if (file_stream.is_open() && file_stream.good()) {

		// the current line being read in
            	string line; 

		// get the first line of the file (this tells us the
		// names of the attributes)
		getline(file_stream, line);

		// an individual word or entry from the line read in
		string token;

		// a string index denoting the end of a token
		int tokenEnd;

		// this function handles .csv files, so the delimiter
		// should always be a comma
		string delimiter = ",";

		// appending a comma to the end of the line makes the
		// parsing a little bit easier, as you'll see below
		line.append(delimiter);

		do {
			// find the next delimiting character
			tokenEnd = line.find(delimiter);

			// get the substring from the beginning of the line
			// up to, but not including, the delimiter
			token = line.substr(0, tokenEnd);

			// add this string to the vector of all attribute names
			attribute_names_.push_back(token);

			// delete the substring you just created from "line," as
			// well as the delimiter that follows it
			line.erase(line.begin(), line.begin() + tokenEnd + 1);

		  // repeat so long as we have not removed
		  // all of the characters from the string
		} while (line.size() > 0);

		attribute_names_.pop_back();

		// set the member variable containing the number of attributes
		num_attributes_ = attribute_names_.size();

		// initialize the vector containing the range of values for
		// each attribute (except the class attribute)
		for (int i=0; i<num_attributes_; i++)
			attribute_ranges_.push_back(make_pair(FLT_MAX,FLT_MIN));

		// a vector for storing attribute values
		vector<double> curr_vect;

		// an iterator for attributes
		int curr_att;

		// read until the end of the file is reached, or until
		// some other error flag is set
		while (file_stream.good()) {

			// reset the attribute iterator
			curr_att = 0;

			// get the current line
			getline(file_stream, line);

			// add a comma to the end of the line (this makes parsing easier,
			// as mentioned above)
			line.append(delimiter);

			// as attribute values are extracted from a line, they are deleted
			// from the string. Thus, we want to move onto the next line when
			// the line size reaches 0.
			for (int j = 0; line.size() > 0; j++) {

				// find the next delimiting character
			        tokenEnd = line.find(delimiter);

			        // get the substring from the beginning of the line
			        // up to, but not including, the delimiter
			        token = line.substr(0, tokenEnd);

				// we have to have this check here because otherwise the program
				// will throw an exception when it reaches the end of the file.
				if (token.compare("") != 0) {

					// add the attribute value to the current data point vector
				       curr_vect.push_back(stof(token));

				       // update the maximum and minimum values of this attribute across
				       // all data points, if necessary
				       if (curr_att < num_attributes_) {
					       if (curr_vect.back() < attribute_ranges_[curr_att].first)
						       attribute_ranges_[curr_att].first = curr_vect.back();
					       if (curr_vect.back() > attribute_ranges_[curr_att].second)
						       attribute_ranges_[curr_att].second = curr_vect.back();
				       }
				       curr_att++;  // update the current attribute
				}

			        // delete the substring you just created from "line," as
			        // well as the delimiter that follows it
			        line.erase(line.begin(), line.begin() + tokenEnd + 1);

			} // end for

			// CHECK FOR CLASSES
			if (file_stream.good()) {

				// get the class of the current data instance
				curr_class = (int) curr_vect.back();
			
				// if the class is not already in the hashmap, add it
				// and update the number of classes
				if (classes.find(curr_class) == classes.end()) {
					classes.emplace(curr_class,curr_class);
					num_classes_++;
				}

				// add the vector to the vector of data points, and then
				// delete its contents so that it can be used again
				data_points_.push_back(curr_vect);
				curr_vect.clear();

				// update the number of data points
				num_data_points_++;

			} // end if 
		} // end while
	} else {

		// the file didn't open correctly, so print an error and quit
		cout << stderr << "Error: Unable to open file." << endl;
		exit(0); 
	}

	file_stream.close();        // close the file stream
	return num_data_points_;    // return the number of vectors read in

} // end readFromCsvFile

/****************************************************************************
 * Inputs:
 * Outputs;
 * Description:
 ****************************************************************************/ 
void Dataset::printDataset() {

	int i; // counter

	printf("Number of attributes (excluding class attribute): %d\n", num_attributes_);
	printf("Number of classes: %d\n", num_classes_);
	printf("Number of data points: %d\n", num_data_points_);

	printf("Attributes: \n");
	for (i=0; i<num_attributes_; i++)
		printf("%s ", attribute_names_[i].c_str());
	printf("\n");

	for (i=0; i<num_data_points_; i++)
		printDataPoint(data_points_[i], num_attributes_);

} // end print

/****************************************************************************
 * Inputs:
 * Outputs;
 * Description:
 ****************************************************************************/ 
Dataset Dataset::random(int num_data_points) {

	// all ranges for randomly generated data are [0,1]
	pair<double,double> range_pair = make_pair(0,1);

	// create the vector of attribute names and ranges
	vector<string> attribute_names;
	vector< pair<double,double> > attribute_ranges;
	for (int i=0; i<NUM_TEST_NAMES; i++) {
		attribute_names.push_back(TEST_NAMES[i]);
		attribute_ranges.push_back(range_pair);
	}

	// create the vector of class names
	vector<string> class_names;
	for (int i=0; i<NUM_CLASSES; i++) {
		class_names.push_back(CLASS_NAMES[i]);
	}

	// create the vector of class ranges
	vector<pair<double,double> > class_ranges;
	class_ranges.push_back(make_pair(0,0));
	class_ranges.push_back(make_pair(0,0.25));
	class_ranges.push_back(make_pair(0.25,0.5));
	class_ranges.push_back(make_pair(0.5,0.75));
	class_ranges.push_back(make_pair(0.75,1));

	// create the vector of data points
	vector< vector<double> > data_points;
	for (int i=0; i<num_data_points; i++)
		data_points.push_back(randomDataPoint(NUM_TEST_NAMES));

	Dataset d = Dataset(attribute_names, class_names, attribute_ranges,
			class_ranges, data_points);
	return d;

} // end random

/****************************************************************************
 * Inputs:      The index in the training set of the data point to be used.
 * Outputs;     A rule specified according to the values of the input.
 * Description: Generates a rule based on a data point from the training set.
 ****************************************************************************/ 
Rule Dataset::createRuleFromDataPoint(int i, double range_scalar) {

	// get the input vector and the number of attributes it contains
	vector<double> input = data_points_[i];
	int num_attributes = input.size() - 1;

	// create the rule
	Rule r = Rule(num_attributes);

	// set the rule's class to that of the input
	r.setClass(input.back());
	
	// generate values for the condition's attributes
	double curr_att_range = 0;
	double spread = 0;
	for (int i=0; i<num_attributes; i++) {

		// get the possible range of values for the current attribute
		curr_att_range = attribute_ranges_[i].second - attribute_ranges_[i].first;

		// scale it by a factor range_scalar
		spread = curr_att_range * range_scalar;

		// set the values of the attribute
		r.condition_[i].setCenter(input[i]);
		r.condition_[i].setDontCare(false);
		r.condition_[i].setSpread(spread);
	}

	return r;

} // end createRuleFromDataPoint

/****************************************************************************
 * Inputs:
 * Outputs;
 * Description:
 ****************************************************************************/ 
vector<double> Dataset::randomDataPoint(int num_attributes) {

	// generate random values for the attributes
	vector<double> data_point;
	for (int i=0; i<num_attributes; i++) 
		data_point.push_back(real_dist(rng));
 
	// add a random class
	data_point.push_back(int_dist(rng) % NUM_CLASSES);

	return data_point;

} // end randomDataPoint
