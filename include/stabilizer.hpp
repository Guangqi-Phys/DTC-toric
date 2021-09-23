#ifndef STABILIZER_HPP_
#define STABILIZER_HPP_

#include <vector>

class Stabilizer{
public:
  int x;
  int y;
  int bitrep;
  bool parity;
  Stabilizer(int xx, int yy, int bb);
};

typedef std::vector<Stabilizer> Stabilizers;
#endif // STABILIZER_HPP_
