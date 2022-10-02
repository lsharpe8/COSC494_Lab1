all: notate

notate: include/notate.hpp src/notate.cpp $(src/eps/*)
	g++ -Iinclude -o notate src/notate.cpp
	./notate > test.jgr
	./jgraph test.jgr > test.eps
