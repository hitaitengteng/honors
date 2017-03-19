#include "attribute.h"

/****************************************************************************
 * File: attribute.h
 * Author: Will Gantt
 * Description:
 ****************************************************************************/ 


/****************************************************************************
 * Inputs:      None
 * Outputs:
 * Description:
 ****************************************************************************/ 
Attribute Attribute::random() {

	// create the attribute
	Attribute a;

	// set the dontCare value
	int dc = (int_dist(rng) % 2);
	if (dc == 0) {
		a.setDontCare(false);
	} else {
		a.setDontCare(true);
	}

	// set the center and spread
	a.setCenter(real_dist(rng));
	a.setSpread(real_dist(rng));

	return a;

} // end getRandom

/****************************************************************************
 * Inputs:     
 * Outputs:
 * Description:
 ****************************************************************************/ 
Attribute Attribute::random(std::pair<double,double> attribute_ranges, 
		double range_scalar, double dont_care_prob) {

	// create the attribute
	Attribute a;

	// get the range of acceptable values
	double range = attribute_ranges.second - attribute_ranges.first;

	// randomly set the dontCare value
	double dc = real_dist(rng);
	if (dc <= dont_care_prob) {
		a.setDontCare(true);
	} else {
		a.setDontCare(false);
	}

	// set the center and spread
	a.setCenter(attribute_ranges.first + (real_dist(rng) * range));
	a.setSpread(real_dist(rng) * range * range_scalar);

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

	printf("Name:     %s\n", name_.c_str());
	printf("Center:   %.3f\n", center_);
	printf("Spread:   %.3f\n", spread_);
	printf("dontCare: %s\n", dc.c_str());
}
