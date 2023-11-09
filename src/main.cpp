#include <iostream>
#include <stdlib.h>
#include "percolation.h"

int main()
{
    std::srand(1234);
    int     n           = 100;                  // Side of Grid
    double  pc          = 0.5 ;                 // Critical Probability
    char    filename[]  = "../pic/Cluster.txt"; // File adress for saving data
    System  grid;
    grid.n  = n;
    grid.pc = pc;
    FILE *out;
    
    out = fopen(filename, "w");


    /* Allocate Memory*/
    grid.cluster    =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.classes    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.finclas    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.children   =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));

    /* Create Grid*/
    createGrid(grid);

    /* Create Clusters*/
    hoshenKopelman(grid);

    /* Save Data*/
    outputCluster(grid, out);
    
    fclose(out);
    
    /* Deallocate memory*/
    free(grid.cluster);
    free(grid.classes);
    free(grid.finclas);
    free(grid.children);
    
    return 0;
}