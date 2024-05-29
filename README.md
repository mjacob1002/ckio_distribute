# Info
This experiment is meant to show the speed difference between reading something from disk and sending it over the network in supercomputing clusters. We specifically measure zero copy vs ifstream. Additionally, make sure to run this experiment with only 2 nodes, 1 PE per node. 

## Running the benchmark
./charmrun +p2 data <filenae>, where filename is the file that we are going to read in entirely, and then distribute over the network entirely.
