#ifndef ATTRIBUTEVECTOR_H
#define ATTRIBUTEVECTOR_H

#include "attribute.h"

class AttributeVector {

	public:
		// function for reading in data from a csv file
		int readFromCsvFile(std::string fileName);

		// wrappers for relevant vector functions
		void addAttribute(Attribute a) {attVect.push_back(a);}
		bool isEmpty() {return attVect.empty();}
		int size() {return attVect.size();}

	private:
		std::vector<Attribute> attVect;
};

#endif
