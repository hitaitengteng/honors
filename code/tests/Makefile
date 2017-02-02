# Makefile --- Tests #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

RULE_DEPS = ruleTest.cpp ../rule.cpp ../attribute.cpp ../dataset.cpp
POP_DEPS = populationTest.cpp ../population.cpp ../rule.cpp ../attribute.cpp ../dataset.cpp

test_rules: $(RULE_DEPS)
	$(CXX) $(CPPFLAGS) -o test_rules $(RULE_DEPS)

test_pop: $(POP_DEPS)
	$(CXX) $(CPPFLAGS) -o test_pop $(POP_DEPS)

all:
	make test_rules
	make test_pop
clean:
	rm -f test_rules test_pop
