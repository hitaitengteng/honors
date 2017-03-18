#include "../utilities.h"
#include "../rule.h"

using namespace std;

mt19937 rng;
random_device rd;

uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {

	rng.seed(rd());

	double p_mutate = 0.25;
	double p_dont_care = 0.4;
	double range_scalar = 0.2;

	vector<pair<double,double> > ranges;
	for (int i=0; i<10; i++)
		ranges.push_back(make_pair(0,1));

	Rule r = Rule::random(10,2);
	r.print();
	r.mutate(p_mutate,p_dont_care,ranges,range_scalar);
	r.print();
	r.mutate(p_mutate,p_dont_care,ranges,range_scalar);
	r.print();

	return 0;
}
