# Makefile #

CXX = g++

CPPFLAGS =  -g # Debugging
CPPFLAGS =  -O3 # Optimization
CPPFLAGS += -Wall # Show all warnings
CPPFLAGS += -std=c++11 # Must use C++11

OBJS = dataset.o population.o rule.o attribute.o main.o

classify: $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $@

main.o: main.cpp
	$(CXX) $(CPPFLAGS) -c main.cpp -o $@

dataset.o: dataset.cpp dataset.h rule.cpp rule.h
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

clean:
	rm *.o classify
