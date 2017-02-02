# Makefile --- Tests #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

test_rules: ruleTest.cpp ../rule.cpp ../attribute.cpp ../dataset.cpp
	$(CXX) $(CPPFLAGS) -o test_rules ruleTest.cpp ../rule.cpp ../attribute.cpp ../dataset.cpp

test_pop: populationTest.cpp ../population.cpp ../rule.cpp ../attribute.cpp
	$(CXX) $(CPPFLAGS) -o test_pop populationTest.cpp ../population.cpp ../rule.cpp ../attribute.cpp

all:
	make test_rules
	make test_pop
clean:
	rm -f test_rules test_pop
