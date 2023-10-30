#include <iostream>
#include <stdlib.h>

#define RAND() ((double)rand()/(double)(RAND_MAX))

struct SystemGrid
{
    int n;
    double pc;
    int *pos;
    bool *state;
};

typedef struct SystemGrid System;

bool gen(double pc)
{
    bool p = (RAND()>=pc) ? true : false;
    return p;
}

void createGrid(int *pos, bool *state, const int n, const double pc)
{
    int ii, jj, pp;
    for (jj=0; jj<n; jj++) {
        for (ii=0; ii<n; ii++) {
            pp = ii+n*jj;
            pos[pp] = pp;  
            state[pp] = gen(pc); 
        }
    }   
}

/* append data to output. */
void output(System &grid, FILE *out)
{
    int ii, jj;
    for (jj=0; jj< grid.n; jj++) {
        for (ii=0; ii< grid.n; ii++) {
            fprintf(out, " %d", grid.state[ii+grid.n*jj]);
        }
        fprintf(out, "\n");
    }
}

int main()
{
    std::srand(1234);
    int     n           = 100;                  // Side of Grid
    double  pc          = 0.5;                  // Critical Probability
    char    filename[]  = "../pic/Grid.txt";    // File adress for saving data
    System  grid;
    grid.n  = n;
    grid.pc = pc;
    FILE *out;
    
    out = fopen(filename, "w");

    /* Allocate Memory*/
    grid.pos=(int *)malloc(grid.n*grid.n*sizeof(int));
    grid.state=(bool *)malloc(grid.n*grid.n*sizeof(bool));

    /* Create Grid*/
    createGrid(grid.pos, grid.state, grid.n, grid.pc);

    /* Save Data*/
    output(grid, out);
    fclose(out);
    
    /* Deallocate memory*/
    free(grid.pos);
    free(grid.state);
    
    return 0;
}