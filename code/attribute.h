#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "utilities.h"

/****************************************************************************
 * File:        attribute.h
 * Author:      Will Gantt
 * Description: Defines the Attribute class. [Description]
 ****************************************************************************/ 

class Attribute {

	public:
		// create an attribute with just a name
		Attribute(): 
			name_(""), center_(0), spread_(0), dont_care_(false) {
		}

		// create an attribute with no name
		Attribute(double center, double spread, bool dont_care) {
			setCenter(center);
			setSpread(spread);
			setDontCare(dont_care);
		}


		// fully specify an attribute
		Attribute(std::string name, double center, double spread, bool dont_care) {
			setName(name);
			setCenter(center);
			setSpread(spread);
			setDontCare(dont_care);
		}

		// generates a random attribute
		static Attribute random();

		// prints info about an attribute
		void print();

		// getters 
		std::string name() const {return name_;}
		double center() const {return center_;}
		double spread() const {return spread_;}
		bool dont_care() const {return dont_care_;}

		// setters
		void setName(std::string name) {name_ = name;}
		void setCenter(double center) {center_ = center;}
		void setSpread(double spread) {spread_ = spread;}
		void setDontCare(bool dont_care) {dont_care_ = dont_care;}

	private:
		std::string name_; // the name of the attribute
		double center_;    // the midpoint of the interval covered by the attribute
		double spread_;    // this defines the size of the interval
		bool dont_care_;   // indicates whether this attribute is a relevant component
			           // of a given rule (true = 'we don't care'; false = 'we do care')
};

#endif
