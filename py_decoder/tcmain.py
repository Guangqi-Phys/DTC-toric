import numpy as np
from tcfunctionsx import *





def main_fun(lx,ly,npsi):
	# data_file = 'crtedq.dat'
	er_qubits, m_stablz = initial_states(lx,ly,npsi)
	er_stablz = torus_error(m_stablz, er_qubits)
	matching = mwpm_toric(er_stablz)
	crted_qubits = correct_qubits(matching,er_qubits)
	int_crted_qubits = convert_to_number(crted_qubits)
	return int_crted_qubits
	# with open(data_file, 'w') as f:
	# 	f.write("%d"%int(int_qubits))
	# f.close()

# s = main_fun(3,3,1152)
# print (s)
# print ((bin(s).replace('0b','')).zfill(18))
# write crted_qubits








