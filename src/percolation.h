/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <algorithm>

#define RAND() ((double)rand()/(double)(RAND_MAX))

struct SystemGrid
{
    int     n;          // Side of the squared grid
    double  pc;         // Critical Probability
    int     percolate;  // If there exists, it contains the percolate cluster label.
    int     *cluster;   // Cluster's label
    int     *classes;   // 1D Array for cluster's alias (Temporal)
    int     *finclas;   // 1D Array for cluster's alias (Equivalente classes joined)
    int     *children;  // Children per cluster
};

typedef struct SystemGrid System;

bool gen(double pc)
{
    // Get a cell occupied (1) or unoccupied (0) 
    int p = (RAND()<=pc) ? 1 : 0;
    return p;
}

void createGrid(System &grid)
{
    // Initialise the grid
    int pp, n, n2, cm;
    n  = grid.n;
    n2 = n*n;
    cm = n2/4;
    for (pp=0; pp<n2; pp++) grid.cluster[pp] = gen(grid.pc);

    for (pp=0; pp<cm; pp++) {
        grid.classes[pp]  = 0;
        grid.finclas[pp]  = 0;
        grid.children[pp] = 0;
    }
}

int find(int *classes, int p) {
    // find parent (root) of the pth-cluster
    int temp = p;
    int temp2;
    while (classes[temp] != temp) temp = classes[temp];
    
    while (classes[p] != p) {
        temp2 = classes[p];
        classes[p] = temp;
        p = temp2;
    }
    return temp;
}

int link(int *classes, int above, int left)
{   
    // Connect clusters above and left by changing the root
    // of one of them
    classes[find(classes, above)] = find(classes, left);
    return classes[find(classes, above)];
}

void hoshenKopelman(System &grid)
{
    int ii, jj, pc, left, above, x;
    int n  = grid.n;
    int n2 = n*n;
    for (jj=0; jj<n; jj++) {
        for (ii=0; ii<n; ii++) {
            pc = ii + n*jj;
            if (grid.cluster[pc]) {
                above = (jj==0 ? 0 : grid.cluster[ii + n*(jj-1)]);
                left  = (ii==0 ? 0 : grid.cluster[(ii-1) + n*jj]);
                switch(!!left + !!above)
                {
                    case 0: // New Cluster [Isolated cell]
                        grid.classes[0] ++;
                        grid.classes[grid.classes[0]] = grid.classes[0];
                        grid.cluster[pc] = grid.classes[0];
                        break;

                    case 1: // Join one cluster [1 Neighbour]
                        grid.cluster[pc] = std::max(left, above);
                        break;

                    case 2: // Link two clusters [2 Neighbours]
                        grid.cluster[pc] = link(grid.classes, above, left);
                        break;
                }
            }
        }
    }

    // Assign first equivalente class for each cluster

    for (pc=0; pc <n2; pc++){
        if (grid.cluster[pc]) {
            x = find(grid.classes, grid.cluster[pc]);
            if (grid.finclas[x] == 0) {
                grid.finclas[0] ++;
                grid.finclas[x] = grid.finclas[0];
            }
            grid.cluster[pc] = grid.finclas[x];
            grid.children[grid.finclas[x]] ++;
        }
    }
}

int percolation(int *cluster, int *children, int n)
{
    /* returns, if there is a percolant cluster, the percolant cluster label*/
    int ii, jj;
    int n2 = n*n;
    int last_row = n2-n;
    for (ii=0; ii<n; ii++){
        if (cluster[ii] && (children[ii]>= n)) {
            for (jj=0; jj<n; jj++) {
                if (cluster[ii]==cluster[last_row + jj]) return cluster[ii];
            }
        }
    }
    for (ii=0; ii<n2; ii+=n) {
        if (cluster[ii] && (children[ii]>= n)) {
            for (jj=1; jj<=n; jj++) {
                if (cluster[ii]==cluster[n*jj-1]) return cluster[ii];
            }
        }
    }
    return 0;
}

/* Save data in a file*/
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