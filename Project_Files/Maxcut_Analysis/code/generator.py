 
# -------------------------------------------------------------------
# Andrew Morato
# CS 631 Term Project
# May 5, 2020
# Random Weighted Graph Generator
# -------------------------------------------------------------------

'''

        Code to randomly generate an undirected, weighted graph.
    Works by building several bipartite graphs and linking them
    together, adding a source on the left and a sink on right to form
    a graph, inlcuding randomally weighted edges. The caller
    specifies the amount of bipartite graphs linked together and the
    desired average degree of each node, including the range of
    weights.

'''

# Imports ---

import numpy as np
import random as r


# Global Variables ---

UNIQUE_NODE_ID = 1


# Objects ---

# Represents a node or vertex in a graph
#
# ID      Integer unique identifier
# edges   List of Edge objects connecting to this Node
# group   Used for graph partitioning. Boolean value used to denote
#         this Node's membership to one group or the other
class Node:
	def __init__(self, ID):
		self.ID = ID
		self.edges = []
		self.group = None

# Represents an edge connecting two Nodes in a graph
#
# n       The Node object on one endpoint of this Edge
# m       The Node object on the other endpoint of this Edge
# weight  Weight or cost of this Edge. Integer value
class Edge:
	def __init__(self, n, m, weight):
		self.n = n
		self.m = m
		self.weight = weight
		self.n.edges.append(self)
		self.m.edges.append(self)


# ----------------------- Dataset Generation ------------------------


# Generates 4 datasets of graphs with different characteristics
#
# Dataset 1 in direcotry dir_1: Vertices have a high degree
# Dataset 2 in direcotry dir_2: Vertices have a low degree
# Dataset 3 in direcotry dir_3: High count of vertices (> 1000)
# Dataset 4 in direcotry dir_4: Low count of vertices (< 1000)
#
# Each dataset has ~1000 graphs
#
# returns  None
def generate_datasets():
	pass


# ------------------------ Graph Generation -------------------------

# Constructs several bipartite graphs and links them together,
# where each grouping of nodes has edges to next grouping of nodes,
# beginning at a single source and ending at a single sink
#
# groups     List of ints where each entry represents the size of the
#            corresponding group. The list len is the number of groups
# degree     Average degree per node (min 2)
# w          Upper range limit of edge weights
#
# returns    A graph of the form (([v1], [v2], [v3], etc), [e]) where
#            [v]'s are groups of nodes and [e] is the set of all edges
def create_graph(groups, degree, w):
	global UNIQUE_NODE_ID

	# create initial bipartite graph
	g1, g2 = groups[0], groups[1]
	G = create_bipartite_graph(g1, g2, degree, w)

	# iterativly add groups and edges to the tail of the graph
	remaning_groups = len(groups) - 2
	for i in range(remaning_groups):
		addition = add_to_graph(G[0][-1], groups[i+2], degree, w)
		G[0].append(addition[0])
		G[1] += addition[1]

	# create the source and sink nodes, reset global counter
	s, t = Node(0), Node(UNIQUE_NODE_ID)
	UNIQUE_NODE_ID = 1

	# add edges from s and t to the first and last groups in graph
	edges = [Edge(s, n, r.randrange(w)+1) for n in G[0][0]]
	edges += [Edge(n, t, r.randrange(w)+1) for n in G[0][-1]]
	G[1] += edges

	# add source and sink to the graph
	G[0].insert(0, [s])
	G[0].append([t])

	return G


# Constructs a bipartite graph according to the specifications
#
# n_size   Number of nodes on one side of the graph
# m_size   Number of nodes on the other side of the graph
# degree   Average degree per node (min 1)
# w        Upper range limit of edge weights
#
# returns  A bipartite graph of the form (([n], [m]), e) where
#          [[n], [m]] are the nodes and e is the collection of edges
def create_bipartite_graph(n_size, m_size, degree, w):
	global UNIQUE_NODE_ID

	# generate n_size nodes
	n = np.arange(UNIQUE_NODE_ID, UNIQUE_NODE_ID + n_size)
	n = [Node(ID) for ID in n]
	UNIQUE_NODE_ID += n_size

	m, e = add_to_graph(n, m_size, degree, w)

	return [[n, m], e]


# Increases the graph by creating a new column of nodes and adding
# edges to the given tail column of nodes 'n', according to the 
# specifications
#
# n        List of node objects on one side of the graph
# m_size   Number of nodes on the other side of the graph
# degree   Average degree per node (min 1)
# w        Upper range limit of edge weights
#
# returns  The newly created m node objects and the edge objects
#          between n and m in the from ([m], [e])
def add_to_graph(n, m_size, degree, w):

	# edge     An Edge object
	# edges    A collection of Edge objects
	# returns  True if edge is not in edges
	def not_in(edge, edges):
		for e in edges:
			if edge.n == e.n and edge.m == e.m:
				return False
		return True

	global UNIQUE_NODE_ID

	# generate m_size nodes
	m = np.arange(UNIQUE_NODE_ID, UNIQUE_NODE_ID + m_size)
	m = [Node(ID) for ID in m]
	UNIQUE_NODE_ID += m_size

	# generate an edge from each node in n to a random node in m
	e = [Edge(node, r.choice(m), r.randrange(w)+1) for node in n]

	# add an edge from nodes in m that don't have any edges
	for node in m:
		if len(node.edges) == 0:
			e.append(Edge(node, r.choice(n), r.randrange(w)+1))

	# exit the function if the desired average degree per node is met
	# else, add edges from random nodes in n to random nodes in m
	# until the expected number of edges is met
	exp_num_of_edges = int(np.ceil((degree * (len(n) + m_size)) / 2))
	if len(e) >= exp_num_of_edges:
		return (m, e)

	# build list of possible edges to add to the graph, avoiding
	# duplicating edges that already exist
	all_edges = []
	for v1 in n:
		all_edges += [(v1, v2) for v2 in m]
	all_edges = [edge for edge in all_edges if not present(edge, e)]

	# add edges to the graph by randomlly picking from this list
	while len(e) < exp_num_of_edges and len(all_edges) > 0:
		ind = r.randrange(len(all_edges))
		v1, v2 = all_edges[ind]
		e.append(Edge(v1, v2, r.randrange(w)+1))
		del all_edges[ind]

	return (m, e)


# Checks if a tuple of Nodes are connected by an Edge from the
# given list of edges
#
# pair     The tuple of Nodes to check
# edges    A list of Edge objects to check the pair of nodes against
#
# returns  True if the nodes in pair are connected by a given Edge
def present(pair, edges):
	for edge in edges:
		if edge.n == pair[0] and edge.m == pair[1]:
			return True
		elif edge.n == pair[1] and edge.m == pair[0]:
			return True
	return False

