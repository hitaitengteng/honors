#include "../population.h"

using namespace std;

mt19937 rng;
random_device rd;

uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {

	rng.seed(rd());
	Population p = Population::random(10,10);

	int *selections = (int*) malloc(sizeof(int) * 10);
	for (int i=0; i<10; i++)
		selections[i] = 0;
	int selection = -1;
	for (int i=0; i<10000; i++) {
		selection = p.rouletteWheelSelect();
		selections[selection]++;
	}

	p.rank();
	for (int i=0; i<p.max_size(); i++)
		printf("%d: %f\n", p.rules_[i].id(), (p.rules_[i].fitness1()/p.fitness1_sum()));

	cout << endl << endl;

	for (int i=0; i<10; i++)
		printf("%d: %f\n", p.rules_[i].id(), selections[p.rules_[i].id()]/10000.0);

	free(selections);

	return 0;
}
