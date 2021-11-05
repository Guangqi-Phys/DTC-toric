#ifndef MATCH_HPP_
#define MATCH_HPP_

void mwpm_decoding(int dx, int dy, double threshold, cx_dvec &psi);
int mwpm_python(int dx, int dy, int n_psi);
void get_correction(int dx, int dy, cx_dvec &psi, cx_dvec &corr);
int valueAtbit(int num, int bit);

// int infty();
// void mwpm(int n_nodes, int mates[], int n_edges, int nodes_a[], int nodes_b[], int weights[]);

// Label each node by a tuple consisting of the x- and the y-coordinate.

// Taxicab distance

#endif // MATCH_HPP_
