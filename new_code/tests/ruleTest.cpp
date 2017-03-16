#include "../utilities.h"
#include "../rule.h"

using namespace std;

int main(int argc, char **argv) {

	Rule r = Rule::random(10);
	r.printVerbose();

	return 0;
}
