#include "attributeVector.h"

int main(int argc, char **argv) {

	AttributeVector attV;
	std::string fileName;

	fileName = argv[1];
	attV.readFromCsvFile(fileName);

	return 0;
}
