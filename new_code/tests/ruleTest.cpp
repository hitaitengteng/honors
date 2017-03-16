#include "../utilities.h"
#include "../rule.h"

using namespace std;

mt19937 rng;
random_device rd;

uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {

	rng.seed(rd());
	Rule r = Rule::random(10);
	r.printVerbose();

	return 0;
}
