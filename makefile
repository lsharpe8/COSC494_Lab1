all: notate

notate: include/notate.hpp src/notate.cpp src/eps/* src/jgr/*
	f=`ls src/jgr/`;\
	for i in $$f;\
	do j=`echo $$i | sed 's/jgr/eps/'`; ./jgraph src/jgr/$$i > src/eps/$$j ;\
	done ;\
	g++ -Iinclude -o notate src/notate.cpp
	cat test/test.txt | ./notate > test/test.jgr
	./jgraph test/test.jgr > test/test.eps
