HACC I/O
================================================================================

Copyright (c) 2012, UChicago Argonne, LLC.  All rights reserved.

Originally authored by Venkatram Vishwanath, Argonne National Laboratory.
More details available on the official [CORAL Benchmarks][coral benchmarks]
website.

Compiling
--------------------------------------------------------------------------------

HACC-IO requires only MPI and can be built from the provided makefile via

```
make CXX=mpicxx
```

Running
--------------------------------------------------------------------------------

The general syntax for `testhacc_io` is

    mpirun ./testhacc_io numparticles /path/to/outputfile

`numparticles` is the length of the arrays that each MPI rank should allocate.
HACC IO currently uses this parameter to size the following arrays:

variable | type  | size
---------|-------|---------
      xx | float | 4 bytes
      yy | float | 4 bytes
      zz | float | 4 bytes
      vx | float | 4 bytes
      vy | float | 4 bytes
      vz | float | 4 bytes
     phi | float | 4 bytes
     pid | int64 | 8 bytes
    mask | int16 | 2 bytes

So each rank will write out `numparticles` * 38 bytes worth of data.

[coral benchmarks]: https://asc.llnl.gov/CORAL-benchmarks/#hacc
