#include "../rule.h"

bool testEquality();
bool testGeneralizes();
bool testMutate();
bool testSpecify();

int main(int argc, char **argv) {

	return 0;
}

bool testEquality(mnt19937 &rng) {

	Rule r1;
	Rule r2;

	r1.setClass(1);
	r2.setClass(1);

	uniform_real_distribution<double> dist(0,1);

	double center = 0;
	double spread = 0;

	for (int i=0; i<10; i++) {j
		center = dist(rng);
		spread = dist(rng);
		r1.condition.set

	// equal; should return true
	
	// same centers, different spreads; should return false
	
	// same spreads, different centers; should return false
	
	// same centers and spreads, different classes; should
	// return false
	
	// rule 1 generalizes rule 2; should return false
}
