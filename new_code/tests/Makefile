
CXX = g++

CPPFLAGS = -g # debugging
CPPFLAGS += -Wall # show all warnings
CPPFLAGS += -std=c++11 # must use C++11

ruleTest: ruleTest.cpp ../rule.cpp ../attribute.cpp
	$(CXX) $(CPPFLAGS) -o ruleTest ruleTest.cpp ../rule.cpp ../attribute.cpp

clean:
	rm ruleTest
	rm *.o
