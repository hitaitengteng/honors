# Makefile #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

classify: main.cpp dataset.cpp
	$(CXX) $(CPPFLAGS) main.cpp dataset.cpp -o $@

# main.o: main.cpp
#	$(CXX) $(CPPFLAGS) -c main.cpp -o $@

# dataset.o: dataset.cpp dataset.h
#	$(CXX) $(CPPFLAGS) -c dataset.cpp -o $@

rule.o: rule.cpp rule.h
	$(CXX) $(CPPFLAGS) -c rule.cpp -o $@

attribute.o: attribute.cpp attribute.h utilities.h
	$(CXX) $(CPPFLAGS) -c attribute.cpp -o $@

clean:
	rm *.o classify
