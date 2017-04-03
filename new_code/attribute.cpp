#include "attribute.h"

/****************************************************************************
 * File:        attribute.cpp
 * Author:      Will Gantt
 * Description: Defines functions for the Attribute class. See attribute.h
 * 		for a detailed description of the class.
 ****************************************************************************/ 


/****************************************************************************
 * Inputs:     
 * 		quantiles: a vector of doubles demarcating the quantiles
 * 		of this attribute.
 *
 * 		quantile: the index of the quantile in the quantiles vector
 * 		specifying the range of values covered by the attribute
 *
 * 		dont_care_prob: the probability that the attribute will have
 * 		its 'dont_care_' value set to true.
 *
 * Outputs:     A random attribute object
 * Description: Creates a random attribute from a vector of quantiles, a
 * 		quantile index, and a 'dont_care_' probability. Note that
 * 		the only real randomness here is in the selection of the 
 * 		value of dont_care_. This is by design.
 ****************************************************************************/ 
Attribute Attribute::random(std::vector<double> quantiles, int quantile, 
		double dont_care_prob) {

	bool dont_care = false;

	// randomly set the dont_care_ value
	double dc = real_dist(rng);
	if (dc <= dont_care_prob)
		dont_care = true;

	// create the attribute
	Attribute a = Attribute(quantiles[quantile], 
			quantiles[quantile+1], quantile, dont_care);
	return a;

} // end getRandom

/****************************************************************************
 * Inputs:      None
 * Outputs:     None
 * Description: Prints the values of all member variables of an attribute
 ****************************************************************************/ 
void Attribute::print() {

	std::string dc = "";
	if (dont_care_ == false)
		dc = "false";
	else
		dc = "true";

	printf("Name:        %s\n", name_.c_str());
	printf("Quantile:    %d\n", quantile_);
	printf("Upper Bound: %.3f\n", u_bound_);
	printf("Lower Bound: %.3f\n", l_bound_);
	printf("Don't Care:  %s\n", dc.c_str());

} // end print
