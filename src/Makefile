CC=mpic++ -std=c++17
CFLAGS=-Wall -Werror
COPTS=-O3
DEPS=percolation.h
SRC=main.cpp
OBJ=${SRC:.cpp=.o}

%.o: %.cpp $(DEPS)
	@$(CC) $(CFLAGS) $(COPTS) -o $@ -c $<

percolation: $(OBJ)
	@$(CC) $(CFLAGS) $(COPTS) -o ../$@ $(OBJ)

clean:
	@rm -f *.o *.mod *.x *.out *.gif percolation