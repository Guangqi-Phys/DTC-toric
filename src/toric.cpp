#include "../include/toric.hpp"
#include "../include/stabilizer.hpp"
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;

/* Conventions for the qubit labeling
d=2:

0 1
2 3

d=3:

0 1 2
3 4 5
6 7 8
 
etc.
x-direction:͢͢͢ Left to right
y-direction: Up to down
 */

/* Conventions for the stabilizer checks
Bulk: From the top left corner, place X-check, Z-check, etc.

Boundary: Top and bottom are two-body Z-checks. Left and right are X-checks.

 */

void print_surface(int dx, int dz, int bitstring)
{
  const int n = 4096;
  const string str = std::bitset<n>(bitstring).to_string();
  cout << "Bit string:";
  std::cout << str.substr(str.size() - dx * dz, str.size()) << endl;

  cout << "Square grid format:" << endl;
  for (int y = 0; y < dz; y++)
  {
    for (int x = 0; x < dx; x++)
    {
      int k = coordinate2num(x, y, dx, dz);
      cout << ((bitstring & (1 << k)) >> k);
    }
    cout << endl;
  }
  cout << endl;
}

int coordinate2num(int x, int y, int dx, int dz)
{
  return x + dx * y;
}

int num2x(int n, int dx, int dz)
{
  return n % dx;
}

int num2y(int n, int dx, int dz)
{
  return n / dx;
}

Stabilizers stab_x_bulk(int dx, int dz)
{
  Stabilizers vec;
  for (int x = 0; x < dx; x++)
  {
    for (int y = 0; y < dz; y++)
    {
      if ((y % 2 == 0) && (y + 1 < dz))
      {
        if ((x % 2 == 0) && (x + 1 < dx))
        {
          int n0 = coordinate2num(x, y, dx, dz);
          int n1 = coordinate2num(x + 1, y, dx, dz);
          int n2 = coordinate2num(x, y + 1, dx, dz);
          int n3 = coordinate2num(x + 1, y + 1, dx, dz);
          vec.push_back(Stabilizer(x, y, (1 << n0) | (1 << n1) | (1 << n2) | (1 << n3)));
        }
      }
      else if ((y % 2 == 1) && (y + 1 < dz))
      {
        if ((x % 2 == 1) && (x + 1 < dx))
        {
          int n0 = coordinate2num(x, y, dx, dz);
          int n1 = coordinate2num(x + 1, y, dx, dz);
          int n2 = coordinate2num(x, y + 1, dx, dz);
          int n3 = coordinate2num(x + 1, y + 1, dx, dz);
          vec.push_back(Stabilizer(x, y, (1 << n0) | (1 << n1) | (1 << n2) | (1 << n3)));
        }
      }
    }
  }

  return vec;
}

Stabilizers stab_z_bulk(int dx, int dz)
{
  Stabilizers vec;
  for (int x = 0; x < dx; x++)
  {
    for (int y = 0; y < dz; y++)
    {
      if ((y % 2 == 0) && (y + 1 < dz))
      {
        if ((x % 2 == 1) && (x + 1 < dx))
        {
          int n0 = coordinate2num(x, y, dx, dz);
          int n1 = coordinate2num(x + 1, y, dx, dz);
          int n2 = coordinate2num(x, y + 1, dx, dz);
          int n3 = coordinate2num(x + 1, y + 1, dx, dz);
          vec.push_back(Stabilizer(x, y, (1 << n0) | (1 << n1) | (1 << n2) | (1 << n3)));
        }
      }
      else if ((y % 2 == 1) && (y + 1 < dz))
      {
        if ((x % 2 == 0) && (x + 1 < dx))
        {
          int n0 = coordinate2num(x, y, dx, dz);
          int n1 = coordinate2num(x + 1, y, dx, dz);
          int n2 = coordinate2num(x, y + 1, dx, dz);
          int n3 = coordinate2num(x + 1, y + 1, dx, dz);
          vec.push_back(Stabilizer(x, y, (1 << n0) | (1 << n1) | (1 << n2) | (1 << n3)));
        }
      }
    }
  }

  return vec;
}

