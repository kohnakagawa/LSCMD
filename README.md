[![Build Status](https://travis-ci.org/kohnakagawa/md_local_stress.svg?branch=master)](https://travis-ci.org/kohnakagawa/md_local_stress)
[![codecov](https://codecov.io/gh/kohnakagawa/md_local_stress/branch/master/graph/badge.svg)](https://codecov.io/gh/kohnakagawa/md_local_stress)

MDLScalculator
=============
A local stress calculation library for molecular dynamics simulations written in C++11.
(This software is currently in the beta stage.)

## Supported methods
* Irving-Kirkwood-Noll procedure for pairwise forces.
* Central Force Decomposition (CFD) for 3,4 body forces.

## Dependency
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)

## Examples
### Example 1

* sample/mdstep/local_stress/observer.cpp

```c++
#include "ls_calculator.hpp"

...

void
Observer::local_pressure(Variables *vars) {
  Atom *atoms = vars->atoms.data();
  const double mass = 1.0;
  static auto lscalculator = LS::CalculatorFactory<double>::create({0.0, 0.0, 0.0}, {Lx, Ly, Lz},
                                                                   LS::BoundaryType::PERIODIC_XYZ,
                                                                   {1, 1, 240},
                                                                   {"Kinetic", "LJ"});
  // kinetic term
  const int N = vars->number_of_atoms();
  for (int k = 0; k < N; k++) {
    lscalculator->calcLocalStressKin({atoms[k].qx, atoms[k].qy, atoms[k].qz},
                                     {atoms[k].px, atoms[k].py, atoms[k].pz},
                                     mass,
                                     0);
  }

  // potential term
  const int *neighbor_list = vars->neighbor_list.data();
  const int *i_position    = vars->i_position.data();
  const int *j_count       = vars->j_count.data();
  for (int i = 0; i < N; i++) {
    const double qix = atoms[i].qx;
    const double qiy = atoms[i].qy;
    const double qiz = atoms[i].qz;
    const int ip = i_position[i];
    for (int k = 0; k < j_count[i]; k++) {
      const int j = neighbor_list[ip + k];
      const double qjx = atoms[j].qx;
      const double qjy = atoms[j].qy;
      const double qjz = atoms[j].qz;
      double dx = qjx - qix;
      double dy = qjy - qiy;
      double dz = qjz - qiz;
      adjust_periodic(dx, dy, dz);
      const double r2 = (dx * dx + dy * dy + dz * dz);
      if (r2 > CL2)continue;
      const double r6 = r2 * r2 * r2;
      const double df = (24.0 * r6 - 48.0) / (r6 * r6 * r2);
      lscalculator->calcLocalStressPot2NoCheck({qix, qiy, qiz},
                                               {qjx, qjy, qjz},
                                               {df * dx, df * dy, df * dz},
                                               {-df * dx, -df * dy, -df * dz},
                                               1);
    }
  }
  lscalculator->nextStep();
}

```

### Example 2

* sample/mdstep/local\_stress\_omp/observer.cpp

``` c++

#include "observer.hpp"

...

Observer::Observer() {
  lscalculators = LS::CalculatorFactory<double>::createOMP(omp_get_max_threads(),
                                                           {0.0, 0.0, 0.0}, {Lx, Ly, Lz},
                                                           LS::BoundaryType::PERIODIC_XYZ,
                                                           {1, 1, 240},
                                                           {"Kinetic", "LJ"});
}
//------------------------------------------------------------------------
Observer::~Observer() {
  LS::LSHelpers<double>::saveLocalStressDistOMP(lscalculators);
}
//------------------------------------------------------------------------

...

void
Observer::local_pressure(Variables *vars) {
  Atom *atoms = vars->atoms.data();
  const double mass = 1.0;

#pragma omp parallel
  {
    // kinetic term
    const auto tid = omp_get_thread_num();
    const int N = vars->number_of_atoms();
#pragma omp for nowait
    for (int k = 0; k < N; k++) {
      lscalculators[tid]->calcLocalStressKin({atoms[k].qx, atoms[k].qy, atoms[k].qz},
                                             {atoms[k].px, atoms[k].py, atoms[k].pz},
                                             mass,
                                             0);
    }

    // potential term
    const int *neighbor_list = vars->neighbor_list.data();
    const int *i_position    = vars->i_position.data();
    const int *j_count       = vars->j_count.data();
#pragma omp for nowait
    for (int i = 0; i < N; i++) {
      const double qix = atoms[i].qx;
      const double qiy = atoms[i].qy;
      const double qiz = atoms[i].qz;
      const int ip = i_position[i];
      for (int k = 0; k < j_count[i]; k++) {
        const int j = neighbor_list[ip + k];
        const double qjx = atoms[j].qx;
        const double qjy = atoms[j].qy;
        const double qjz = atoms[j].qz;
        double dx = qjx - qix;
        double dy = qjy - qiy;
        double dz = qjz - qiz;
        adjust_periodic(dx, dy, dz);
        const double r2 = (dx * dx + dy * dy + dz * dz);
        if (r2 > CL2)continue;
        const double r6 = r2 * r2 * r2;
        const double df = (24.0 * r6 - 48.0) / (r6 * r6 * r2);
        lscalculators[tid]->calcLocalStressPot2NoCheck({qix, qiy, qiz},
                                                       {qjx, qjy, qjz},
                                                       {df * dx, df * dy, df * dz},
                                                       {-df * dx, -df * dy, -df * dz},
                                                       1);
      }
    }
    lscalculators[tid]->nextStep();
  }
}

```

## History
* 2017/Sep/10 first beta version
