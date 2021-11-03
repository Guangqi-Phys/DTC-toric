#include "../include/toric.hpp"
#include "../include/stabilizer.hpp"
#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

// Checking that apply_ppr and apply_ppr_slow yields the same result.
int main(int argc, char *argv[])
{

  int dx = atoi(argv[1]);
  int dz = atoi(argv[2]);
  const int n = 4096;

  Stabilizers xchecks_bulk = stab_x_bulk(dx, dz);
  Stabilizers zchecks_bulk = stab_z_bulk(dx, dz);

  Stabilizers xchecks_bdy = stab_x_bdy(dx, dz);
  Stabilizers zchecks_bdy = stab_z_bdy(dx, dz);

  cout << "X-checks (bulk) " << endl;
  for (auto s : xchecks_bulk)
  {
    cout << "(x,y)=(" << s.x << "," << s.y << ")" << endl;
    int i = s.bitrep;
    print_surface(dx, dz, i);
    cout << endl;
  }

  cout << "Z-checks (bulk) " << endl;
  for (auto s : zchecks_bulk)
  {
    cout << "(x,y)=(" << s.x << "," << s.y << ")" << endl;
    int i = s.bitrep;
    print_surface(dx, dz, i);
    cout << endl;
  }

  cout << "X-checks (boundary) " << endl;
  for (auto s : xchecks_bdy)
  {
    cout << "(x,y)=(" << s.x << "," << s.y << ")" << endl;
    int i = s.bitrep;
    print_surface(dx, dz, i);
    cout << endl;
  }

  cout << "Z-checks (boundary) " << endl;
  for (auto s : zchecks_bdy)
  {
    cout << "(x,y)=(" << s.x << "," << s.y << ")" << endl;
    int i = s.bitrep;
    print_surface(dx, dz, i);
    cout << endl;
  }

  cout << "Logical X:" << endl;
  {
    const string str = std::bitset<n>(logical_x(dx, dz)).to_string();
    std::cout << str.substr(str.size() - dx * dz, str.size()) << ' ';
  }

  cout << endl;

  cout << "Logical Z:" << endl;
  {
    const string str = std::bitset<n>(logical_z(dx, dz)).to_string();
    std::cout << str.substr(str.size() - dx * dz, str.size());
    cout << endl;
  }

  cout << "Generating random bit string" << endl;
  int mystring = rand() % (1 << (dx * dz));
  const string mystring_str = std::bitset<n>(mystring).to_string();
  std::cout << "Mystring=" << mystring_str.substr(mystring_str.size() - dx * dz, mystring_str.size()) << endl;

  Stabilizers syndromes_x = xchecks(dx, dz);
  measure_stab_x(syndromes_x, mystring);
  cout << "X-checks (bulk): " << endl;
  for (auto s : syndromes_x)
  {
    std::cout << "Location=(" << s.x << "," << s.y << ")" << endl;
    std::cout << s.parity << endl;
  }

  Stabilizers syndromes_z = zchecks(dx, dz);
  measure_stab_z(syndromes_z, mystring);
  cout << "Z-checks (bulk): " << endl;
  for (auto s : syndromes_z)
  {
    std::cout << "Location=(" << s.x << "," << s.y << ")" << endl;
    std::cout << s.parity << endl;
  }
}
