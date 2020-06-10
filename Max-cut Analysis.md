# Experimental Analysis of the Max-cut Problem

&nbsp;&nbsp;&nbsp;&nbsp;The [Maximum-cut Problem](https://en.wikipedia.org/wiki/Maximum_cut) is an optimization problem on a graph comprised of vertices and edges where the goal is to partition the vertex set in such a way as to optimize the number of edges between both partitions of the vertices. This analysis compares two maximum-cut approximations under varying conditions, contrasting their speed, accuracy, and robustness in original implementations of both algorithms. The problem is unsolvable by any known polynomial time algorithm. Thus, only approximations can be offered when searching for an efficient, “good enough” solution. The first approach, found in Algorithm Design by Jon Kleinberg and Eva Tardos, uses a local search technique to search for a good approximation of an optimal cut in the plane of all possible cuts. The second approach is described in [Yet Another Algorithm for Dense Max Cut: Go Greedy](Project_Files/Maxcut_Analysis/maxcut_greedy_approach.pdf) by Clarie Mathieu and Warren Schudy and entails a simple greedy approach where vertices are put into the partition that would maximize the edges in the cut.

An introduction of the problem and overview of the approximation algorithms can be found in these [presentation slides](Project_Files/Maxcut_Analysis/max-cut-pres.pdf).

The whitepaper describing the analysis and discussing the experimental results is [here](Project_Files/Maxcut_Analysis/max-cut-paper.pdf).

The fully documented code to produce those experimental results can be found [here](Project_Files/Maxcut_Analysis/code)
