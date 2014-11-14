## Contributors

* Parul Awasthy (<pawasthy@gatech.edu>)
* Young Jin Kim (<ykim362@gatech.edu>)
* Ankit Srivastava (<asrivast@gatech.edu>)

## Building

[SCons](http://www.scons.org/) is used for building the project. It is based on Python
and can be easily downloaded and installed. After installing SCons, following needs to be
executed on the command line:
```
scons [DEBUG=1]
```
This builds the project in *builds/[release|debug]* directory under the current directory and installs the final executable,
named *tspsolver[_debug]*, in the current directory.


#### Required Libraries

[Boost](http://www.boost.org/) libraries are required for building the project.
Installing *libboost-all-dev*, on Linux, should do the trick.


## Running

The executable can be run as follows:
```
./tspsolver[_debug] --alg [BnB | Approx | Heur| LS1 | LS2] --inst <filename.tsp> [--time <cutoff (in minutes)>]
```