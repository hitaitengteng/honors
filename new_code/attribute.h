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
			name_(""), l_bound_(0), u_bound_(0), dont_care_(false) {
		}

		// create an attribute with no name
		Attribute(double l_bound, double u_bound, int quantile, bool dont_care) {
			setLowerBound(l_bound);
			setUpperBound(u_bound);
			setQuantile(quantile);
			setDontCare(dont_care);
		}


		// fully specify an attribute
		Attribute(std::string name, double l_bound, double u_bound, int quantile, bool dont_care) {
			setName(name);
			setLowerBound(l_bound);
			setUpperBound(u_bound);
			setQuantile(quantile);
			setDontCare(dont_care);
		}

		// generates a random attribute
		static Attribute random(std::vector<double> quantiles, int quantile, double dont_care_prob);

		// prints info about an attribute
		void print();

		// getters 
		std::string name() const {return name_;}
		double l_bound() const {return l_bound_;}
		double u_bound() const {return u_bound_;}
		int quantile() const {return quantile_;}
		bool dont_care() const {return dont_care_;}

		// setters
		void setName(std::string name) {name_ = name;}
		void setLowerBound(double l_bound) {l_bound_ = l_bound;}
		void setUpperBound(double u_bound) {u_bound_ = u_bound;}
		void setQuantile(int quantile) {quantile_ = quantile;}
		void setDontCare(bool dont_care) {dont_care_ = dont_care;}

	private:
		std::string name_; // the name of the attribute
		double l_bound_;   // the midpoint of the interval covered by the attribute
		double u_bound_;   // this defines the size of the interval
		int quantile_;      // the range of values covered by this attribute
		bool dont_care_;   // indicates whether this attribute is a relevant component
			           // of a given rule (true = 'we don't care'; false = 'we do care')
};

#endif
