#include "dataset.h"

/****************************************************************************
 * File:        dataset.cpp
 * Author:      William Gantt
 * Description: Implementations of various functions for the Dataset class.
 *
 * TODO:
 *
 * 	- add implementation that lets you determine the ranges of attribute
 * 	  values represented in the data
 * 	- add implementation to determine the number of distinct classes
 * 	  there are
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

	int num_data_points; // the number of attribute vectors read in

	// if it opened successfully, start reading
	if (file_stream.is_open() && file_stream.good()) {

            	string line;         // the current line being read in

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

		// set the member variable containing the number of attributes
		num_attributes_ = attribute_names_.size();

		// a vector for storing attribute values
		vector<double> curr_vect;

		// read until the end of the file is reached, or until
		// some other error flag is set
		for (num_data_points = 0; file_stream.good(); num_data_points++) {

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
				if (token.compare("") != 0)
				       curr_vect.push_back(stof(token));

			        // delete the substring you just created from "line," as
			        // well as the delimiter that follows it
			        line.erase(line.begin(), line.begin() + tokenEnd + 1);

			} // end for

			// add the vector to the vector of data points, and then
			// delete its contents so that it can be used again
			data_points_.push_back(curr_vect);
			curr_vect.clear();

		} // end for 
	} else {

		// the file didn't open correctly, so print an error and quit
		cout << stderr << "Error: Unable to open file." << endl;
		exit(0); 
	}

	// set the member variable containing the number 
	// of data points to the appropriate value
	num_data_points_ = num_data_points;

	file_stream.close(); // close the file stream
	return num_data_points;    // return the number of vectors read in

} // end readFromCsvFile

void Dataset::print() {

	printf("Number of attributes: %d\n", num_attributes_);
	printf("Number of classes: %d\n", num_classes_);
	printf("Number of data points: %d\n", num_data_points_);
	
	vector<double> curr_vect;
	for (int i=0; i<num_data_points_; i++) {
		curr_vect = data_points_[i];
		for (int j=0; j<num_attributes_; j++) {
			printf("%.3f ", curr_vect[j]);
		}
		printf("\n");
	}
}
