#include "../population.h"

using namespace std;

mt19937 rng;
random_device rd;

uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {

	rng.seed(rd());
	Population p = Population::random(10,10,2);
	p.rules_[8].print();
	p.rules_[9].print();
	pair<Rule,Rule> offspring = p.crossover(8,9);
	offspring.first.printVerbose();
	offspring.second.printVerbose();

	return 0;
}
