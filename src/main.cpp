/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "percolation.h"

int main(int argc, char **argv)
{
    std::srand(atoi(argv[1]));   // random seed
    int     l   = atoi(argv[2]); // Side of Grid
    double  pc  = atof(argv[3]); // Filling Probability
    int     n   = atoi(argv[4]); // Number of experiments
    int ii;
    double dens = 0.; // Percolation probability
    double sper = 0.; // Size of the spanning cluster
    double mscl = 0.; // Mean size of clusters (exc. spanning cl.) 
    System  grid;
    grid.n  = l;
    grid.pc = pc;
    grid.percolate = 0;

    /* Allocate Memory*/
    grid.cluster    =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.classes    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.finclas    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.children   =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));

    const auto start{std::chrono::steady_clock::now()};

    // Approximate probability as number of favourable outcomes per total events
    for (ii=0; ii<n; ii++) {
        /* Create Grid */
        createGrid(grid);

        /* Create Clusters */
        hoshenKopelman(grid);
        grid.percolate = percolation(grid.cluster, grid.children, grid.n);

        if (grid.percolate) {
            dens ++;
            sper += grid.percolate;
        }
        mscl += meanclustersize(grid);
    }

    dens /= n;
    sper /= n;
    mscl /= n;

    /* Save Data */
    outputCluster(grid);

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    
    std::cout << dens << "\t" << sper << "\t" << mscl << "\t" << grid.n << "\t" << elapsed_seconds.count() << std::endl;
    
    /* Deallocate memory */
    free(grid.cluster);
    free(grid.classes);
    free(grid.finclas);
    free(grid.children);
    
    return 0;
}