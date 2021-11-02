#include "Python/Python.h"
#include "../include/floquet.hpp"
#include "../include/stabilizer.hpp"
#include "../include/pauli_product.hpp"
#include "../include/match.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>

using namespace std;

int main()
{

    int dx = 3;
    int dy = 3;
    unsigned int lx;
    unsigned int lgz = 0;
    int n_simu = 50;
    int n_time = 100;
    string filename;
    double random_value;
    double theta0;
    double theta1;
    double measur1, measur2;

    ofstream outfile;
    filename = string("data/data-decoder-") + string("n_s=") + to_string(n_simu) + "-n_t=" + to_string(n_time) + string(".dat");
    outfile.open(filename);

    srand((unsigned)time(NULL));

    cx_dvec psi = initial_allzero(dx, dy);

    Py_Initialize();

    for (int i = 1; i < 2 * dy; i = i + 2)
    {
        lgz = lgz | (1 << (i * dx + 1));
    }

    for (int time = 1; time < 2; time++)
    {

        // cout << random_value << end./dtc l;

        apply_stabl_uniform(dx, dy, psi);

        // cout << psi[0] << endl;

        mwpm_decoding(dx, dy, psi);

        // cout << psi[0] << endl;

        if (time % 2 == 0)
        {
            measur1 = -measure_pp(0, lgz, psi);
        }
        else
        {
            measur1 = measure_pp(0, lgz, psi);
        }

        for (int i = 0; i < dx; i++)
        {
            random_value = (rand() % 200 - 100) / 1000.0;
            theta1 = 0.47 * M_PI + random_value;
            lx = 1 << (1 * dx + i);
            apply_ppr(lx, 0, theta1, psi);
        }

        mwpm_decoding(dx, dy, psi);

        if (time % 2 == 0)
        {
            measur2 = -measure_pp(0, lgz, psi);
        }
        else
        {
            measur2 = measure_pp(0, lgz, psi);
        }

        outfile << time << " " << measur1 << " " << measur2 << endl;
    }

    Py_Finalize();
    outfile.close();

    return 0;
}
