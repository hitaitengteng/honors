#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "utilities.h"

/****************************************************************************
 * File:        attribute.h
 * Author:      Will Gantt
 * Description: Defines the Attribute class and declares related functions.
 *
 * 		An attribute is a component of a rule and
 * 		specifies a range of values defined by an upper bound 
 * 		('u_bound') and a lower bound (l_bound). This range typically
 * 		represents a single quantile from a larger range of values,
 * 		which is the reason for the 'quantile_' member variable.
 *
 * 		Attributes also have a boolean member variable named
 * 		'dont_care_'. This variable indicates whether the attribute
 * 		is relevant for a particular rule. If attribute N of a rule
 * 		has dont_care_ set to true, then attribute N is not a factor
 * 		in determining whether the rule matches a given example.
 *
 * 		Finally, attributes also have a name. Currently, this
 * 		information is not used anywhere in the program, except when
 * 		examples are being read in.
 ****************************************************************************/ 

class Attribute {

	public:
		// create an attribute with just a name
		Attribute(): 
			name_(""), 
			l_bound_(0), 
			u_bound_(0), 
			dont_care_(false) {
		}

		// create an attribute with no name
		Attribute(double l_bound, 
			  double u_bound, 
			  int quantile, 
			  bool dont_care) {

			setLowerBound(l_bound);
			setUpperBound(u_bound);
			setQuantile(quantile);
			setDontCare(dont_care);
		}


		// create a fully-specified attribute with a name
		Attribute(std::string name, 
			  double l_bound, 
			  double u_bound, 
			  int quantile, 
			  bool dont_care) {

			setName(name);
			setLowerBound(l_bound);
			setUpperBound(u_bound);
			setQuantile(quantile);
			setDontCare(dont_care);
		}

		// generates a random attribute
		static Attribute random(std::vector<double> quantiles, 
				        int quantile, 
					double dont_care_prob);

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

		// the name of the attribute
		std::string name_; 

		// the lower bound of the interval covered by the attribute
		double l_bound_;   

		// the upper bound of the interval covered by the attribute
		double u_bound_;   

		// the quantile of possible values for this attribute represented
		// by the range defined by l_bound_ and u_bound_
		int quantile_;      
		
		// indicates whether this attribute is a relevant component
		// of a given rule (true = 'we don't care'; false = 'we do care')
		bool dont_care_;   			           
};

#endif
