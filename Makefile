CXX = g++

all: jsonfmt

jsonfmt: jsonfmt.cpp
	$(CXX) $^ -g -o $@

clean:
	rm jsonfmt
