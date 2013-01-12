CXX=g++
CXXFLAGS= -O3 -Wall -march=native -pipe -std=c++98
#CXXFLAGS=-O0 -g -D CONFIG_ASSERT -Wall -std=c++98
config=/home/seb/coveo-blitz-2013/index/001-Apache.conf

TARGETS=Korafle.cgi

Korafle.cgi: 

LIBRARY_OBJECTS=KorafleWebService.o Mapper.o Engine.o Entry.o

Korafle.cgi: Korafle_main.o $(LIBRARY_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -l jsoncpp
	chmod g-w $@
	chmod o-w $@
	mkdir -p DocumentRoot
	rm DocumentRoot/Dorafle.cgi
	cp Korafle.cgi DocumentRoot/Dorafle.cgi
	cp Korafle.cgi DocumentRoot/Korafle.cgi

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $? -o $@


start:
	apache2 -f $(config) -k start

restart:
	apache2 -f $(config) -k restart

stop:
	apache2 -f $(config) -k stop

clean:
	rm -rf Koracle.cgi *.o

