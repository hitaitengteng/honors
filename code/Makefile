# Makefile #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

classify: main.o lcs.o dataset.o population.o rule.o attribute.o
	$(CXX) $(CPPFLAGS) main.o lcs.o dataset.o population.o rule.o attribute.o -o $@

lcs.o: lcs.cpp lcs.h
	$(CXX) $(CPPFLAGS) -c lcs.cpp -o $@

main.o: main.cpp lcs.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o $@

dataset.o: dataset.cpp dataset.h
	$(CXX) $(CPPFLAGS) -c dataset.cpp -o $@

population.o: population.cpp population.h
	$(CXX) $(CPPFLAGS) -c population.cpp -o $@

rule.o: rule.cpp rule.h
	$(CXX) $(CPPFLAGS) -c rule.cpp -o $@

attribute.o: attribute.cpp attribute.h utilities.h
	$(CXX) $(CPPFLAGS) -c attribute.cpp -o $@

all:
	make attribute.o
	make rule.o
	make population.o
	make dataset.o
	make main.o
	make lcs.o

clean:
	rm *.o classify
