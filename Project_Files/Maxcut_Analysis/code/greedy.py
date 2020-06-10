
# -------------------------------------------------------------------
# Andrew Morato
# CS 631 Term Project
# May 5, 2020
# Max-Cut: Greedy Implementation
# -------------------------------------------------------------------

'''
 -------- A Maximum-Cut Approximation via Greedy Alogirthms ---------

  Greedy Algorithms

        Greedy algoithms refer to any algorithm that blindly selects
    the best option presented to it at any given moment. There are
    numerous variations to this, but the naïve greedy approach is the
    one that picks the best choice now.

  Claire Mathieu's and Warren Schudy's Greedy Approximation

	    Given an undirected graph G = (V, E) with a positive integer
	weight on each edge, the goal is to find a partition (A, B) of
	the vertex set such that the weight of all edges with one end in
	A and the other in B is maximized. This problem is NP-hard. To
	solve this problem with a greedy approach, Mathie and Warren
	propose the following solution in "Yet Another Algorithm for
	Dense Max Cut: Go Greedy".

	    This greedy approach considers vertices one by one in random
	fashion and places them in one partition or the other depending
	on the edge weights to neighbors that are already in each of the
	partitions. Each vertex in placed in the partition that maximizes
	the value of the cut. For example, say vertex v has neighbors x
	and y in partition A and has neighbors u and w in parition B,
	with an edge weight of 5 to x, 6 to y, 3 to u, and 7 to w. Thus,
	when placing vertex v in a partition, we see that it has a total
	weight of 11 to parition A and 10 to parition B. According to
	this algoirthm, vertex v would be placed in partition B to take
	advantage of v's greater edge weight to partition A, maximizing
	the value of the cut from A to B. After all vertices are
	randomaly placed in one partition or the other, we have our max
	cut approximation.

	    One variation invovles repeating this process multiple times
	and selecting the best resulting cut. If repeated enough times,
	namely 2^2^poly(n) times, we could guarantee near-optimality,
	however this would be quite slow and therefore useless as an
	efficient approximation.
'''

# Imports ---

import generator
import visualizer as viz
import localsearch
import random
import cv2


# ------------------------ Greedy Algorithm -------------------------


# Runs a greedy algorithm to approximate the maximum the cost of the
# cut on a graph.
#
# n              The number of times to repeat the approximation
# graph          The graph on which to run the max cut approximation.
#                If not supplied, a random graph is generated. The
#                graph is a triplet with: (nodes, edges, groups)
# display_graph  Visually displays the graph with opencv if True
# term           Outputs excess information to terminal
#
# returns        None
def approx_maxcut(n=1, graph=None, display_graph=False, term=False):

	if term:
		print("\n*** maxcut approx via greedy approach ***\n")

	nodes, edges, groupings = graph

	best_cost, best_attempt = 0, 0
	for i in range(n):

		if n > 1 and term:
			print(" * attempt #" + str(i+1))

		# Shuffles the list of Nodes in place to generate a random order
		random.shuffle(nodes)

		# Clears the Nodes from any grouping
		for node in nodes:
			node.group = None

		# Display initial graph
		if display_graph == True:
			msg = "initial random graph"
			viz.display_graph(groupings, edges, msg, 0) 

		# Executes the greedy algorithm for each node
		for node in nodes:
			cost_a, cost_b = place_in_group(node)
			cost = localsearch.cost_of_cut(edges)

			# Output change to terminal
			g = "A" if node.group == True else "B"
			msg  = " * assigned vertex " + str(node.ID) + " to group"
			msg += " " + g + " for a total cost of " + str(cost)
			msg += " (A - "+ str(cost_a) +", B - " + str(cost_b) +")"
			if term:
				print(msg)

			# Display graph
			if display_graph == True:
				msg = "vertex " + str(node.ID) + " to " + str(g)
				viz.display_graph(groupings, edges, msg, cost)

		if cost > best_cost:
			best_cost, best_attempt = cost, i+1

		if term:
			output = " * final cost of attempt #" + str(i+1)
			print(output + ": " + str(cost) + "\n")

	if n > 1 and term:
		output = " * best cost: " + str(best_cost)
		print(output + ", from attempt #" + str(best_attempt) + "\n")

	cv2.destroyAllWindows()

	return best_cost


# Uses generator.py's create_graph function to build a graph,
# comprised of Nodes, Edges, a sink, and a source, where all Edges
# are weighed and all Nodes are grouped in one of two continuous
# partitions. See generator.py#create_graph's documentation to
# give custom arguments
#
# groups   List of ints where each entry represents the size of the
#          corresponding group. The list size is the number of groups
# degree   Average degree per node (min 2)
# weight   Upper range limit of edge weights
# term     Outputs excess information to the terminal
#
# returns  A triplet containing the list of all Nodes, list of all
#          Edges, and the groups of Nodes in the graph
def generate_graph(groups, degree=2, weight=15, term=False):
	if term:
		print(" * generating graph ...")
	groups, e = generator.create_graph(groups, degree, weight)
	nodes = [node for nodes in groups for node in nodes]

	if term:
		print(" * generated a weighted, partitioned graph with:\n")
		print("       nodes: " + str(len(nodes)))
		print("       edges: " + str(len(e)))

	return (nodes, e, groups)


# Places the given Node to one side of the partition or the other,
# based on maximizing the edge weights to neighbors that are already
# in each of the partitions. Randomly assigned the Node to A or B if
# the combined weights to A and B are equal. Assigns the Node to a
# partition by changing the group field of the Node object.
#
# node     The Node under consideration
#
# returns  A tuple with the cost of the given Node to its neighbors
#          in A and its neighbors in B (cost_to_A, cost_to_B)
def place_in_group(node):
	cost_to_A, cost_to_B = 0, 0

	# Calculates the cost to the given Node's neighbors in A or B
	for edge in node.edges:
		neighbor = edge.n if edge.m.ID == node.ID else edge.m
		if neighbor.group == True:
			cost_to_A += edge.weight
		elif neighbor.group == False:
			cost_to_B += edge.weight

	# Assign the Node to a group
	if cost_to_A > cost_to_B:
		node.group = False
	elif cost_to_A < cost_to_B:
		node.group = True
	else:
		node.group = random.choice([True, False])

	return (cost_to_A, cost_to_B)

