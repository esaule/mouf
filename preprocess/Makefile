CXXFLAGS=-O3 -std=c++11 -g
LDFLAGS=-ltinyxml2 -g
LD=g++

extract: extract.o definition.o genre.o
	$(LD) $(LDFLAGS) -o extract extract.o definition.o genre.o

clean:
	-rm test
