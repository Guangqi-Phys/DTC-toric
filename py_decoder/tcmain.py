import matplotlib.pyplot as plt
import numpy as np
from tcfunctionsx import *



l_simult = [5,7,9] # matrix size to simulate
n_simult = 5000  # number of simulations 

# generating the parobability list
d_prob = 0.02
prob_start = 0.02
prob_end = 0.20
probs = []
for i in range(int((prob_end-prob_start)/d_prob+1.01)):
	probs.append(prob_start+d_prob*i)


# simulating

data = []
data_file = 'data.dat'
for l in l_simult:
	for prob in probs:
		accu_rate = 0
		for ind_n in range(n_simult):
			m_stablz, m_qubits = torus_initial(l, l)
			er_stablz, er_qubits = torus_error(m_stablz, m_qubits, prob)
			matching = mwpm_toric(er_stablz)
			crted_qubits = correct_qubits(matching,er_qubits)
			check_bool = check_logical(crted_qubits)
			accu_rate +=check_bool
		accu_rate = 1- accu_rate/n_simult
		data.append([l,prob,accu_rate])
		
with open(data_file, 'w') as f:
	for ind in range(len(l_simult)*len(probs)):
		for jnd in range(3):
			f.write("%.4f  "%float(data[ind][jnd]))
		f.write('\n')
f.close()





# plotting
data_trans = list(map(list, zip(*data)))
data_plot = []
data_plot.append(probs)
for i  in range(0,len(probs)*len(l_simult),len(probs)):
	data_plot.append(data_trans[2][i:i+len(probs)])

# generating error bar
error_plot = []
errorbar_plot = []
for i in range(len(data_plot)):
    for j in range(len(data_plot[0])):
        error_plot.append(2*np.sqrt(n_simult*data_plot[i][j]*(1-data_plot[i][j]))/n_simult)
    errorbar_plot.append(error_plot)
    error_plot = []


for i in range(len(l_simult)):
	plt.errorbar(data_plot[0],data_plot[i+1],yerr = errorbar_plot[i+1],marker='o',capsize=4,capthick=1)



labels_list =[]
l_simult_str = [str(i) for i in l_simult]
for i in range(len(l_simult)):
	str1 = list(l_simult_str[i])
	str1.insert(0,'L = ')
	labels_list.append(''.join(str1))

plt.legend(labels= labels_list )
plt.axis([prob_start, prob_end, -0.1, 0.8])
plt.title(r'Toric Code - MWPM')
plt.xlabel(r'Qubit error rate')
plt.ylabel(r'Logical error rate')


plt.savefig('mwpm.pdf',dpi=600,format='pdf')
plt.show()









