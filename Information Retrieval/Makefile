# This is a simple Makefile for small projects.  When you 
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CXX=g++
CPPFLAGS=-lcurl

main: main.cpp
	$(CXX) -std=c++11 -o webspider main.cpp htmlstreamparser.cpp  $(CPPFLAGS)

webquery: webquery.cpp
	$(CXX) -std=c++11 -o webquery webquery.cpp

