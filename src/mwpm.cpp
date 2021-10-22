#include "Python/Python.h"
#include <fstream>
#include <iostream>
#include <string>
#include <bitset>
using namespace std;

void mwpm_decoding(int dx, int dy, cx_dvec &psi)
{
    int d = psi.size();
    int j int const len = 2 * dx * dy;

    for (int i = 1; i < d; i++)
    {
        if (psi[i] != 0)
        {
            Savequbits(dx, dy, len, psi[i]);

            Py_Initialize();
            PyRun_SimpleString("import sys");
            PyRun_SimpleString("execfile('./tcmain.py')");
            Py_Finalize();

            add python;
            n_psi = readqubits();
            psi[n_psi] = psi[n_psi] + psi[i];
            psi[i] = 0;
        }
    }
}

int mwpm_python(int dx, int dy, int n_psi)
{
    int res;
    PyObject *pModule, *pFunc;
    PyObject *pArgs, *pValue;

    /* import */
    pModule = PyImport_Import(PyString_FromString("tcmain"));

    /* great_module.great_function */
    pFunc = PyObject_GetAttrString(pModule, "main_fun");

    /* build args */
    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyInt_FromLong(a));

    /* call */
    pValue = PyObject_CallObject(pFunc, pArgs);

    res = PyInt_AsLong(pValue);
    return res;
}

void Savequbits(int dx, int dy, int const len, int qubits)
{
    ofstream outfile;
    outfile.open("qubits.dat");

    bitset<len> bitstr(qubits);

    outfile << dx << endl;
    outfile << dy << endl;
    outfile << bitstr.to_string() << endl;
    outfile.close();
}

void readqubits()
{
    string line;
    ifstream myfile("py_decoder/crtedq.dat");
    if (myfile.is_open())
    {
        getline(myfile, line);
        myfile.close();
    }
    else
        cout << "Unable to open file";
    int n_psi = stoi(line);
    return n_psi;
}