CC=g++
CFLAGS=-O3
SRC=main.cpp percolation.h

percolation: $(SRC)
	$(CC) $(FFLAGS) $(SRC) -o $@.x; ./$@.x

clean:
	@rm -f *.x *.txt *.png *.out *.gif