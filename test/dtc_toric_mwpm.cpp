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

int main(int argc, char *argv[])
{

    int dx = 3;
    int dy = 3;
    // double threshold = 0.01;
    unsigned int lx;
    unsigned int lgz = 0;
    int const n_time = 500;
    int const n_simu = 200;
    string filename;
    double random_value;
    double theta0;
    double theta1;
    double measur1, measur2;
    double measur1list[n_time] = {0};
    double measur2list[n_time] = {0};

    ofstream outfile;
    filename = string("data/data_decoder") + string("_ns=") + to_string(n_simu) + "_nt=" + to_string(n_time) + string(".dat");
    outfile.open(filename);

    srand((unsigned)time(NULL));

    cx_dvec psi = initial_allzero(dx, dy);
    cx_dvec corr(1 << (2 * dx * dy), 0.0);

    for (int i = 1; i < 2 * dy; i = i + 2)
    {
        lgz = lgz | (1 << (i * dx + 1));
    }

    Py_Initialize();

    get_correction(dx, dy, psi, corr);

    // define logical z

    for (int simu = 0; simu < n_simu; simu++)
    {
        psi = initial_allzero(dx, dy);
        for (int time = 1; time < n_time; time++)
        {

            // apply stablizer
            apply_stabl_uniform(dx, dy, psi);

            // do mwpm decoder for psi
            // mwpm_decoding(dx, dy, threshold, psi);

            // measure logical z operator
            if (time % 2 == 0)
            {
                measur1 = -measure_pp_corr(0, lgz, psi, corr);
            }
            else
            {
                measur1 = measure_pp_corr(0, lgz, psi, corr);
            }

            measur1list[time] = measur1list[time] + measur1 / n_simu;

            // apply logical x operator
            for (int i = 0; i < dx; i++)
            {
                random_value = (rand() % 200 - 100) / 1000.0;
                theta1 = 0.47 * M_PI + random_value;
                lx = 1 << (1 * dx + i);
                apply_ppr(lx, 0, theta1, psi);
            }

            // do mwpm decoder
            // mwpm_decoding(dx, dy, threshold, psi);

            // measure logical z operator
            if (time % 2 == 0)
            {
                measur2 = -measure_pp_corr(0, lgz, psi, corr);
            }
            else
            {
                measur2 = measure_pp_corr(0, lgz, psi, corr);
            }

            measur2list[time] = measur2list[time] + measur2 / n_simu;
        }
    }

    for (int time = 1; time < n_time; time++)
    {
        outfile << time << " " << measur1list[time] << " " << measur2list[time] << endl;
    }

    Py_Finalize();
    outfile.close();

    return 0;
}
