
# -------------------------------------------------------------------
# Andrew Morato
# CS 631 Term Project
# May 5, 2020
# Max Cut Approximation Comparator
# -------------------------------------------------------------------

'''

        Functions that compare greedy and local search
    implementations of maximum cut approximations. Most of these
    involve repeated tests of both algorithms on the same randomly
    generated graph

'''

# Imports ---

import numpy as np
import random as r
import generator
import localsearch
import greedy
import matplotlib.pyplot as plt


# ---------------------- Comparison Functions -----------------------


# Generates a graph and runs local search and a single greedy
# approximation on it.
#
# groups   List of ints where each entry represents the size of the
#          corresponding group in the graph to generate
# display  Visually displays the graph with opencv if True
#
# returns  Tuple of the local search result and the greedy result
#          (local serch result, best greedy result)
def single_test(groups, display=False):

	print("\n|----- single test comparison -----|\n")

	graph = greedy.generate_graph(groups)

	g = greedy.approx_maxcut(graph=graph, display_graph=display)
	l = localsearch.approx_maxcut(graph=graph, display_graph=display)

	print(" >- greedy approximation: " + str(g))
	print(" >- local search approximation: " + str(l))

	print("\n|----- end single test -----|\n")

	return (l, g)


# Generates a graph and runs local search and a multiple greedy
# approximation on it.
#
# n        The number of greedy approximations run
# groups   List of ints where each entry represents the size of the
#          corresponding group in the graph to generate
# deg      Desired average degree per vertex in the generated graph
# term     Outputs excess information to the terminal
# display  Visually displays the graph with opencv if True
#
# returns  Tuple of the local search result and the best greedy
#          result (local serch result, best greedy result)
def repeated_test(n, groups, deg=2, term=False, display=False):

	if term:
		print("\n|----- single test comparison -----|\n")

	graph = greedy.generate_graph(groups, degree=deg)

	g = greedy.approx_maxcut(n, graph=graph, display_graph=display)
	l = localsearch.approx_maxcut(graph=graph, display_graph=display)

	if term:
		print(" >- best greedy approximation: " + str(g))
		print(" >- local search approximation: " + str(l))
		print("\n|----- end single test -----|\n")

	return (l, g)


# ----------------------- Graphing Functions ------------------------


# Sizes of graphs to test on 

G01 = [5] * 5 # 25
G02 = [8] * 9 # 72
G03 = [10] * 13 # 130
G04 = [14] * 13 # 182
G05 = [15] * 15 # 225
G06 = [17] * 16 # 272
G07 = [23] * 14 # 322
G08 = [29] * 13 # 377
G09 = [33] * 13 # 429
G10 = [39] * 12 # 468
G11 = [41] * 13 # 533
G12 = [47] * 12 # 564
G13 = [48] * 13 # 625
G14 = [57] * 12 # 684
G15 = [52] * 14 # 725
G16 = [56] * 14 # 784
G17 = [63] * 13 # 825
G18 = [67] * 13 # 871
G19 = [62] * 15 # 930
G20 = [70] * 14 # 980

G21 = [154] * 13 # 2002
G22 = [200] * 13 # 2002
G23 = [250] * 13 # 2002
G24 = [300] * 13 # 2002
G25 = [400] * 13 # 2002

GROUPS = [G01, G02, G03, G04, G05, G06, G07, G08, G09, G10]
GROUPS += [G11, G12, G13, G14, G15, G16, G17, G18, G19, G20]
GROUPS += [G21, G22, G23, G24, G25]


# Runs and plots the results of the local search and greedy
# approximations on a specified number of graphs with specified
# increments.
#
# n          The number of greedy approximations run per approx
# degree     The degree of the vertices
#
# returns  None
def experiment(n, degree):

	ls_results, greedy_results, sizes = [], [], []
	msg = "\n Case with degree " + str(degree) + " with greedy "
	msg += "approximations repeated " + str(n) + " time(s)\n"
	print(msg)

	for i in range(len(GROUPS)):

		group = GROUPS[i]
		size = len(group) * group[0]

		msg = " * Testing case #" + str(i+1) + " with " + str(size)
		print(msg + " vertices...")

		ls_result, greedy_result = repeated_test(n, group, degree)

		ls_results.append(ls_result)
		greedy_results.append(greedy_result)
		sizes.append(size)

	deg = "High" if degree > 3 else "Low"
	title = deg + " Degree, N=" + str(n)
	plot_results(title, greedy_results, ls_results, sizes)


# Plots the given greedy results and local search results against
# the given size of the graphs
#
# title    Subtitle to put in the plot
# greedy   Greedy algorithm results to graph against graph sizes
# ls       Local search results to graph against graph sizes
# sizes    Sizes of the graphs approximated on
#
# returns  None
def plot_results(title, greedy, ls, sizes):

	print("\n Plotting the results...\n")

	fig = plt.figure()
	fig.suptitle(title, fontsize=13)

	plt.plot(sizes, greedy, marker='o', label="greedy approx")
	plt.plot(sizes, ls, marker='o', label="local search approx")

	plt.legend(loc="upper left")
	plt.ylabel('Cost of the cut')
	plt.xlabel('Size of the graph')

	fig.savefig(title + ".png")
