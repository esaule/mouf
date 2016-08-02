CXXFLAGS=-O3 -std=c++11 -g
LDFLAGS=-ltinyxml2 -g
LD=g++

extract: extract.o definition.o genre.o
	$(LD) $(LDFLAGS) -o extract extract.o definition.o genre.o

clean:
	-rm test

depend:
	makedepend -I . *.hpp *.cpp

# DO NOT DELETE

definition.o: util.hpp
extract.o: /usr/include/tinyxml2.h util.hpp definition.hpp genre.hpp
genre.o: genre.hpp util.hpp
