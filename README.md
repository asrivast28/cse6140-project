# Select Approaches for Solving the Travelling Salesman Problem

This project was undertaken as part of CSE 6140 - CSE Algorithms. The final report for the project can be found [here](report.pdf).

## Contributors

* Parul Awasthy (<pawasthy@gatech.edu>)
* Young Jin Kim (<ykim362@gatech.edu>)
* Ankit Srivastava (<asrivast@gatech.edu>)

## Building

### SCons

[SCons](http://www.scons.org/) is used for building the project. It is based on Python
and can be easily downloaded and installed. After installing SCons, following needs to be
executed on the command line:
```
scons [DEBUG=1]
```
This builds the project in *builds/[release|debug]* directory under the current directory and installs the final executable,
named *tspsolver[_debug]*, in the current directory.

### Make

Alternatively, the project can also be built using Makefile by executing following on the command line:
```
make
```
This builds the project in the top level directory under the current directory and installs the final executable,
named *tspsolver*, in the current directory. Note that only release version can be built using Makefile. Please use SCons,
described above, if you want to build the debug version.

#### Required Libraries

[Boost](http://www.boost.org/) libraries are required for building the project.
Installing *libboost-all-dev*, on Linux, should do the trick.


## Running

The executable can be run as follows:
```
./tspsolver[_debug] --alg [BnB | Approx | Heur| LS1 | LS2] --inst <filename.tsp> [--time <cutoff (in seconds)>] [--seed <seed number>]
```
