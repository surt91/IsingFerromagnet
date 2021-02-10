# Ising Ferromagnet

This simulation code in standard C simulates an Ising Ferromagnet on
different types of topologies. Currently square lattices, relative neighborhood
graphs (RNG), Gabriel graphs (GG) and minimum spanning trees (MST) are implemented.

This code was used to generate the results of
[Scientific Reports 7, 8040 (2017)](https://dx.doi.org/10.1038/s41598-017-08531-8)
and the thesis [Ising-Ferromagnet auf Ad-Hoc Netzwerken](https://hendrik.schawe.me/pdf/bachelorThesis.pdf)
(German title, but English text).

## Setup

For compilation the GNU scientific library (GSL) is needed. If it is installed systemwide,
a simple `make` should generate the executable `ising`.

## Usage

Executing `ising` without arguments will start a simulation with a set of standard parameters.
To change the parameters, command line arguments can be passed:
(The output of `ising -h` is unfortunately in German.)

```
Usage: ./ising -[hTLxNesou]
    -h     shows this text
    -v     verbose mod
    -Tx    temperature x                        (double)
    -Lx    side length x                           (int)
    -xx    seed x                                  (int)
    -Nx    x Monte Carlo sweeps                    (int)
    -ex    Equilibration time x sweeps             (int)
    -ix    save every x sweeps (autocorrelation)   (int)
    -sx    sigma x (spatial disorder parameter) (double)
    -ax    alpha x (interaction decay paramter) (double)
    -tx    Graphtype: (1: RNG, 2: Gabriel, 3: MST) (int)
    -ox    filename (max. 79 Zeichen)           (string)
    -gx    SVG filename (max. 79 Zeichen)       (string)
    -ux    starting order x (0: random, 1: all up) (int)
    -w     use Wolff algorithm
    -p     use Parallel Tempering
    -r     simulate at the percolationthreshold of the graph
    -z     gzip outputfile
```

## Technical details

The code for the simulation, including the main function,
is included in `spin.c`. Other functions and data structures are
in their own source files.
