# This is a simple Makefile for small projects.  When you 
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

#CXX=/vol/share/software/gcc/5.3.0/bin/g++
CXX = g++
CPPFLAGS=-lcurl

main: main.cpp
	$(CXX) -o webspider main.cpp htmlstreamparser.cpp  $(CPPFLAGS)

webquery: webquery.cpp
	$(CXX) -o webquery webquery.cpp

