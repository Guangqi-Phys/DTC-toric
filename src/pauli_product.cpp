#include "../include/qsim.hpp"
#include "../include/pauli_product.hpp"
#include "../libs/pcg-cpp/include/pcg_random.hpp"
#include <cmath>
#include <stdlib.h>
#include <random>

using namespace std;

// Applies a Pauli operator i^{xz} X^xZ^z to the k'th qubit to the state psi.
// This algorithm takes at most 2^n complex number multiplications.
void apply_pauli_fast(int k, bool x, bool z, cx_dvec &psi)
{
	int d;
	d = psi.size();
	if (pow(2, k) >= d)
	{
		throw "Qubit index out of range.";
	}
	if (z)
	{
		if (x)
		{
			for (int i = 0; i < d; i++)
			{
				if ((i & (1 << k)) >> k == 0)
				{
					cx_double temp1, temp2;
					temp1 = psi[i] * cx_double(0.0, 1.0);
					temp2 = psi[(1 << k) | i] * cx_double(0.0, -1.0);
					psi[i] = temp2;
					psi[(1 << k) | i] = temp1;
				}
			}
		}
		else
		{
			for (int i = 0; i < d; i++)
			{
				if ((i & (1 << k)) >> k == 1)
				{
					psi[i] *= -1.0;
				}
			}
		}
	}
	else
	{
		if (x)
		{
			for (int i = 0; i < d; i++)
			{
				if ((i & (1 << k)) >> k == 0)
				{
					cx_double temp1, temp2;
					temp1 = psi[i];
					temp2 = psi[(1 << k) | i];
					psi[i] = temp2;
					psi[(1 << k) | i] = temp1;
				}
			}
		}
	}
}

// Apply exp(iPθ) to psi, where P = i^{x·z}(⨂_k X_k^(x_k)) (⨂_k Z_k^(z_k))
void apply_ppr(unsigned int x, unsigned int z, double theta, cx_dvec &psi)
{
	long int d;
	d = psi.size();
	cx_double c, s;
	c = cos(theta);
	s = sin(theta);
	//  cx_double csc;
	//  csc = 1.0 / cos(theta);
	bool *not_changed = new bool[(1 << 25)];
	fill_n(not_changed, (1 << 25), true);

	for (int y = 0; y < d; y++)
	{
		if (not_changed[y])
		{

			unsigned int xz = __builtin_popcount(x & z);
			unsigned int yz = __builtin_popcount(y & z);

			cx_double phase1, phase2;

			switch ((1 + xz + 2 * (yz + xz)) % 4) // i ^ { 1 + x·z + (i) ^ { 2 * __builtin_popcount[(y ^ x) & z] } }
			{									  // y = 101010 x = 111000 z = 011000 y ^ x = 010110 (y^x)&z = 1 yz+xz = 1+2 = 3 mod 2
			case 0:
				phase1 = cx_double(1.0, 0.0);
				break;
			case 1:
				phase1 = cx_double(0.0, 1.0);
				break;
			case 2:
				phase1 = cx_double(-1.0, 0.0);
				break;
			case 3:
				phase1 = cx_double(0.0, -1.0);
				break;
			}

			switch ((1 + xz + 2 * yz) % 4) // i^{1+x·z+i^{2·count1[(y&z]}}
			{
			case 0:
				phase2 = cx_double(1.0, 0.0);
				break;
			case 1:
				phase2 = cx_double(0.0, 1.0);
				break;
			case 2:
				phase2 = cx_double(-1.0, 0.0);
				break;
			case 3:
				phase2 = cx_double(0.0, -1.0);
				break;
			}
			cx_double temp1, temp2;
			temp1 = psi[y] * c + psi[y ^ x] * s * phase1; // X ^ { x_k } Z ^ { z_k } psi[y ^ x] \to(-1) ^ { (__builtin_popcount[(y ^ x) & z]) } psi[y]
			temp2 = psi[y ^ x] * c + psi[y] * s * phase2;
			psi[y] = temp1;
			psi[y ^ x] = temp2;

			// psi[y] *= c; psi[y] += s * phase1 * psi[y^x];
			// psi[y^x] += s * phase2 * psi[y];
			// psi[y^x] *= csc;

			not_changed[y] = false;
			not_changed[y ^ x] = false;
		}
	}
	delete[] not_changed;
}

