# Makefile #

CXX = g++

CPPFLAGS =  -g
CPPFLAGS += -Wall
CPPFLAGS += -std=c++11

classify: main.o attributeVector.o
	$(CXX) $(CPPFLAGS) main.o attributeVector.o -o $@

main.o: main.cpp attributeVector.h
	$(CXX) $(CPPFLAGS) -c main.cpp -o $@

attributeVector.o: attributeVector.cpp attributeVector.h
	$(CXX) $(CPPFLAGS) -c attributeVector.cpp -o $@

clean:
	rm *.o classify
