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

int main(int argc, char *argv[])
{

    int dx = 3;
    int dy = 2;
    int const n_time = 100;
    int const n_simu = 50;
    int nq = dx * dy * 2;
    unsigned int lx;
    unsigned int lgz = 0;
    double theta1_uni;
    double theta2;
    double shift;
    double error_rate;
    string filename;
    double measur1, measur2;
    double measur1list[n_time] = {0};
    double measur2list[n_time] = {0};

    int distribution = 0; // 1 for normal distribution error
                          // 0 for uniform distribution error
    const int s = 108;

    mt19937_64 engine1(s);
    mt19937_64 engine2(s + 1);

    error_rate = 0.02;
    shift = 0.01;

    uniform_real_distribution<double> dist1(-0.01, 0.01);
    uniform_real_distribution<double> dist2(-0.1, 0.1);

    ofstream outfile;
    filename = string("data/nodecoder_") + string("dx=") + to_string(dx) + string("_ns=") + to_string(n_simu) + "_nt=" + to_string(n_time) + string(".dat");

    outfile.open(filename);

    std::default_random_engine gen;
    std::normal_distribution<double> theta1(0.47 * M_PI, 0.02 * M_PI);

    srand((unsigned)time(NULL));

    for (int i = 1; i < 2 * dy; i = i + 2)
    {
        lgz = lgz | (1 << (i * dx + 1));
    }

    for (int simu = 0; simu < n_simu; simu++)
    {
        cx_dvec psi = initial_allzero(dx, dy);
        for (int time = 1; time < n_time; time++)
        {

            if (distribution == 1)
            {
                apply_stabl_normal(dx, dy, psi);
            }
            else
            {
                apply_stabl_uniform(dx, dy, psi);
            }

            // add errors
            for (int i = 0; i < nq; i++)
            {
                lx = 1 << i;
                // random_value = dist1(engine1) / 100.0;
                theta2 = error_rate * M_PI + dist1(engine1);
                apply_ppr(lx, 0, theta2, psi);
            }

            if (time % 2 == 0)
            {
                measur1 = -measure_pp(0, lgz, psi);
            }
            else
            {
                measur1 = measure_pp(0, lgz, psi);
            }

            measur1list[time] = measur1list[time] + measur1 / n_simu;

            for (int i = 0; i < dx; i++)
            {
                lx = 1 << (1 * dx + i);
                if (distribution == 1)
                {
                    apply_ppr(lx, 0, theta1(gen), psi);
                }
                else
                {
                    // random_value = (dist2(engine2) % 200 - 100) / 1000.0;
                    theta1_uni = 0.5 * M_PI + shift * M_PI + dist2(engine2);
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

            measur2list[time] = measur2list[time] + measur2 / n_simu;
        }
    }

    for (int time = 1; time < n_time; time++)
    {
        outfile << time << " " << measur1list[time] << " " << measur2list[time] << endl;
    }

    outfile.close();
    return 0;
}
