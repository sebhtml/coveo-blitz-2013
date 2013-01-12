CXX=g++
CXXFLAGS= -O3 -Wall -march=native -pipe -std=c++98
#CXXFLAGS=-O0 -g -D CONFIG_ASSERT -Wall -std=c++98

TARGETS=Korafle.cgi

Korafle.cgi: 

LIBRARY_OBJECTS=KorafleWebService.o

Korafle.cgi: Korafle_main.o $(LIBRARY_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@
	chmod g-w $@
	chmod o-w $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $? -o $@


