/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <math.h> 

#define RAND() ((double)rand()/(double)(RAND_MAX))

struct SystemGrid
{
    int     n;          // Side of the squared grid
    double  pc;         // Filling Probability
    int     percolate;  // If there exists, it contains the percolate cluster label.
    int     *cluster;   // Cluster's label
    int     *classes;   // 1D Array for cluster's alias (Temporal)
    int     *finclas;   // 1D Array for cluster's alias (Equivalente classes joined)
    long int*children;  // Children per cluster
    double  *xcm;       // Center of mass x
    double  *ycm;       // center of mass y
    double  *InerMom;   // Moment of inertia
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
        grid.xcm[pp]      = 0.;
        grid.ycm[pp]      = 0.;
        grid.InerMom[pp]  = 0.;
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

    // Assign first equivalent class for each cluster

    for (pc=0; pc <n2; pc++){
        if (grid.cluster[pc]) {
            x = find(grid.classes, grid.cluster[pc]);
            if (grid.finclas[x] == 0) {
                grid.finclas[0] ++;
                grid.finclas[x] = grid.finclas[0];
            }
            grid.cluster[pc] = grid.finclas[x];
            grid.children[grid.cluster[pc]] ++;
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
    int s   = grid.children[1];
    int Ns  = 0;
    long int A   = 0;
    long int B   = 0;
    int mc  = grid.finclas[0];
    int sp  = grid.children[grid.percolate];
    double ms = 0.0;
    if (mc > 1) {
        std::sort(grid.children+1, grid.children+mc+1);
        for(ii=1; ii<=mc+1; ii++) {
            if (s==grid.children[ii]) Ns ++;
            else {
                A += s*s*Ns;
                B += s*Ns;
                Ns = 1;
                s  = grid.children[ii];
            }
        }
        A -= sp*sp;
        B -= sp;
        ms = (double)A/(double)B;
    }
    return ms;
}

double correlationlength(System &grid)
{
    int ii;
    int n  = grid.n;
    int n2 = n*n;

    // Calculate Correlation Length
    for (ii=0; ii <n2; ii++){
        if (grid.cluster[ii] && (grid.cluster[ii] != grid.percolate)) {
            grid.xcm[grid.cluster[ii]] += ii % n;
            grid.ycm[grid.cluster[ii]] += ii / n;
        }
    }

    double x_coor, y_coor, inv_size;
    for (ii=1; ii<= grid.finclas[0]; ii++) {
        inv_size      = 1./grid.children[ii];
        grid.xcm[ii] *= inv_size;
        grid.ycm[ii] *= inv_size;
    }

    // Calculate Moment of Inertia
    for (ii=0; ii < n2; ii++) {
        if (grid.cluster[ii] && (grid.cluster[ii] != grid.percolate)) {
            x_coor = ii % n - grid.xcm[grid.cluster[ii]];
            y_coor = ii / n - grid.ycm[grid.cluster[ii]];
            grid.InerMom[grid.cluster[ii]] += x_coor*x_coor + y_coor*y_coor;
        }
    }

    long int sum1 = 0;
    long int sum2 = 0;
    for (ii=1; ii <= grid.finclas[0]; ii++) {
        sum1 += grid.children[ii]*grid.InerMom[ii];
        sum2 += grid.children[ii]*grid.children[ii]; 
    }
    return sqrt((double) sum1 / (double) sum2);
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