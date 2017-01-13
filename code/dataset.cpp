#include "dataset.h"

/****************************************************************************
 * File:        dataset.cpp
 * Author:      William Gantt
 * Description: Implementations of various functions for the Dataset class.
 *
 * TODO:
 * 	- Fix type names in readFromCSVFile
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Input:       
 *              fileName: the name of the file to be read in
 * Output:      the number of data points successfully read in
 * Description: reads in vectors of attributes from a text file
 ****************************************************************************/
int Dataset::readFromCSVFile(string fileName) {

	fstream fileStream; // the file stream
	fileStream.open(fileName.c_str()); // try to open the file

	// if it opened successfully, start reading
	if (fileStream.is_open() && fileStream.good()) {

            	string line;        // the current line being read in
	        int attNum;         // the number of the current attribute 
	        int numVects;       // the number of attribute vectors read in

		// get the first line of the file (this tells us the
		// names of the attributes)
		getline(fileStream, line);

		// an individual word or entry from the line read in
		string token;

		// this function handles .csv files, so the delimiter
		// should always be a comma
		string delimiter = ",";

		// appending a comma to the end of the line makes the
		// parsing a little bit easier, as you'll see below
		line.append(delimiter);

		// a list of all the attribute names
		vector<string> attributeNames;

		int tokenEnd; // a string index

		do {
			// find the next delimiting character
			tokenEnd = line.find(delimiter);

			// get the substring from the beginning of the line
			// up to, but not including, the delimiter
			token = line.substr(0, tokenEnd);

			// add this string to the vector of all attribute names
			attributeNames.push_back(token);

			// delete the substring you just created from "line," as
			// well as the delimiter that follows it
			line.erase(line.begin(), line.begin() + tokenEnd + 1);

		  // repeat so long as we have not removed
		  // all of the characters from the string
		} while (line.size() > 0);

		// a variable for reading in a single attribute
		Attribute *a;

		// a variable for the value of the attribute
		float aVal;

		// a variable for a vector of attribute values
		AttributeVector aVect;

		// read until the end of the file is reached, or until
		// some other error flag is set
		for (numVects = 0; fileStream.good(); numVects++) {

			// get the current line
			getline(fileStream, line);

			// add a comma to the end of the line (this makes parsing easier,
			// as mentioned above)
			line.append(delimiter);

			// as attribute values are extracted from a line, they are deleted
			// from the string. Thus, we want to move onto the next line when
			// the line size reaches 0.
			for (attNum = 0; line.size() > 0; attNum++) {

				// find the next delimiting character
			        tokenEnd = line.find(delimiter);

			        // get the substring from the beginning of the line
			        // up to, but not including, the delimiter
			        token = line.substr(0, tokenEnd);

				// we have to have this check here because otherwise the program
				// will throw an exception when it reaches the end of the file.
				if (token.compare("") != 0)
					aVal = stof(token);

				// create a new attribute
				a = new Attribute("test", aVal);

			        // add this attribute value to the vector of all attribute names
			        aVect.addAttribute(*a);
				delete a;

			        // delete the substring you just created from "line," as
			        // well as the delimiter that follows it
			        line.erase(line.begin(), line.begin() + tokenEnd + 1);

			} // end for
		} // end if
	} else {

		// the file didn't open correctly, so print an error and quit
		cout << stderr << "Error: Unable to open file." << endl;
		exit(0); 
	}

	fileStream.close(); // close the file stream
	return numVects;    // return the number of vectors read in

} // end readFromCsvFile
