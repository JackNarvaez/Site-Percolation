Site Percolation
=====
> 2D Site Percolation

2D site Percolation using the Hoshen Kopelman algorithm, with the Union-Find generalization. For this, a nxn grid is filled randomly as occupied (1) or unoccupied (0). If there exists a cluster (set of connected sites) that spans from one side to the opposite side, then the system percolates.

<figure>
  <img src="./pic/Grid.png" width=350 height=350 aling="center" title="Grid">
  <figcaption>Fig.1 - Random grid for P=0.59 and L=15000. </figcaption>
</figure>

<figure>
  <img src="./pic/ZoomCluster.png" width=350 height=350 aling="center" title="Cluster">
  <figcaption>Fig.2 - Clusters for P=0.59 and L=15000. Zoom plot shows the fractal geometry. </figcaption>
</figure>

## Compilation

Clone this repository on your machine. Then, navigate to it and run the command `make` in your terminal. That's all, now you can run the executable via

```./percolation seed l pc n```

where `seed` is the seed for the random number generator, `l` is the grid's side, `pc` is the filling probability, and `n` is the number of experiments.
The code uses the -O3 Flag to improve performance. Additionally, it is parallelised using the MPI library (run it with `mpirun`).

To visualize the clusters obtained, one can use the command `make plot`.

If you need to obtain results not only for a single probability value, but as a function of `pc`, you can use the command:

`./percstats seed l dp pi pf n`

In this case, `dp` refers to the probability step for calculating the function from `pi` to `pf`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
