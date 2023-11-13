CC=mpic++ -std=c++17
CFLAGS=-Wall -Werror
COPTS=-O3
DEPS=percolation.h
SRC0=single.cpp
SRC1=multiple.cpp
OBJ0=${SRC0:.cpp=.o}
OBJ1=${SRC1:.cpp=.o}

%.o: %.cpp $(DEPS)
	@$(CC) $(CFLAGS) $(COPTS) -o $@ -c $<

percolation: $(OBJ0)
	@$(CC) $(CFLAGS) $(COPTS) -o ../$@ $(OBJ0)

percstats: $(OBJ1)
	@$(CC) $(CFLAGS) $(COPTS) -o ../$@ $(OBJ1)

clean:
	@rm -f *.o *.mod *.x *.out *.gif ../percolation ../percstats