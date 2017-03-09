
CXX = g++

CPPFLAGS = -g # debugging
CPPFLAGS += -Wall # show all warnings
CPPFLAGS += -std=c++11 # must use C++11

ruleTest.o: ruleTest.cpp unity.cpp
	$(CXX) $(CPPFLAGS) ruleTest.cpp unity.cpp

unity.o: unity.cpp
	$(CXX) $(CPPFLAGS) -c unity.cpp

clean:
	rm ruleTest
	rm *.o
