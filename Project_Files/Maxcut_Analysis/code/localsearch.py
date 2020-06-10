
# -------------------------------------------------------------------
# Andrew Morato
# CS 631 Term Project
# May 5, 2020
# Max-Cut: Local Search Implementation
# -------------------------------------------------------------------

'''
 ----------- A Maximum-Cut Approximation via Local Search -----------

  Local Search

        Local search is an optimization technique that describes any
    algorithm that explores the space of possible solutions to a
    problem sequentially, moving from one solution to a "nearby" one.
    The idea is to move to better and better "neighboring" solutions
    until an optimal one is found. Thus, local search is comprised of
    two main components, a neighbor relation and a rule for choosing
    a neighboring solution at each step,

       1. Neighbor Relation - The neighborhood of nearby solutions is
          defined as the set of solutions S' obtained by making small
          modifications to the current solution S. The neighbor
          relation refers to the relation of all S' to S. We have the
          freedom to make up any neighbor relation we want.

       2. Choice of Neighboring Solution - In each step of a local
          search algorithm, it chooses a neighbor S' of S (within the
          neighborhood of S as defined by its neighbor relation) and
          iterates. An important part of the algorithm is in the
          choice of a neighboring solution S' of S.

        Moving from solution S to S', the hope is to improve the
    solution at each step i.e. ensure that the cost of S' is less
    than the cost of S. Like many optimization problems, local
    search algorithms can struggle with escaping local minema/maxima
    when all the neighboring solutions of S are more costly.

  The Maximum-Cut Problem & Klienberg's and Tardos' Approximation

	    Given an undirected graph G = (V, E) with a positive integer
	weight on each edge, the goal is to find a partition (A, B) of
	the vertex set such that the weight of all edges with one end in
	A and the other in B is maximized. This problem is NP-hard. To
	solve this problem with local search, Kleinberg and Tardos
	propose the following solution in "Algorithm Design",

	    In a partition of the vertex set (A, B), if there exists a
	node u such that the total weight of the edges from u to nodes in
	its own side of the partition exceeds the total weight of the
	edges from u to nodes in the other side of the partition, then u
	itself should be moved to the other side of the partition. This
	can be called a single-flip. Therefore, in this algorithm, the
	neighborhood of a solution S would be solutions that differ from
	S by just a single-flip of any one node.

	    It can be noted that any locally optimal solution for the
	Maximum Cut problem following the single-flip algorithm is at
	worst exactly half as "bad" as the globally optimal solution
	i.e. if the globally optimal solution yields weight w1 and the
	locally optimal solution yields weight w2, then w2 >= (1/2) w1
'''

# Imports ---

import generator
import visualizer as viz
import numpy as np
import cv2
import greedy


# --------------------- Local Search Algorithm ----------------------


# Runs the local search algorithm to approximate the maximum the cost
# of the cut on a generated graph.
#
# graph          The graph on which to run the max cut approximation.
#                If not supplied, a random graph is generated. The
#                graph is a triplet with: (nodes, edges, groups)
# display_graph  Visually displays the graph with opencv
# term       n   Outputs excess information to terminal
#
# returns        None
def approx_maxcut(graph=None, display_graph=False, term=False):

	if term:
		print("\n*** maxcut approx via localsearch ***\n")

	nodes, edges, groupings = graph

	partition_graph(groupings, nodes, term)
	cost = cost_of_cut(edges)
	if term:
		print("       cost of initial cut: " + str(cost) + "\n")

	# Display initial graph
	if display_graph == True:
		msg = "initial random graph"
		viz.display_graph(groupings, edges, msg, cost)

	improved_cost = True
	while improved_cost:
		flippable_nodes = find_flippable_nodes(nodes)
		flipped_node = flip_single_node(flippable_nodes)
		if flipped_node != None:
			new_cost = cost_of_cut(edges)

			# Output change to terminal
			msg  = " * flipped vertex " + str(flipped_node.ID)
			msg += " for a total cost of " + str(new_cost)
			msg += " (increase by " + str(new_cost-cost) + ")"
			if term:
				print(msg)

			# Display graph
			if display_graph == True:
				msg = "flipped vertex " + str(flipped_node.ID)
				viz.display_graph(groupings, edges, msg, new_cost)

			cost = new_cost
		else:
			improved_cost = False

			# Display final graph
			if display_graph == True:
				msg = "local search complete"
				viz.display_graph(groupings, edges, msg, cost)

	if term:
		print("\n * final cost " + str(cost) + "\n")
	cv2.destroyAllWindows()

	return cost


# Partitions the graph by assigning a color to each of the Nodes.
# Splits the graph in two based on the given Node groupings.
#
# groups   List of Nodes in groups
# nodes    List of Nodes in the graph
# term     Outputs partition information to the terminal
#
# returns  None
def partition_graph(groups, nodes, term):
	split = int(np.ceil(len(groups) / 2))
	for i in range(len(groups)):
		grouping = True if i < split else False
		for node in groups[i]:
			node.group = grouping

	# Outputs grouping to terminal
	if term:
		a = [node.ID for node in nodes if node.group == True]
		b = [node.ID for node in nodes if node.group == False]
		print("       partition:")
		print("           group A: " + str(a))
		print("           group B: " + str(b))


# Takes a list of all the Nodes in the graph and returns a list of
# flippable Nodes. A flippable Node is a Node that has at least one
# edge spanning both paritions (i.e. different groups) and is not the
# source or the sink.
#
# nodes    A list of all Nodes in the graph
#
# returns  A list of flippable Nodes from the graph
def find_flippable_nodes(nodes):
	flippable_nodes = []
	for node in nodes:
		if is_node_flippable(node, 0, len(nodes)-1):
			flippable_nodes.append(node)

	return flippable_nodes


