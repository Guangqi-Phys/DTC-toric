import random
import networkx as nx 

def initial_states(lx,ly,n_psi):
	len = lx*ly*2
	qubits_str = (bin(n_psi).replace('0b','')).zfill(len)
	qubits_str = qubits_str[::-1]
	er_qubits = [[0 for col in range(lx)] for row in range(2*ly)]
	m_stablz = [[0 for col in range(lx)] for row in range(ly)]
	for i_row in range(2*ly):
		for i_col in range(lx):
			er_qubits[i_row][i_col] = int(qubits_str[i_row*lx+i_col])
	return er_qubits, m_stablz

# def read_file():
# 	with open('data.dat', 'r') as fp:
# 		v = fp.readlines()
# 		lx = int(v[0])
# 		ly = int(v[1])
# 		qubits = v[2]
# 	er_qubits = [[0 for col in range(lx)] for row in range(2*ly)]
# 	m_stablz = [[0 for col in range(lx)] for row in range(ly)]
# 	for i_row in range(2*ly):
# 		for i_col in range(lx):
# 			er_qubits[i_row][i_col] = int(qubits[i_row*lx+i_col])
# 	return er_qubits, m_stablz



def torus_error(m_stablz, er_qubits):

	ly = len(m_stablz)
	lx = len(m_stablz[0])

	for i_row in range(2*ly):
	    for i_col in range(lx):
	        if er_qubits[i_row][i_col] == 1:
	            if i_row % 2 == 0:
	                stab_row = int(i_row/2)
	                if stab_row == 0:
	                    m_stablz[stab_row][i_col] += 1
	                    m_stablz[ly-1][i_col] += 1
	                else:
	                    m_stablz[stab_row][i_col] += 1
	                    m_stablz[stab_row - 1][i_col] += 1
	                        
	            else:
	                stab_row = int((i_row - 1)/2)
	                if i_col == 0:
	                    m_stablz[stab_row][i_col] += 1
	                    m_stablz[stab_row][lx-1] += 1
	                else:
	                    m_stablz[stab_row][i_col - 1] += 1
	                    m_stablz[stab_row][i_col] += 1
	for i_row in range(ly):
		for i_col in range(lx):
			if m_stablz[i_row][i_col] % 2 == 0:
				m_stablz[i_row][i_col] = 0
			else:
				m_stablz[i_row][i_col] = 1
	error_stablz = m_stablz
	return error_stablz



def mwpm_toric(er_stablz):
	"""
	Find the minimum weighting perfect matching of disturbed stabilizer matrix with networksx package.
	Input:
	  Disturbed stabilizer matrix.
	Output:
	  Minimum weighting maching list.
	Notice that this package is for maximum weighting perfect matching, so we need to add a negative sign on weight 
	to get a minimum weighting perfect matching
	"""
	g = nx.Graph()
	ly = len(er_stablz)
	lx = len(er_stablz[0])
	for i_row in range(ly):
		for i_col in range(lx):
			if er_stablz[i_row][i_col] == 1:
				g.add_nodes_from([(i_row,i_col)])
	stb_list = []
	graph = nx.Graph()
	for i_row in range(ly):
		for i_col in range(lx):
			if er_stablz[i_row][i_col] == 1:
				stb_list.append((i_row,i_col))
	edges_list = []
	for i in range(len(stb_list)):
	    for j in range(len(stb_list)):
	        if i < j:
	            if stb_list[i][0] > stb_list[j][0]:
	                weight_1 = min(abs(stb_list[i][0] - stb_list[j][0]),abs(ly-stb_list[i][0])+abs(stb_list[j][0]))
	            else:
	                weight_1 = min(abs(stb_list[i][0] - stb_list[j][0]),abs(ly-stb_list[j][0])+abs(stb_list[i][0]))
	            if stb_list[i][1] > stb_list[j][1]:
	                weight_2 = min(abs(stb_list[i][1] - stb_list[j][1]),abs(lx-stb_list[i][1])+abs(stb_list[j][1]))
	            else:
	                weight_2 = min(abs(stb_list[i][1] - stb_list[j][1]),abs(lx-stb_list[j][1])+abs(stb_list[i][1])) 
	            weight = weight_1+weight_2 # Weight1 for row while weight2 for column
	            edges_list.append((stb_list[i],stb_list[j],-weight))
	graph.add_weighted_edges_from(edges_list)
	matching = nx.algorithms.matching.max_weight_matching(graph, maxcardinality=True)
	return matching




def correct_qubits(matching,er_qubits):
	"""
	Using the matching list obtained from the MWPM step to correct qubits matrix
	Input:
	  matching list
	  qubits matrix with errors
	Output:
	  corrected qubits matrix
	"""
	ly = int(len(er_qubits)/2)
	lx = len(er_qubits[0])
	matching_list = list(matching)
	crted_qubits = er_qubits
	q_row = 0
	q_col = 0
	for i in range(len(matching_list)):
	    aux_a = matching_list[i][0][0]
	    aux_b = matching_list[i][0][1]
	    aux_c = matching_list[i][1][0]
	    aux_d = matching_list[i][1][1]
	    q_row = 2*min(aux_a,aux_c)
	    if abs(aux_b-aux_d) <= int(lx/2):
	        for j in range(min(aux_b,aux_d)+1,max(aux_b,aux_d)+1):
	            crted_qubits[q_row+1][j] += 1
	    else:
	        for j in range(0, min(aux_b,aux_d)+1):
	            crted_qubits[q_row+1][j] += 1
	        for j in range(max(aux_b,aux_d)+1, lx):
	            crted_qubits[q_row+1][j] += 1
	    if aux_a >= aux_c:
	        q_col = aux_b
	    else:
	        q_col = aux_d
	    if abs(aux_a-aux_c) <= int(ly/2):
	        for j in range(2*min(aux_a,aux_c)+2,2*max(aux_a,aux_c)+2,2):
	            crted_qubits[j][q_col] += 1
	    else:
	        for j in range(0, 2*min(aux_a,aux_c)+2,2):
	            crted_qubits[j][q_col] += 1
	        for j in range(2*max(aux_a,aux_c)+2, 2*ly,2):
	            crted_qubits[j][q_col] += 1
	for i_row in range(2*ly):
	    for i_col in range(lx):
	        if crted_qubits[i_row][i_col] % 2 == 0:
	            crted_qubits[i_row][i_col] = 0
	        else:
	            crted_qubits[i_row][i_col] = 1
	return crted_qubits



def convert_to_number(crted_qubits):
	ly = int(len(crted_qubits)/2)
	lx = len(crted_qubits[0])
	str_list = []
	for i_row in range(2*ly):
		for i_col in range(lx):
			str_list.append(str(crted_qubits[i_row][i_col]))
	str_bit = ''.join(str_list)
	str_bit = str_bit[::-1]
	int_qubits = int(str_bit,2)
	return int_qubits





















