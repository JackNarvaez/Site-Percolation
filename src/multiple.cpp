/******************************************************************************
 ***          Side Percolation using the Hoshen Kopelman algorithm         ***
 ***          Written by Jacksen Narvaez, 2023.                            ***
******************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <mpi.h>

#include "percolation.h"

int main(int argc, char **argv)
{
    int pId;                     // Process rank
    int nP;                      // Number of processes
    int root{0};                 // Root process
    
    int    l    = atoi(argv[2]); // Side of Grid
    double dp   = atof(argv[3]); // Filling Probability
    double pi   = atof(argv[4]); // Initial probability 
    double pf   = atof(argv[5]); // Final probability
    int    n    = atoi(argv[6]); // Number of experiments
    int    it   = int((pf-pi)/dp) + 1;  // Iterations
    double dens = 0.;            // LOCAL percolation's cases
    double sper = 0.;            // LOCAL normilised size of the spanning cluster
    double mscl = 0.;            // LOCAL normilized mean size of clusters (exc. spanning cl.)
    double crln = 0.;            // LOCAL correlation length
    double Dens = 0.;            // GLOBAL Percolation probability
    double Sper = 0.;            // GLOBAL Density of the spanning cluster
    double Mscl = 0.;            // GLOBAL Mean size of clusters (exc. spanning cl.) 
    double Crln = 0.;            // GLOBAL Correlation Length 
    
    System  grid;
    grid.n  = l;

    /* Allocate Memory*/
    grid.cluster    =(int  *)malloc(grid.n*grid.n*sizeof(int));
    grid.classes    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.finclas    =(int  *)malloc((grid.n*grid.n/4)*sizeof(int));
    grid.children   =(long int  *)malloc((grid.n*grid.n/4)*sizeof(long int));
    grid.xcm        =(double *)malloc((grid.n*grid.n/4)*sizeof(double));
    grid.ycm        =(double *)malloc((grid.n*grid.n/4)*sizeof(double));
    grid.InerMom    =(double *)malloc((grid.n*grid.n/4)*sizeof(double));;

    /*Initializes MPI*/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nP);
    MPI_Comm_rank(MPI_COMM_WORLD, &pId);

    std::srand(atoi(argv[1])+pId); // random seed

    int start   = pId*(double(n)/nP);
    int end     = (pId+1)*(double(n)/nP);
    int expts   = end-start;       // Number of local experiments
    double ninv = 1./n;
    double norm = ninv*1./(grid.n*grid.n);
    int ii, jj;
    double t_ini, t_end, time_ms;


    grid.pc = pi;                // Start filling probability

    for (jj=0; jj<it; jj++) {
        
        t_ini = MPI_Wtime();
        
        grid.percolate = 0;

        // Approximate probability as number of favourable outcomes per total events
        for (ii=0; ii<expts; ii++) {

            /* Create Grid */
            createGrid(grid);

            /* Create Clusters */
            hoshenKopelman(grid);

            grid.percolate = percolation(grid.cluster, grid.children, grid.n);

            if (grid.percolate) {
                dens += ninv;
                sper += norm*grid.children[grid.percolate];
            }
            crln += ninv*correlationlength(grid);
            mscl += ninv*meanclustersize(grid);
        }

        MPI_Reduce(&dens, &Dens, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
        MPI_Reduce(&sper, &Sper, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
        MPI_Reduce(&mscl, &Mscl, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
        MPI_Reduce(&crln, &Crln, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

        /* Save Data */
        if (pId == root) {
            outputCluster(grid);
            t_end = MPI_Wtime();
            time_ms = t_end-t_ini;

            std::cout << grid.pc << "\t" << Dens << "\t" << Sper << "\t" << Mscl << "\t" << Crln << "\t" << grid.n << "\t" << time_ms << std::endl;
        }
        grid.pc += dp;        // Update filling probability
        dens = 0.;            // LOCAL percolation's cases
        sper = 0.;            // LOCAL normilised size of the spanning cluster
        mscl = 0.;            // LOCAL normilized mean size of clusters (exc. spanning cl.)
        crln = 0.;            // LOCAL Correlation Length 
        Dens = 0.;            // GLOBAL Percolation probability
        Sper = 0.;            // GLOBAL Density of the spanning cluster
        Mscl = 0.;            // GLOBAL Mean size of clusters (exc. spanning cl.) 
        Crln = 0.;            // GLOBAL Correlation Length 
    }

    /*Finalizes MPI*/
    MPI_Finalize();

    /* Deallocate memory */
    free(grid.cluster);
    free(grid.classes);
    free(grid.finclas);
    free(grid.children);
    free(grid.xcm);
    free(grid.ycm);
    free(grid.InerMom);
    
    return 0;
}