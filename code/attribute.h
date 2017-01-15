#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "utilities.h"

/****************************************************************************
 * File:        attribute.h
 * Author:      Will Gantt
 * Description: Defines the Attribute class. [Description]
 ****************************************************************************/ 

using namespace std;

// for testing
static const int NUM_TEST_NAMES = 10;
static const string testNames[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon",
			           "Zeta", "Eta", "Theta", "Iota", "Kappa"};
class Attribute {

	public:
		Attribute(string name): 
			name(name), center(0), spread(0), dontCare(false) {
		}

		Attribute(string name, double center, double spread, bool dontCare) {
			setName(name);
			setCenter(center);
			setSpread(spread);
			setDontCare(dontCare);
		}

		~Attribute(){}

		// generates a random attribute
		static Attribute getRandom();

		// prints info about an attribute
		void print();

		// getters 
		string getName() const {return name;}
		double getCenter() const {return center;}
		double getSpread() const {return spread;}
		bool getDontCare() const {return dontCare;}

		// setters
		void setName(string name) {this->name = name;}
		void setCenter(double center) {this->center = center;}
		void setSpread(double spread) {this->spread = spread;}
		void setDontCare(bool dontCare) {this->dontCare = dontCare;}

	private:
		string name;   // the name of the attribute
		double center; // the midpoint of the interval covered by the attribute
		double spread; // this defines the size of the interval
		bool dontCare; // indicates whether this attribute is a relevant component
			       // of a given rule (true = 'we don't care'; false = 'we do care')
};

#endif
