all: notate

notate: include/notate.hpp src/notate.cpp src/eps/* src/jgr/* test/*
	f=`ls src/jgr/`;\
	for i in $$f;\
	do j=`echo $$i | sed 's/jgr/eps/'`; ./jgraph src/jgr/$$i > src/eps/$$j ;\
	done ;\
	g++ -Iinclude -o notate src/notate.cpp
	g=`ls test/`;\
	for k in $$g;\
	do m=`echo $$k | sed 's/txt/jgr/'`; n=`echo $$k | sed 's/txt/eps/'`; cat test/$$k | ./notate > jgr/$$m ; ./jgraph jgr/$$m > eps/$$n ; \
	done ;\
