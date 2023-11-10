CC=g++
CFLAGS=-O3
SRC=main.cpp percolation.h

percolation: $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $@.x; ./$@.x

clean:
	@rm -f *.x *.out *.gif