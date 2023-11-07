#include <iostream>
#include <stdlib.h>
#include "percolation.h"

int main()
{
    std::srand(1234);
    int     n           = 100;                  // Side of Grid
    double  pc          = 0.5 ;                 // Critical Probability
    char    filenameG[] = "../pic/Grid.txt";    // File adress for saving data
    char    filenameC[] = "../pic/Cluster.txt"; // File adress for saving data
    System  grid;
    grid.n  = n;
    grid.pc = pc;
    grid.n_cluster = 0;
    FILE *outg, *outc;
    
    outg = fopen(filenameG, "w");
    outc = fopen(filenameC, "w");

    /* Allocate Memory*/
    grid.pos        =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.state      =(bool *)malloc(grid.n*grid.n*sizeof(bool));
    grid.cluster    =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.classes    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.children   =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));

    /* Create Grid*/
    createGrid(grid);

    /* Create Clusters*/
    hoshenKopelman(grid);
    countCluster(grid);

    /* Save Data*/
    output(grid, outg);
    outputCluster(grid, outc);
    
    fclose(outg);
    fclose(outc);
    
    /* Deallocate memory*/
    free(grid.pos);
    free(grid.state);
    free(grid.cluster);
    free(grid.classes);
    free(grid.children);
    
    return 0;
}