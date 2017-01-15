#include "attribute.h"

Attribute Attribute::getRandom() {

	// a random number generator and a uniform distrubtion over [0,1]
	mt19937 rng;
	rng.seed(random_device{}());
	uniform_real_distribution<double> dist(0,1);


	// generate a name for the attribute type
	int index = rng() % NUM_TEST_NAMES;
	string name = testNames[index];
	
	// create the attribute
	Attribute a(name);

	// set the dontCare value
	int dontCare = rng() % 2;
	if (dontCare == 0)
		a.setDontCare(false);
	else
		a.setDontCare(true);

	// set the center and spread
	a.setCenter(dist(rng));
	a.setSpread(dist(rng));

	return a;

} // end getRandom

void Attribute::print() {

	string dc = "";
	if (dontCare == false)
		dc = "false";
	else
		dc = "true";

	printf("Name:     %s\n", name.c_str());
	printf("Center:   %.3f\n", center);
	printf("Spread:   %.3f\n", spread);
	printf("dontCare: %s\n", dc.c_str());
}