// Measure Pauli Product operator (fast)
double measure_pp(unsigned int x, unsigned int z, cx_dvec &psi)
{
	int d;
	d = psi.size();
	unsigned int bits, n_temp = (d < 0) ? -d : d;
	for (bits = 0; n_temp != 0; ++bits)
		n_temp >>= 1;
	unsigned int n_q = bits - 1;

	cx_double mysum = 0.0;
	for (int y = 0; y < d; y++)
	{
		unsigned int xz = __builtin_popcount(x & z);
		unsigned int yz = __builtin_popcount(y & z);
		cx_double phase;
		switch ((xz + 2 * yz) % 4)
		{
		case 0:
			phase = cx_double(1.0, 0.0);
			break;
		case 1:
			phase = cx_double(0.0, 1.0);
			break;
		case 2:
			phase = cx_double(-1.0, 0.0);
			break;
		case 3:
			phase = cx_double(0.0, -1.0);
			break;
		}
		mysum += psi[y] * conj(psi[y ^ x]) * phase;
	}
	return real(mysum);
}

double measure_pp_corr(unsigned int x, unsigned int z, cx_dvec &psi, cx_dvec &corr)
{
	int d;
	d = psi.size();
	unsigned int bits, n_temp = (d < 0) ? -d : d;
	for (bits = 0; n_temp != 0; ++bits)
		n_temp >>= 1;
	unsigned int n_q = bits - 1;

	cx_double mysum = 0.0;
	for (int y = 0; y < d; y++)
	{
		unsigned int xz = __builtin_popcount(x & z);
		unsigned int yz = __builtin_popcount(y & z);
		cx_double phase;
		switch ((xz + 2 * yz) % 4)
		{
		case 0:
			phase = cx_double(1.0, 0.0);
			break;
		case 1:
			phase = cx_double(0.0, 1.0);
			break;
		case 2:
			phase = cx_double(-1.0, 0.0);
			break;
		case 3:
			phase = cx_double(0.0, -1.0);
			break;
		}
		mysum += psi[y] * conj(psi[y ^ x]) * phase * corr[y];
	}
	return real(mysum);
}

// void apply_stablz(int dx, int dy, double theta0, cx_dvec &psi)
// {
// 	unsigned int sx;
// 	unsigned int sz;
// 	for (int i = 0; i < 2 * dy; i = i + 2)
// 	{
// 		for (int j = 0; j < dx; j++)
// 		{
// 			if (i == 2 * dy - 2)
// 			{
// 				if (j == dx - 1)
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx)) | (1 << (j + 1)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((i - 1) * dx)) | (1 << (i * dx)) | (1 << ((i + 1) * dx)));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 				else
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx + (j + 1))) | (1 << (j + 1)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((i - 1) * dx + (j + 1))) | (1 << (i * dx + (j + 1))) | (1 << ((i + 1) * dx + (j + 1))));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 			}
// 			else if (i == 0)
// 			{
// 				if (j == dx - 1)
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx)) | (1 << ((i + 2) * dx + j)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((2 * dy - 1) * dx)) | (1 << (i * dx)) | (1 << ((i + 1) * dx)));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 				else
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx + (j + 1))) | (1 << ((i + 2) * dx + j)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((2 * dy - 1) * dx + (j + 1))) | (1 << (i * dx + (j + 1))) | (1 << ((i + 1) * dx + (j + 1))));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 			}
// 			else
// 			{
// 				if (j == dx - 1)
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx)) | (1 << ((i + 2) * dx + j)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((i - 1) * dx)) | (1 << (i * dx)) | (1 << ((i + 1) * dx)));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 				else
// 				{
// 					sz = ((1 << (i * dx + j)) | (1 << ((i + 1) * dx + j)) | (1 << ((i + 1) * dx + (j + 1))) | (1 << ((i + 2) * dx + j)));
// 					sx = ((1 << (i * dx + j)) | (1 << ((i - 1) * dx + (j + 1))) | (1 << (i * dx + (j + 1))) | (1 << ((i + 1) * dx + (j + 1))));
// 					apply_ppr(sx, sz, theta0, psi);
// 				}
// 			}
// 		}
// 	}
// }

