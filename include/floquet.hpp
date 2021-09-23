#ifndef FLOQUET_HPP_
#define FLOQUET_HPP_

#include "qsim.hpp"

void tc_stab_x(double theta, int dx, int dz, cx_dvec &psi);
void tc_stab_z(double theta, int dx, int dz, cx_dvec &psi);

void tc_logical_x(int dx, int dz, cx_dvec &psi);
void tc_logical_z(int dx, int dz, cx_dvec &psi);

cx_dvec initial_allzero(int dx, int dz);
cx_dvec initial_allplus(int dx, int dz);

double measure_logical_x(int dx, int dz, cx_dvec &psi);
double measure_logical_z(int dx, int dz, cx_dvec &psi);

// Perturbations
void global_x(double theta, int dx, int dz, cx_dvec &psi);
void global_z(double theta, int dx, int dz, cx_dvec &psi);

#endif // FLOQUET_HPP_
