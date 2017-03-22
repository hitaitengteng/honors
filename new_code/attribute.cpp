#include "attribute.h"

/****************************************************************************
 * File: attribute.h
 * Author: Will Gantt
 * Description:
 ****************************************************************************/ 


/****************************************************************************
 * Inputs:     
 * Outputs:
 * Description:
 ****************************************************************************/ 
Attribute Attribute::random(std::vector<double> quantiles, int quantile, double dont_care_prob) {

	bool dont_care = false;

	// randomly set the dontCare value
	double dc = real_dist(rng);
	if (dc <= dont_care_prob)
		dont_care = true;

	Attribute a = Attribute(quantiles[quantile], quantiles[quantile+1], quantile, dont_care);
	return a;

} // end getRandom

/****************************************************************************
 * Inputs:      None
 * Outputs:     None
 * Description: Prints information about an attribute, including its name,
 * 		center, spread, and "don't care" values.
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