void apply_stabl_normal(int dx, int dy, cx_dvec &psi)
{
	std::default_random_engine gen;
	std::normal_distribution<double> theta0(0.3 * M_PI, 0.021 * M_PI);
	unsigned int sx;
	unsigned int sz;
	for (int i = 0; i < 2 * dy; i = i + 2)
	{
		for (int j = 0; j < dx; j++)
		{
			sz = (1 << (i * dx + j)) | (1 << (((i + 1) % (2 * dy)) * dx + j)) | (1 << (((i + 1) % (2 * dy)) * dx + ((j + 1) % dx))) | (1 << (((i + 2) % (2 * dy)) * dx + j));
			sx = (1 << (i * dx + j)) | (1 << (((i - 1 + 2 * dy) % (2 * dy)) * dx + ((j + 1) % dx))) | (1 << (i * dx + ((j + 1) % dx))) | (1 << (((i + 1) % (2 * dy)) * dx + ((j + 1) % dx)));
			apply_ppr(sx, sz, theta0(gen), psi);
		}
	}
}

void apply_stabl_uniform(int dx, int dy, cx_dvec &psi)
{
	unsigned int sx;
	unsigned int sz;
	double theta0_uni;
	// srand((unsigned)time(NULL));
	pcg_extras::seed_seq_from<random_device> seed_source;

	pcg32 engine3(seed_source);

	uniform_real_distribution<double> dist3(-0.5, 0.5);

	for (int i = 0; i < 2 * dy; i = i + 2)
	{
		for (int j = 0; j < dx; j++)
		{
			// random_value = (dist3(engine3) * 200 - 100) / 1000.0;
			theta0_uni = 0.25 * M_PI + dist3(engine3);
			sz = (1 << (i * dx + j)) | (1 << (((i + 1) % (2 * dy)) * dx + j)) | (1 << (((i + 1) % (2 * dy)) * dx + ((j + 1) % dx))) | (1 << (((i + 2) % (2 * dy)) * dx + j));
			sx = (1 << (i * dx + j)) | (1 << (((i - 1 + 2 * dy) % (2 * dy)) * dx + ((j + 1) % dx))) | (1 << (i * dx + ((j + 1) % dx))) | (1 << (((i + 1) % (2 * dy)) * dx + ((j + 1) % dx)));
			apply_ppr(sx, sz, theta0_uni, psi);
		}
	}
}

void initial_add_phase(int dx, int dy, cx_dvec &psi)
{
	unsigned int lx_i;
	// unsigned int sz;
	double theta_i;
	// srand((unsigned)time(NULL));
	pcg_extras::seed_seq_from<random_device> seed_source_i;

	pcg32 engine_i(seed_source_i);

	uniform_real_distribution<double> dist_i(-0.5, 0.5);

	for (int i = 0; i < 2 * dy; i++)
	{
		for (int j = 0; j < dx; j++)
		{
			// random_value = (dist3(engine3) * 200 - 100) / 1000.0;
			theta_i = 0.25 * M_PI;
			lx_i = (1 << (i * dx + j));
			apply_ppr(lx_i, 0, theta_i, psi);
		}
	}
}

// measure (I+X^xZ^z)/2 (probability of +1 eigenstate of X^xZ^z)
double measure_prob_positive(unsigned int x, unsigned int z, cx_dvec &psi)
{
	int d;
	d = psi.size();
	unsigned int bits, n_temp = (d < 0) ? -d : d;
	for (bits = 0; n_temp != 0; ++bits)
		n_temp >>= 1;
	unsigned int n_q = bits - 1;

	cx_double mysum = 0.0;
	for (int y = 0; y < d; y++)
	{
		unsigned int xz = __builtin_popcount(x & z);
		unsigned int yz = __builtin_popcount(y & z);
		cx_double phase;
		switch ((xz + 2 * yz) % 4)
		{
		case 0:
			phase = cx_double(1.0, 0.0);
			break;
		case 1:
			phase = cx_double(0.0, 1.0);
			break;
		case 2:
			phase = cx_double(-1.0, 0.0);
			break;
		case 3:
			phase = cx_double(0.0, -1.0);
			break;
		}
		mysum += 0.5 * psi[y] * conj(psi[y ^ x]) * phase + 0.5;
	}
	return real(mysum);
}
