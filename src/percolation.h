/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <fstream>
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
    long int    *children;  // Children per cluster
};

typedef struct SystemGrid System;

void createGrid(System &grid)
{
    // Initialise the grid
    int pp, n, n2, cm;
    n  = grid.n;
    n2 = n*n;
    cm = n2/4;
    for (pp=0; pp<n2; pp++) grid.cluster[pp] = (RAND()<=grid.pc);

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
    return classes[find(classes, above)] = find(classes, left);
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

int percolation(int *cluster, long int *children, int n)
{
    /* returns, if there is a spanning cluster, the spanning cluster label*/
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

double meanclustersize(System &grid)
{
    /* Calculate the mean size of clusters in the grid.
       Excluding the spanning cluster (if it exists).*/
    int ii;
    int mc = grid.finclas[0];
    double ms = 0.0;
    if (mc > 1) {
        for (ii=1; ii<mc; ii++) {
            if (ii != grid.percolate) ms += grid.children[grid.finclas[ii]];
        }
        return ms/(mc-1);
    } else {
        return ms;
    }
}

/* Save data in a binary file*/
void outputCluster(System &grid)
{
    std::ofstream file;
    file.open("./pic/Cluster", std::ios::out | std::ios::binary);
    int size = grid.n*grid.n*sizeof(int);
    file.write((char*)grid.cluster, size);
    file.close();
}