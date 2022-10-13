export

all: notate test

notate: include/notate.hpp src/notate.cpp src/eps/* src/jgr/* test/*
	f=`ls src/jgr/`;\
	for i in $$f;\
	do j=`echo $$i | sed 's/jgr/eps/'`; /home/jplank/bin/LINUX-X86_64/jgraph src/jgr/$$i > src/eps/$$j ;\
	done ;\
	g++ -Iinclude -o notate src/notate.cpp ;

test: notate
	g=`ls test/`;\
	for k in $$g;\
	do m=`echo $$k | sed 's/txt/jgr/'`; n=`echo $$k | sed 's/txt/pdf/'`; cat test/$$k | ./notate > jgr/$$m ; /home/jplank/bin/LINUX-X86_64/jgraph -P jgr/$$m | ps2pdf - > pdf/$$n ; \
	done ;

run: notate
	./notate > tmp.jgr ;
	/home/jplank/bin/LINUX-X86_64/jgraph -P tmp.jgr | ps2pdf - > pdf/$(file) ;
	rm tmp.jgr ;

clean:
	if [ -f notate ]; then \
	rm notate ; \
	fi
