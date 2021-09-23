#ifndef TORIC_HPP_
#define TORIC_HPP_
#include <vector>
#include "stabilizer.hpp"

// Print a bitstring, arranged on a dx x dz lattice.
void print_surface(int dx, int dz, int bitstring);

// Helper functions for converting between abstract index and 2D coordinates.
int coordinate2num(int x, int y, int dx, int dz);
int num2x(int n, int dx, int dz);
int num2y(int n, int dx, int dz);

// Stabilizers (Bulk/Boundary and X/Z)
Stabilizers stab_x_bulk(int dx, int dz);
Stabilizers stab_z_bulk(int dx, int dz);
Stabilizers stab_x_bdy(int dx, int dz);
Stabilizers stab_z_bdy(int dx, int dz);

// Stabilizers (X/Z)
Stabilizers xchecks(int dx, int dz);
Stabilizers zchecks(int dx, int dz);

// Logical operators (Only one representative)
int logical_x(int dx, int dz);
int logical_z(int dx, int dz);

// Measure stabilizers
void measure_stab_x(Stabilizers SX, int xstring);
void measure_stab_z(Stabilizers SZ, int zstring);

// Distance between stabilizers on a 2D lattice
int distance(Stabilizer s1, Stabilizer s2);

// Matching graph function
void match_graph_x(Stabilizers SX, int xstring);
void match_graph_z(Stabilizers SZ, int zstring);

#endif // TORIC_HPP_
