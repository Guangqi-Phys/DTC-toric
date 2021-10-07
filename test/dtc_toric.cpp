#include "../include/floquet.hpp"
#include "../include/stabilizer.hpp"
#include "../include/pauli_product.hpp"
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
    double random_value;
    double theta1_uni;
    double measur1, measur2;

    int distribution = 1; // 1 for normal distribution error
                          // 0 for uniform distribution erroe

    ofstream outfile;
    outfile.open("data.dat");

    std::default_random_engine gen;
    std::normal_distribution<double> theta1(0.47 * M_PI, 0.02 * M_PI);

    srand((unsigned)time(NULL));

    cx_dvec psi = initial_allzero(dx, dy);

    for (int i = 1; i < 2 * dy; i = i + 2)
    {
        lgz = lgz | (1 << (i * dx + 1));
    }

    for (int time = 1; time < 1000; time++)
    {

        // cout << random_value << endl;
        if (distribution == 1)
        {
            apply_stabl_normal(dx, dy, psi);
        }
        else
        {
            apply_stabl_uniform(dx, dy, psi);
        }

        if (time % 2 == 0)
        {
            measur1 = -measure_pp(0, lgz, psi);
        }
        else
        {
            measur1 = measure_pp(0, lgz, psi);
        }

        for (int i = 1; i < dx; i++)
        {
            lx = 1 << (1 * dx + i);
            if (distribution == 1)
            {
                apply_ppr(lx, 0, theta1(gen), psi);
            }
            else
            {
                random_value = (rand() % 200 - 100) / 1000.0;
                theta1_uni = 0.47 * M_PI + random_value;
                apply_ppr(lx, 0, theta1_uni, psi);
            }
        }

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
    outfile.close();
    return 0;
}
