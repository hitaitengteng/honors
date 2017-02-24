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
	int dc = (rng() % 2);
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
Attribute Attribute::random(std::pair<double,double> attribute_ranges, double range_scalar) {

	// create the attribute
	Attribute a;

	// get the range of acceptable values
	double range = attribute_ranges.second - attribute_ranges.first;

	// set the dontCare value
	int dc = (rng() % 2);
	if (dc == 0) {
		a.setDontCare(false);
	} else {
		a.setDontCare(true);
	}

	// set the center and spread
	a.setCenter(real_dist(rng) * range);
	a.setSpread(real_dist(rng) * range_scalar);

	return a;

} // end getRandom

/****************************************************************************
 * Inputs:      None
 * Outputs:     None
 * Description:
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
