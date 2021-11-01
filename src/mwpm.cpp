#include "Python/Python.h"
#include "../include/qsim.hpp"
#include "../include/floquet.hpp"
#include "../include/pauli_product.hpp"
#include "../include/toric.hpp"
#include "../include/stabilizer.hpp"
#include "../include/match.hpp"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void mwpm_decoding(int dx, int dy, cx_dvec &psi)
{
    int i, n_psi;
    int d = psi.size();
    int n_q = dx * dy * 2;

    for (i = 0; i < d; i++)
    {
        if ((psi[i] != 0.0) && (__builtin_popcount(i) < 0.1 * n_q))
        {
            n_psi = mwpm_python(dx, dy, i);

            // cout << i << " " << n_psi << endl;
            if (n_psi != i)
            {
                psi[n_psi] = sqrt(psi[n_psi] * conj(psi[n_psi]) + psi[i] * conj(psi[i]));
                psi[i] = 0.0;
            }
        }
    }
}

int mwpm_python(int dx, int dy, int n_psi)
{
    int res;
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *sys, *path;

    // if (Stop == 1)
    // {
    //     Py_Initialize();
    // }

    sys = PyImport_ImportModule("sys");
    path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("py_decoder/"));

    /* import */
    pName = PyUnicode_FromString("tcmain");
    pModule = PyImport_Import(pName);

    if (!pModule)
    {
        PyErr_Print();
        printf("ERROR in pModule\n");
        exit(1);
    }

    /* call python function */
    pFunc = PyObject_GetAttrString(pModule, "main_fun");

    /* build args */
    pArgs = PyTuple_New(3);
    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(dx));
    PyTuple_SetItem(pArgs, 1, PyLong_FromLong(dy));
    PyTuple_SetItem(pArgs, 2, PyLong_FromLong(n_psi));

    /* call */
    pValue = PyObject_CallObject(pFunc, pArgs);

    res = PyLong_AsLong(pValue);

    // if (Stop == 3)
    // {
    //     Py_Finalize();
    // }

    return res;
}

// void Savequbits(int dx, int dy, int const len, int qubits)
// {
//     ofstream outfile;
//     outfile.open("qubits.dat");

//     bitset<len> bitstr(qubits);

//     outfile << dx << endl;
//     outfile << dy << endl;
//     outfile << bitstr.to_string() << endl;
//     outfile.close();
// }

// void readqubits()
// {
//     string line;
//     ifstream myfile("py_decoder/crtedq.dat");
//     if (myfile.is_open())
//     {
//         getline(myfile, line);
//         myfile.close();
//     }
//     else
//         cout << "Unable to open file";
//     int n_psi = stoi(line);
//     return n_psi;
// }