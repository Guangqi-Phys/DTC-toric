#include "../include/qsim.hpp"
#include "../include/floquet.hpp"
#include "../include/pauli_product.hpp"
#include "../include/toric.hpp"
#include "../include/stabilizer.hpp"
#include <cmath>
#include <stdlib.h>

using namespace std;

void tc_stab_x(double theta, int dx, int dz, cx_dvec &psi)
{
  Stabilizers checks = xchecks(dx, dz);

  for (auto s : checks)
  {
    apply_ppr(s.bitrep, 0, theta, psi);
  }
}

void tc_stab_z(double theta, int dx, int dz, cx_dvec &psi)
{
  Stabilizers checks = zchecks(dx, dz);

  for (auto s : checks)
  {
    apply_ppr(0, s.bitrep, theta, psi);
  }
}

void tc_logical_x(int dx, int dz, cx_dvec &psi)
{
  double theta = 1.57079632679;
  apply_ppr(logical_x(dx, dz), 0, theta, psi);
}

void tc_logical_z(int dx, int dz, cx_dvec &psi)
{
  double theta = 1.57079632679;
  apply_ppr(0, logical_z(dx, dz), theta, psi);
}

cx_dvec initial_allzero(int dx, int dz)
{
  cx_dvec psi(1 << (2 * dx * dz), 0.0);
  psi[0] = 1.0;
  return psi;
}

cx_dvec initial_allplus(int dx, int dz)
{
  cx_dvec psi(1 << (2 * dx * dz), 1.0 / sqrt(pow(2.0, 2 * dx * dz)));
  return psi;
}

double measure_logical_x(int dx, int dz, cx_dvec &psi)
{
  return measure_pp(logical_x(dx, dz), 0, psi);
}

double measure_logical_z(int dx, int dz, cx_dvec &psi)
{
  return measure_pp(0, logical_z(dx, dz), psi);
}

void global_x(double theta, int dx, int dz, cx_dvec &psi)
{
  for (int i = 0; i < dx * dz; i++)
  {
    apply_ppr(1 << i, 0, theta, psi);
  }
}

void global_z(double theta, int dx, int dz, cx_dvec &psi)
{
  for (int i = 0; i < dx * dz; i++)
  {
    apply_ppr(0, 1 << i, theta, psi);
  }
}
