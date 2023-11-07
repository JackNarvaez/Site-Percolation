#include <iostream>
#include <stdlib.h>
#include <algorithm>

#define RAND() ((double)rand()/(double)(RAND_MAX))

struct SystemGrid
{
    int     n;          // Side of the squared grid
    double  pc;         // Critical Probability
    int     n_cluster;  // Number of created clusters
    int     *pos;       // Cells ID
    bool    *state;     // Occupied or Unoccupied
    int     *cluster;   // Cluster's label
    int     *classes;   // 1D Array for cluster's alias
    int     *children;     // Children per cluster
};

typedef struct SystemGrid System;

bool gen(double pc)
{
    bool p = (RAND()<=pc) ? true : false;
    return p;
}

void createGrid(System &grid)
{
    int ii, jj, pp;
    int n = grid.n;
    for (jj=0; jj<n; jj++) {
        for (ii=0; ii<n; ii++) {
            pp = ii+n*jj;
            grid.pos[pp] = pp;  
            grid.state[pp] = gen(grid.pc); 
            grid.cluster[pp] = 0;
        }
    }
    int class_max = n*n/4;
    for (pp=0; pp<class_max; pp++) {
        grid.classes[pp] = pp;
        grid.children[pp] = 0;
    }
}

void link(int *classes, int n_ctr, int min_ctr)
{   
    classes[n_ctr] = min_ctr;
}

int find(int *classes, int p) {
    int temp = p;
    while (classes[temp] != temp) temp = classes[temp];
    return temp;
}

void hoshenKopelman(System &grid)
{
    int ii, jj, pc, pl, pa, min_la, max_la;
    bool left, above;
    int n  = grid.n;
    int n2 = n*n;
    for (jj=0; jj<n; jj++) {
        for (ii=0; ii<n; ii++) {
            pc = ii + n*jj;
            if (grid.state[pc]) {
                pl = (ii-1) + n*jj;
                pa = ii + n*(jj-1);
                above = (jj==0 ? false : grid.state[pa]);
                left = (ii==0 ? false : grid.state[pl]);
                if (!left and !above) {
                    grid.n_cluster += 1;
                    grid.cluster[pc] = grid.n_cluster;
                } else if (left and !above) {
                    grid.cluster[pc] = grid.classes[grid.cluster[pl]];
                } else if (!left and above) {
                    grid.cluster[pc] = grid.classes[grid.cluster[pa]];
                } else {
                    min_la = std::min(grid.cluster[pa], grid.cluster[pl]);
                    max_la = std::max(grid.cluster[pa], grid.cluster[pl]);
                    if (min_la != max_la) link(grid.classes, max_la, min_la);
                    grid.cluster[pc] = grid.classes[min_la];
                }
            }
        }
    }

    // Assign first equivalente class for each cluster
    for (pc=0; pc<n2; pc++) {
        grid.cluster[pc] = find(grid.classes, grid.cluster[pc]);
    }
}

void countCluster(System &grid)
{
    int n2 = grid.n*grid.n;
    int pc;
    grid.n_cluster = *std::max_element(grid.cluster, grid.cluster+n2);

    for (pc=0; pc<grid.n_cluster; pc++) {
        grid.classes[pc] = pc;
    }
    for (pc=0; pc<n2; pc++) {
        grid.children[grid.cluster[pc]] += 1;
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