Stabilizers stab_x_bdy(int dx, int dz)
{
  Stabilizers vec;

  for (int y = 0; y < dz; y++)
  {
    if ((y % 2 == 1) && ((y + 1) < dz))
    {
      int n0 = coordinate2num(0, y, dx, dz);
      int n1 = coordinate2num(0, y + 1, dx, dz);
      vec.push_back(Stabilizer(-1, y, (1 << n0) | (1 << n1)));
    }
  }

  if (dx % 2 == 0)
  {
    for (int y = 0; y < dz; y++)
    {
      if ((y % 2 == 1) && ((y + 1) < dz))
      {
        int n0 = coordinate2num(dx - 1, y, dx, dz);
        int n1 = coordinate2num(dx - 1, y + 1, dx, dz);
        vec.push_back(Stabilizer(dx, y, (1 << n0) | (1 << n1)));
      }
    }
  }
  else
  {
    for (int y = 0; y < dz; y++)
    {
      if ((y % 2 == 0) && ((y + 1) < dz))
      {
        int n0 = coordinate2num(dx - 1, y, dx, dz);
        int n1 = coordinate2num(dx - 1, y + 1, dx, dz);
        vec.push_back(Stabilizer(dx, y, (1 << n0) | (1 << n1)));
      }
    }
  }

  return vec;
}
Stabilizers stab_z_bdy(int dx, int dz)
{
  Stabilizers vec;
  for (int x = 0; x < dx; x++)
  {
    if ((x % 2 == 0) && ((x + 1) < dx))
    {
      int n0 = coordinate2num(x, 0, dx, dz);
      int n1 = coordinate2num(x + 1, 0, dx, dz);
      vec.push_back(Stabilizer(x, -1, (1 << n0) | (1 << n1)));
    }
  }

  if (dz % 2 == 0)
  {
    for (int x = 0; x < dx; x++)
    {
      if ((x % 2 == 0) && ((x + 1) < dx))
      {
        int n0 = coordinate2num(x, dz - 1, dx, dz);
        int n1 = coordinate2num(x + 1, dz - 1, dx, dz);
        vec.push_back(Stabilizer(x, dz, (1 << n0) | (1 << n1)));
      }
    }
  }
  else
  {
    for (int x = 0; x < dx; x++)
    {
      if ((x % 2 == 1) && ((x + 1) < dx))
      {
        int n0 = coordinate2num(x, dz - 1, dx, dz);
        int n1 = coordinate2num(x + 1, dz - 1, dx, dz);
        vec.push_back(Stabilizer(x, dz, (1 << n0) | (1 << n1)));
      }
    }
  }

  return vec;
}

Stabilizers xchecks(int dx, int dz)
{
  Stabilizers bulk = stab_x_bulk(dx, dz);
  Stabilizers bdy = stab_x_bdy(dx, dz);

  bulk.insert(bulk.end(), bdy.begin(), bdy.end());
  return bulk;
}

Stabilizers zchecks(int dx, int dz)
{
  Stabilizers bulk = stab_z_bulk(dx, dz);
  Stabilizers bdy = stab_z_bdy(dx, dz);

  bulk.insert(bulk.end(), bdy.begin(), bdy.end());
  return bulk;
}

int logical_x(int dx, int dz)
{
  int out = 0;
  for (int x = 0; x < dx; x++)
  {
    out |= (1 << (coordinate2num(x, 0, dx, dz)));
  }
  return out;
}

int logical_z(int dx, int dz)
{
  int out = 0;
  for (int y = 0; y < dz; y++)
  {
    out |= (1 << (coordinate2num(0, y, dx, dz)));
  }
  return out;
}

// Convention is the bulk stabilizers first and then the boundary
// stabilizers.

void measure_stab_x(Stabilizers SX, int xstring)
{

  for (auto v : SX)
  {
    v.parity = __builtin_popcount(xstring & v.bitrep) % 2;
  }
}

void measure_stab_z(Stabilizers SZ, int zstring)
{

  for (auto v : SZ)
  {
    v.parity = __builtin_popcount(zstring & v.bitrep) % 2;
  }
}

int distance(Stabilizer s1, Stabilizer s2)
{
  int a, b;

  a = s1.x - s2.x;
  b = s1.y - s2.y;

  //
  if ((a + b) % 2 != 0)
  {
    cout << "Warning: You are probably using stabilizer checks of different types." << endl;
  }

  return abs((a + b) / 2) + abs((a - b) / 2);
}

void match_graph_x(Stabilizers SX, int xstring)
{
  measure_stab_x(SX, xstring);

  Stabilizers SX_marked;
  // Find the indices of the stabilizers which detected an error.
  for (auto s : SX)
  {
    if (s.parity == true)
    {
      SX_marked.push_back(s);
    }
  }

  // nodes_a

  // nodes_b

  // weights

  // Matching Graph should contain n_nodes, mates, n_edges, nodes_a, nodes_b, weights
}

void match_graph_z(Stabilizers SX, int xstring)
{
}
