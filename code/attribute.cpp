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

	// generate a name for the attribute type
	int index = rng() % NUM_TEST_NAMES;
	std::string name = TEST_NAMES[index];
	
	// create the attribute
	Attribute a(name);

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
