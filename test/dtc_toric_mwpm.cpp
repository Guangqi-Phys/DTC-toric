#include "include/floquet.hpp"
#include "include/stabilizer.hpp"
#include "include/pauli_product.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

using namespace std;

int main()
{

    int dx = 3;
    int dy = 3;
    unsigned int lx;
    unsigned int lgz = 0;
    unsigned int m_stabl;
    double random_value;
    double theta0;
    double theta1;
    double measur1, measur2;

    ofstream outfile;
    outfile.open("data.dat");
    srand((unsigned)time(NULL));

    cx_dvec psi = initial_allzero(dx, dy);

    for (int i = 1; i < 2 * dy; i = i + 2)
    {
        lgz = lgz | (1 << (i * dx + 1));
    }

    for (int time = 0; time < 200; time++)
    {

        // cout << random_value << endl;

        apply_stabl(dx, dy, psi);
        measure_stabl(dx, dy, m_stabl, psi);

        measur1 = measure_pp(0, lgz, psi);

        for (int i = 1; i < dx; i++)
        {
            random_value = (rand() % 200 - 100) / 1000.0;
            theta1 = M_PI / 2 + random_value;
            lx = 1 << (1 * dx + i);
            apply_ppr(lx, 0, theta1, psi);
        }

        measur2 = measure_pp(0, lgz, psi);

        outfile << time << " " << measur1 << " " << measur2 << endl;
    }
    outfile.close();
    return 0;
}