# Takes a list of Nodes that can be flipped and determines which flip
# would increase the total cost of the cut (if any). Flips that Node.
#
# candidates  List of Nodes that can be flipped
#
# returns     flipped Node
def flip_single_node(candidates):

	# Iterates over the flippable nodes, tracking the best costing
	# flip (and the node responsible) throughout
	best_flip, node_to_flip = 0, None
	for node in candidates:
		cost, cost_if_flipped = 0, 0
		for edge in node.edges:
			# If the group is the same
			if edge.m.group == edge.n.group:
				cost_if_flipped += edge.weight
			# If the group is different
			else:
				cost += edge.weight

		# If the total cost would be greater upon flipping the node
		if cost_if_flipped > cost and cost_if_flipped > best_flip:
			best_flip = cost_if_flipped
			node_to_flip = node

	# Flips node if the flip would increase the total cost
	if node_to_flip != None:
		node_to_flip.group = not node_to_flip.group

	return node_to_flip


# ----------------------- Single-Flip Helpers -----------------------


# Returns True if the given Node is flippable i.e. if the Node,
#
#  1  Is not the source
#  2  Is not the sink
#  3  Contains at least one Edge that bridges the partitions (groups)
#  4  Flipping the node does not leave a neighbor without a path to
#     its source or sink
#
# node    The node to test if it can be validly flipped
# souce   The ID of the source
# sink    The ID of the sink
#
# returns  True if the Node meets the criteria to be flipped
def is_node_flippable(node, source, sink):

	return True

'''
	THIS LOGIC IS TEMPORARILY DISABLED

	# Create variables checking if the Node is the source or sink
	isSource = node.ID == source
	isSink = node.ID == sink
	if isSource or isSink:
		return False

	# Create variable to check if the Node has inter-group edges
	bridge_edges = [e for e in node.edges if e.n.group != e.m.group]
	hasBridgeEdge = len(bridge_edges) > 0
	
	# Create variable to check if the Node cuts neighbors off
	isolatesNeighbors = does_flip_cut_off_nodes(node, source, sink)
	if isolatesNeighbors == True or hasBridgeEdge == False:
		return False
	else:
		return True
'''


# Returns True if flipping the given Node would cut off its neighbors
# from paths to their source or sink.
#
# node    The node that would be flipped
# souce   The ID of the source
# sink    The ID of the sink
#
# returns  True if the Node cuts off any of its neighbors, else False
def does_flip_cut_off_nodes(node, source, sink):

	# Get the neighbors of the node that would be flipped
	neighbors = getNeighbors(node)

	# Check if each neighbor would maintain a path to their source
	# or sink if the current node was flipped
	for neighbor in neighbors:
		origin = source if neighbor.group == True else sink
		if find_path_to_origin(neighbor, origin, [node.ID]) == False:
			return True

	return False


# Returns True if a path exists from the given Node to the origin.
# A path is defined as a consecutive series of hops from one Node
# to another Node beloning to the same group that share an Edge.
# This is done by a recursive DFS where children are neighbors of
# the same group.
#
# node         The Node at the beginning the path
# origin       The ID of the source or the sink
# visited      A List of IDs of nodes already visted
#
# returns      True if a path exists between the node and the origin,
#              False otherwise.
def find_path_to_origin(node, origin, visited):

	# Return True if this Node is the origin Node
	if node.ID == origin:
		return True

	# Add node to the list of already visted nodes in this DFS
	visited.append(node.ID)

	# Get neighbors, excluding the visted nodes
	neighbors = getNeighbors(node)
	neighbors = [n for n in neighbors if n.group == node.group]
	neighbors = [n for n in neighbors if not n.ID in visited]

	# Recursivly search the list of neighbors for a path
	for neighbor in neighbors:
		if find_path_to_origin(neighbor, origin, visited) == True:
			return True
	return False


# -------------------- Graph Management Helpers ---------------------


# Returns a list of Nodes containing the neighbors of the given node
#
# node     The Node whose neighbors to return
#
# returns  The given Node's list of neighbors
def getNeighbors(node):
	neighbors = []
	for edge in node.edges:
		if edge.m.ID == node.ID:
			neighbors.append(edge.n)
		else:
			neighbors.append(edge.m)
	return neighbors


# Returns True if the given Edge connects with the given Node
#
# edge     The Edge to check if it connects with the Node
# node     The Node to check if it contains the Edge
#
# returns  True if the Edge connects with the Node
def isEdge(edge, node):
	if edge.m.ID == node.ID:
		return True
	elif edge.n.ID == node.ID:
		return True
	else:
		return False


# Calculates and returns the value of the cut i.e. the combined
# weights of the edges between vertices in group A and group B
# 
# edges    Edges between a vertex in A and a vertex in B
#
# returns  Integer value of the cut
def cost(edges):
	return sum([e.weight for e in edges])


# Calculates and returns the value of the cut i.e. the combined
# weights of the edges between vertices in group A and group B.
# 
# edges    List of all Edges in the graph
#
# returns  Integer value of the cut
def cost_of_cut(edges):

	# Returns True if Nodes n and m are in different partitions
	def diff_group(n, m):
		if n.group == True and m.group == False:
			return True
		if n.group == False and m.group == True:
			return True
		return False

	# gets the edges bridging the parititons
	bridging_edges = [e for e in edges if diff_group(e.n, e.m)]

	# computes the cost of the cut
	cost = sum(edge.weight for edge in bridging_edges)

	return cost
