#ifndef QSIM_PAULI_PRODUCT_HPP_
#define QSIM_PAULI_PRODUCT_HPP_

#include "qsim.hpp"

void apply_pauli(int k, bool x, bool z, cx_dvec &psi);
void apply_pauli_slow(int k, bool x, bool z, cx_dvec &psi);
void apply_ppr(unsigned int x, unsigned int z, double theta, cx_dvec &psi);
void apply_ppr_slow(unsigned int x, unsigned int z, double theta, cx_dvec &psi);
double measure_pp(unsigned int x, unsigned int z, cx_dvec &psi);
double measure_pp_slow(unsigned int x, unsigned int z, cx_dvec &psi);
void apply_stabl_normal(int dx, int dy, cx_dvec &psi);
void apply_stabl_uniform(int dx, int dy, cx_dvec &psi);
double measure_prob_positive(unsigned int x, unsigned int z, cx_dvec &psi);
double measure_pp_corr(unsigned int x, unsigned int z, cx_dvec &psi, cx_dvec &corr);
void apply_stabl_evolve(int dx, int dy, double theta0, cx_dvec &psi);

#endif // QSIM_PAULI_PRODUCT_HPP_
