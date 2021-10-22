import numpy as np
from tcfunctionsx import *



def main_fun():
	data_file = 'crtedq.dat'
	er_qubits, m_stablz = read_file()
	er_stablz = torus_error(m_stablz, er_qubits)
	matching = mwpm_toric(er_stablz)
	crted_qubits = correct_qubits(matching,er_qubits)
	int_qubits = convert_to_number(crted_qubits)
	return int_qubits
	# with open(data_file, 'w') as f:
	# 	f.write("%d"%int(int_qubits))
	# f.close()

s = main()	
print (s)
# write crted_qubits








