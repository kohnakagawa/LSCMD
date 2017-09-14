MDLScalculator
=============
[![Build Status](https://travis-ci.org/kohnakagawa/md_local_stress.svg?branch=master)](https://travis-ci.org/kohnakagawa/md_local_stress)
A local stress calculation library for molecular dynamics simulations written in C++11.
(This software is currently in the beta stage.)

## Supported methods
* Irving-Kirkwood-Noll procedure for pairwise forces.
* Central Force Decomposition (CFD) for 3,4 body forces.

## Dependency
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

## Examples
### Example 1

* sample/mdstep/step6.5/observer.cpp

```c++
#include "ls_calculator.hpp"

...

void
Observer::local_pressure(Variables *vars, std::vector<Pair> &pairs) {
  // specify box type and space division
  static LS::LSCalculator<double> lscalculator({0.0, 0.0, 0.0}, {Lx, Ly, Lz},
                                               LS::BoundaryType::PERIODIC_XYZ,
                                               {1, 1, 160});
  Atom *atoms = vars->atoms.data();
  // calculate kinetic term of local stress
  const int N = vars->number_of_atoms();
  for (int k = 0; k < N; k++) {
    lscalculator.calcLocalStressKin({atoms[k].qx, atoms[k].qy, atoms[k].qz},
                                    {atoms[k].px, atoms[k].py, atoms[k].pz},
                                    1.0);
  }

  // calculate potential term of local stress
  const int pp = pairs.size();
  for (int k = 0; k < pp; k++) {
    const int i = pairs[k].i;
    const int j = pairs[k].j;
    double dx = atoms[j].qx - atoms[i].qx;
    double dy = atoms[j].qy - atoms[i].qy;
    double dz = atoms[j].qz - atoms[i].qz;
    adjust_periodic(dx, dy, dz);
    double r2 = (dx * dx + dy * dy + dz * dz);
    if (r2 > CL2) continue;
    double r6 = r2 * r2 * r2;
    double df = (24.0 * r6 - 48.0) / (r6 * r6 * r2);
    lscalculator.calcLocalStressPot2({atoms[j].qx, atoms[j].qy, atoms[j].qz},
                                     {atoms[i].qx, atoms[i].qy, atoms[i].qz},
                                     {-df * dx, -df * dy, -df * dz});
  }

  // save
  lscalculator.nextStep();
}

```

## History
* 2017/Sep/10 first beta version
