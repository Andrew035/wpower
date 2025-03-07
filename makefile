CXX = g++
CXXFLAGS = -Wall -g
IODIR = ../..wpower_IO/

mytest: wpower.o mytest.cpp
	$(CXX) $(CXXFLAGS) wpower.o mytest.cpp -o mytest

wpower.o: wpower.cpp wpower.h wpower.o
	$(CXX) $(CXXFLAGS) -c wpower.cpp

clean:
	rm *.o*
	rm *~

run:
	./mytest

val:
	valgrind ./mytest

vale:
	valgrind -s ./mytest
