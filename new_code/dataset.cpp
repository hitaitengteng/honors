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
int Dataset::readFromCSVFile(string file_name, int target_class) {

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

				       // update the current attribute
				       curr_att++;
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

				// add the vector to the vector of data points
				data_points_.push_back(curr_vect);

				// if the class attribute value of the current example
				// is the same as the target class, add this example to
				// the vector of examples in the target class
				if (curr_vect.back() == target_class)
					target_examples_.push_back(curr_vect);

				// delete the contents of curr_vect so that it can be
				// used again
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
 * Input:       
 *              file_name: the name of the file to be read in
 * Output:      the number of quantiles
 * Description: reads in the quantiles for all of the attributes in the
 * 		data set with respect to a target class
 * Note:        
 * 		This function expects a csv file of a specific format:
 * 		Specifically, one with no column or row names, where each
 * 		attribute gets its own line, and where the values on each
 * 		line are the quantiles listed in ascending order. Such files
 * 		can be generated from a data set using the R script
 * 		make_quantiles, which can be found in this directory.
 ****************************************************************************/
int Dataset::readQuantiles(string file_name) {

	fstream file_stream;                 // the file stream
	file_stream.open(file_name.c_str()); // try to open the file

	// if opened successfully, start reading
	if (file_stream.is_open() && file_stream.good()) {

		// the line being read in 
		string line;
		
		// an individual word or entry from the line read in
		string token;

		// a string index denoting the end of a token
		int tokenEnd;

		// this function handles .csv files, so the delimiter
		// should always be a comma
		string delimiter = ",";

		// the set of quantiles for the current attribute
		vector<double> curr_quantiles;

		// read until the end of the file is reached, or until
		// some other error flag is set
		while (file_stream.good()) {

			// reset the quantiles vector
			curr_quantiles.clear();

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
				       curr_quantiles.push_back(stof(token));

				}

			        // delete the substring you just created from "line," as
			        // well as the delimiter that follows it
			        line.erase(line.begin(), line.begin() + tokenEnd + 1);
			}

			// add the quantiles for the current attribute to the vector
			// of quantiles for all the attributes (on the last iteration
			// of the while loop, the curr_quantiles vector will be empty,
			// and we don't want to add an empty vector to attribute_quantiles_,
			// hence the need for this check)
			if (!curr_quantiles.empty())
				attribute_quantiles_.push_back(curr_quantiles);
		}

	} else {

		// the file didn't open correctly, so print an error and quit
		cout << stderr << "Error: Unable to open file." << endl;
		exit(0);
	}

	file_stream.close();

	// return the number of quantiles (the quantiles vector also contains
	// the minimum, which is why we have to subtract 1)
	return (attribute_quantiles_[0].size() - 1);

} // end readQuantiles

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Prints important information about the data set.
 ****************************************************************************/ 
void Dataset::printInfo() {

	int i; // counter

	printf("Number of attributes (excluding class attribute): %d\n", num_attributes_);
	printf("Number of classes: %d\n", num_classes_);
	printf("Number of data points: %d\n", num_data_points_);

	printf("Attributes: \n");
	for (i=0; i<num_attributes_; i++)
		printf("%s ", attribute_names_[i].c_str());
	printf("\n");

	if (!attribute_quantiles_.empty()) {
		printf("Attribute Quantiles: \n");
		for (i=0; i<num_attributes_; i++)
			printDataPoint(attribute_quantiles_[i]);
		printf("\n");
	}


} // end print

/****************************************************************************
 * Inputs:      i: The index in the training set of the example to be used.
 * Outputs;     A rule specified according to the values of the example.
 * Description: Generates a rule based on an example from the data set.
 ****************************************************************************/ 
Rule Dataset::createRuleFromExample(int i) {

	// get the example 
	vector<double> example = data_points_[i];

	// create the rule
	Rule r = Rule(num_attributes_);

	// set the rule's class to that of the example
	r.setClass(example.back());
	
	// generate values for the condition's attributes
	for (int i=0; i<num_attributes_; i++) {

		// since we are specifying the rule according to an
		// example, all of its "don't cares" should be false
		r.condition_[i].setDontCare(false);

		// determine the quantile to which the current attribute
		// belongs and set its upper and lower bounds accordingly
		int j=0;
		for (; (attribute_quantiles_[i][j] <= example[i]) && (j < num_quantiles_); j++);
		r.condition_[i].setQuantile(j-1);
		r.condition_[i].setLowerBound(attribute_quantiles_[i][j-1]);
		r.condition_[i].setUpperBound(attribute_quantiles_[i][j]);

	}

	return r;

} // end createRuleFromExample 

/****************************************************************************
 * Inputs:      the example to use
 * Outputs;     A rule specified according to the values of the example.
 * Description: Generates a rule based on an example from the data set.
 ****************************************************************************/ 
Rule Dataset::createRuleFromExample2(vector<double> example) {

	// create the rule
	Rule r = Rule(num_attributes_);

	// set the rule's class to that of the example
	r.setClass(example.back());

	// no don't cares
	r.setNumDontCare(0);
	
	// generate values for the condition's attributes
	for (int i=0; i<num_attributes_; i++) {

		// since we are specifying the rule according to an
		// example, all of its "don't cares" should be false
		r.condition_[i].setDontCare(false);

		// determine the quantile to which the current attribute
		// belongs and set its upper and lower bounds accordingly
		int j=0;
		for (; (attribute_quantiles_[i][j] <= example[i]) && (j < num_quantiles_); j++);
		r.condition_[i].setQuantile(j-1);
		r.condition_[i].setLowerBound(attribute_quantiles_[i][j-1]);
		r.condition_[i].setUpperBound(attribute_quantiles_[i][j]);

	}

	return r;

} // end createRuleFromExample2
