# Makefile #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

OBJS = lcs.o dataset.o population.o rule.o attribute.o matchSet.o correctSet.o main.o

classify: $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $@

lcs.o: lcs.cpp lcs.h matchSet.cpp correctSet.cpp
	$(CXX) $(CPPFLAGS) -c lcs.cpp -o $@

main.o: main.cpp lcs.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o $@

dataset.o: dataset.cpp dataset.h rule.cpp rule.h
	$(CXX) $(CPPFLAGS) -c dataset.cpp -o $@

correctSet.o: correctSet.cpp correctSet.h
	$(CXX) $(CPPFLAGS) -c correctSet.cpp -o $@

matchSet.o: matchSet.cpp matchSet.h
	$(CXX) $(CPPFLAGS) -c matchSet.cpp -o $@

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
	make matchSet.o
	make correctSet.o
	make lcs.o

clean:
	rm *.o *.csv classify
