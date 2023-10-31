#include <iostream>
#include <stdlib.h>

#define RAND() ((double)rand()/(double)(RAND_MAX))

struct SystemGrid
{
    int     n;
    double  pc;
    int     n_cluster;
    int     *pos;
    bool    *state;
    int     *cluster;
};

typedef struct SystemGrid System;

bool gen(double pc)
{
    bool p = (RAND()>=pc) ? true : false;
    return p;
}

void createGrid(int *pos, bool *state, int *cluster, const int n, const double pc)
{
    int ii, jj, pp;
    for (jj=0; jj<n; jj++) {
        for (ii=0; ii<n; ii++) {
            pp = ii+n*jj;
            pos[pp] = pp;  
            state[pp] = gen(pc); 
            cluster[pp] = 0; 
        }
    }  
}

void link(int *cluster, int pl, int pa, int n)
{  
    int pmin = std::min(cluster[pa],cluster[pl]);
    int pmax = std::max(cluster[pa],cluster[pl]);
    int pp;
    for (pp=0; pp<pl+1; pp++) {
        if (cluster[pp] == pmax) cluster[pp] = pmin;
    }
}

void hoshenKopelman(System &grid)
{
    int ii, jj, pc, pl, pa;
    bool left, above;
    for (jj=0; jj<grid.n; jj++) {
        for (ii=0; ii<grid.n; ii++) {
            pc = ii + grid.n*jj;
            if (grid.state[pc]) {
                pl = (ii-1) + grid.n*jj;
                pa = ii + grid.n*(jj-1);
                above = (jj==0 ? false : grid.state[pa]);
                left = (ii==0 ? false : grid.state[pl]);
                if (!left and !above) {
                    grid.n_cluster += 1;
                    grid.cluster[pc] = grid.n_cluster;
                } else if (left and !above) {
                    grid.cluster[pc] = grid.cluster[pl];
                } else if (!left and above) {
                    grid.cluster[pc] = grid.cluster[pa];
                } else {
                    if (grid.cluster[pa] != grid.cluster[pl]) {
                        grid.n_cluster -= 1;
                        link(grid.cluster, pl, pa, grid.n);
                    }
                    grid.cluster[pc] = grid.cluster[pl];
                }
            }
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

/* append data to output. */
void outputCluster(System &grid, FILE *out)
{
    int ii, jj;
    for (jj=0; jj< grid.n; jj++) {
        for (ii=0; ii< grid.n; ii++) {
            fprintf(out, " %d", grid.cluster[ii+grid.n*jj]);
        }
        fprintf(out, "\n");
    }
}