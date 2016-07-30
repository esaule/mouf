CXXFLAGS=-O3 -std=c++11 -g
LDFLAGS=-ltinyxml2 -g
LD=g++

test: test.o definition.o
	$(LD) $(LDFLAGS) -o test test.o definition.o

clean:
	-rm test
