/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "percolation.h"

int main()
{
    std::srand(1234);
    int     n           = 1000;                 // Side of Grid
    double  pc          = 0.6 ;                 // Critical Probability
    char    filename[]  = "../pic/Cluster.bin"; // File adress for saving data
    System  grid;
    grid.n  = n;
    grid.pc = pc;
    grid.percolate = 0;

    /* Allocate Memory*/
    grid.cluster    =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.classes    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.finclas    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.children   =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));

    const auto start{std::chrono::steady_clock::now()};

    /* Create Grid */
    createGrid(grid);

    /* Create Clusters */
    hoshenKopelman(grid);
    grid.percolate = percolation(grid.cluster, grid.children, grid.n);

    std::cout << grid.percolate << "\t" << grid.children[grid.percolate] << std::endl;

    /* Save Data */
    outputCluster(grid);

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};
    std::cout << elapsed_seconds.count() << " s " << std::endl;
    
    
    /* Deallocate memory */
    free(grid.cluster);
    free(grid.classes);
    free(grid.finclas);
    free(grid.children);
    
    return 0;
}