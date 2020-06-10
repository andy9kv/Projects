
# Imports ---

import visualizer
import generator as gen
import localsearch
import greedy
import sys
import comparator

# Constants ---

WEIGHT_RANGE = 15
DISPLAY = False
LOW_DEGREE=2
HIGH_DEGREE=6

# Graphs
SMALL = [5] * 5
MID = [15] * 10
LARGE = [30] * 15
HUGE = [50] * 20

#comparator.single_test(groups=HUGE, display=DISPLAY)
#comparator.repeated_test(n=5, groups=HUGE, display=DISPLAY)
comparator.experiment(n=30, degree=HIGH_DEGREE)
'''
# n=1
comparator.experiment(n=1, degree=LOW_DEGREE)
comparator.experiment(n=1, degree=HIGH_DEGREE)

# n=10
comparator.experiment(n=10, degree=LOW_DEGREE)
comparator.experiment(n=10, degree=HIGH_DEGREE)

# n=25
comparator.experiment(n=25, degree=LOW_DEGREE)
comparator.experiment(n=25, degree=HIGH_DEGREE)
'''