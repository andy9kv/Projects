
# -------------------------------------------------------------------
# Andrew Morato
# CS 631 Term Project
# May 5, 2020
# Undirected Weighted Graph Visualizer
# -------------------------------------------------------------------

'''

        Code to display an undirected, weighted graph using opencv.
    Accepts graphs of comprised of Node and Edge objects of the form:

        [s][v1][v2][v3][s]...[vn][e]

    where s is the source node, v1, v2, v3, ..., vn are groups of
    Node objects with edges connecting to the previous and next
    groups, t is the sink node, and [e] is the group of all Edge
    objects. The graph is displayed by drawing all vertices from
    each group in their own "column" and connecting edges between all
    drawn vertices. 

'''

# Imports ---

import cv2
import numpy as np
import localsearch


# Global Variables ---

# Colors in BGR
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
BACKGROUND = (210, 210, 210)
GRAY = (100, 100, 100)
BLUE = (191, 114, 15)
ORANGE = (46, 125, 209)
RED = (80, 53, 219)
TEAL = (69, 133, 69)

# Text and Image
RAD = 20
OFFSET = 5
FONT = cv2.FONT_HERSHEY_SIMPLEX
WINDOW = 200 #Window width for displaying information

# Cost Tracking
BEST_COST = 0

# Mouse Position
xPos, yPos = -1, -1

# Interactive Visualizer ---

# Displays an OpenCV image representation of the given graph. Creates
# a blank image that will fit the graph and populates it with a
# simple but adequate representation of the graph
#
# graph    Graph to visualize. Has the format described in this
#          file's description
#
# returns  None
def visualize(graph):
	global xPos, yPos

	# set the vertex locations in the image
	triplets, dim = set_vertex_locations(graph[0], WINDOW)

	# create a blank image
	Img = np.zeros(dim, np.uint8)
	Img[:] = BACKGROUND

	# draw the graph
	edges = graph[1]
	Graph = draw_graph(Img.copy(), triplets, edges)

	# seperate edges that join [A, B]
	E = [e for e in edges if e.n.group != e.m.group]

	# populate display window
	Graph = disp_window(Graph, WINDOW, E, "Initial Randomized Graph")

	# display
	cv2.namedWindow('graph')
	cv2.setMouseCallback('graph', on_mouse_clicked)
	cv2.imshow('graph', Graph)

	while True:

		# exit the interactive display
		k = cv2.waitKey(5) & 0xFF
		if k == 27 or k == ord('q'):
			break

		# mouse clicked, handle user input
		if xPos != -1:

			# determine where the click occured and handle each case
			code = check_location(triplets, xPos, yPos)

			# Mouse click location is not in a Node
			if code == -4:
				msg = "Clicked empty space"

			# Mouse click location is in the Source (unswitchable)
			elif code == -3:
				msg = "Selected source vertex"

			# Mouse click location is in the Sink (unswitchable)
			elif code == -2:
				msg = "Selected sink vertex"

			# Mouse click location is another unswitchable Node
			elif code == -1:
				msg = "Selected unflippable vertex"

			# Mouse click location is in a valid Node with that ID 
			elif code >= 0:
				msg = "Flipped vertex " + str(code)
				flip(triplets, code)
				E = [e for e in edges if e.n.group != e.m.group]

			# redraw the display window and graph
			Graph = disp_window(Img.copy(), WINDOW, E, msg)
			Graph = draw_graph(Graph, triplets, edges)

			# display the graph
			cv2.imshow('graph', Graph)

			xPos = -1

	cv2.destroyAllWindows()


# Runs when the mouse was clicked. Sets global variables xPos and
# yPos when the mouse's left button was clicked
#
# event    Mouse event
# x        x position of the cursor
# y        y position of the cursor
# f        passed over
# p        passed over
#
# returns  None
def on_mouse_clicked(event, x, y, f, p):
	global xPos, yPos
	if event == cv2.EVENT_LBUTTONDOWN:
		xPos, yPos = x, y


# Processing ---

# Flips the Node with the given ID
#
# triplets  triplet of Nodes to lookup the Node by ID
# ID        ID of the node to flip
#
# returns   None
def flip(triplets, ID):
	node = None
	for triplet in triplets:
		if ID == triplet[2].ID:
			node = triplet[2]
	node.group = not node.group
	group = "B" if node.group == True else "A"
	opposite = "A" if group == "B" else "B"
	msg = "[ Flipped vertex " + str(ID) + " from group " + group
	msg += " to group " + opposite + " ]"
	print(msg)


# Maps out vertex location for all vertices in each vertex group
#
# v        Set of groups of vertices
# win      Window width for displaying information
# height   Vertical distance between vertices
# width    Horizontal distance between vertex groups
# marg     Margin - padding size
#
# returns  Tuple of list of triplets for all vertices: (x, y, Node)
#          and the Img to draw the graph's shape
def set_vertex_locations(v, win, height=125, width=100, marg=65):
	triplets = []
	left = marg

	# calculate the midpoint of the Img
	biggest_group = max(v, key=len)
	middle = int(((len(biggest_group) - 1) * height) / 2) + marg

	for groups in v:
		# calculates the horizontal and vertical starting point
		nodes_below = int(len(groups) / 2)
		if len(groups) % 2 == 0:
			nodes_below = nodes_below - 0.5
		bottom = int(middle - (nodes_below * height))

		# adds locations
		for vertex in groups:
			triplets.append((left, bottom, vertex))
			bottom += height
		left += width
			
	Img_shape = (middle*2, ((len(v)-1) * width) + (marg*2) + win, 3)
	return (triplets, Img_shape)


# Users can click a location on the Graph. Based on the location,
# checks if the location specified by x and y is within a Node in
# the Graph. Returns a code based on the location's characteristics.
#
# triplets  A list of triplets of all Nodes and their x and y
#           positions where each entry has the the form (x, y, Node)
# x         The x position of the location to check
# y         The y position of the location to check
#
# returns   Return code (int) for the following situations
#             -4   Location is not in a Node
#             -3   Location is in the Source (unflippable)
#             -2   Location is in the Sink (unflippable)
#             -1   Location is another unswitchable Node
#              0+  Location is in a valid Node with that ID 
def check_location(triplets, x, y):
	for triplet in triplets:
		if pt_is_in_circle((triplet[0], triplet[1]), (x, y)):

			# Point is in the source
			if triplet[2].ID == 0:
				return -3

			# Point is in the sink
			elif triplet[2].ID == len(triplets)-1:
				return -2

			# Point is in a Node that has no neighbors in the
			# opposing parition
			elif not is_frontier_node(triplet[2]):
				return -1

			# Point is in a valid, flippable Node
			else:
				return triplet[2].ID

	# Point is not within any Node
	return -4


# Calculates if the point is within the circle
#
# c         (x, y) tuple representing the center of the circle
# p         (x, y) tuple representing the point
#
# returns   True if the point resides in this circle, else False
def pt_is_in_circle(c, p):
	dist = np.sqrt(np.square(p[0] - c[0]) + np.square(p[1] - c[1]))
	return dist <= RAD


# Returns True if the given Node is a frontier node, i.e. any of
# its neighbors are in the group opposite to itself
#
# node      The Node to check
#
# returns   True if the given node is a frontier node, else False
def is_frontier_node(node):
	for e in node.edges:
		if e.n.group != e.m.group:
			return True
	return False


# Drawing ---

# Populates the display window with relevant information
#
# Img       OpenCV image to fill the display window
# win       Window width for displaying information
# E         Edges that join partition [A, B]
# msg       Message to print in the display window
# marg      Margin - vertical padding for the display window
#
# returns   The modified OpenCV image
def disp_window(Img, win, E, msg, marg=90):
	
	global BEST_COST

	# Draw bounding line
	h, w, _ = Img.shape
	pt1, pt2 = (w-win, h-marg), (w-win, marg)
	Img = cv2.line(Img, pt1, pt2, BLACK, thickness=2)
	begin_text_width, midpoint = (w-win) + 20, int(h/2)

	# Current Cost
	cost = localsearch.cost(E)
	text = "cost of current cut: " + str(cost)
	pt = (begin_text_width, midpoint)
	Img = cv2.putText(Img, text, pt, FONT, 0.4, BLACK)

	# Update best cost
	BEST_COST = cost if cost > BEST_COST else BEST_COST

	# Best Cost
	text = "cost of best cut: " + str(BEST_COST)
	pt = (begin_text_width, midpoint - 50)
	Img = cv2.putText(Img, text, pt, FONT, 0.4, BLACK)

	# Print Message
	pt = (begin_text_width, midpoint + 50)
	Img = cv2.putText(Img, msg, pt, FONT, 0.4, BLACK)

	return Img


# Draws the labelled vertices and edges. Wrapper function for
# draw_edges and draw_vertices. 
#
# Img       OpenCV image to draw the graph
# triplets  List of vertices belonging to the graph in triplet form:
#           (x-position, y-position, Node)
# edges     List of Edge objects in the graph
#
# returns   The modified OpenCV image
def draw_graph(Img, triplets, edges):
	Img = draw_edges(Img, triplets, edges)
	Img = draw_vertices(Img, triplets)
	return Img


# Draws the vertices and labels them with their IDs on an OpenCV
# image (called after draw_edges)
#
# Img       OpenCV image to draw the vertices
# triplets  List of vertices belonging to the graph in triplet form:
#           (x-position, y-position, Node)
#
# returns   The modified OpenCV image
def draw_vertices(Img, triplets):
	for triplet in triplets:

		# Gets the coordinates of the Node
		v = (triplet[0], triplet[1])

		# Set the color of the Node
		col = GRAY
		if triplet[2].group != None:
			col = RED if triplet[2].group == False else BLUE

		# Draw the Node
		Img = cv2.circle(Img, v, RAD, color=col, thickness=-1)
		Img = cv2.circle(Img, v, RAD, color=BLACK, thickness=2)

		# Write the Node ID
		ID = str(triplet[2].ID)
		if len(ID) == 2:
			v = (triplet[0] - (OFFSET + 7), triplet[1] + (OFFSET + 2))
		else:
			v = (triplet[0] - (OFFSET + 1), triplet[1] + OFFSET)
		Img = cv2.putText(Img, ID, v, FONT, 0.6, WHITE)

	return Img


# Draws the edges on an OpenCV image. Finds the position of each
# edge's vertices and draws an edge between both points, labeled
# with its weight
#
# Img       OpenCV image to draw the edges
# triplets  List of vertices belonging to the graph in triplet form:
#           (x-position, y-position, Node)
# edges     List of Edge objects in the graph
#
# returns   The modified OpenCV image
def draw_edges(Img, triplets, edges):

	# Gets the location of the vertex with the given ID
	#
	# ID        ID of the vertex being searched for
	# triplets  List of vertices belonging to the graph in triplet form:
    #           (x-position, y-position, vertex ID)
    #
    # returns   Location (x, y) of the vertex with the given ID
	def getVertexLocation(ID, triplets):
		for triplet in triplets:
			if ID == triplet[2].ID:
				return (triplet[0], triplet[1])
		print("ERROR: No ID found")

	for edge in edges:
		n_loc = getVertexLocation(edge.n.ID, triplets)
		m_loc = getVertexLocation(edge.m.ID, triplets)

		# set color
		col = BLACK
		if edge.n.group != edge.m.group:
			col = TEAL

		# draw line
		Img = cv2.line(Img, n_loc, m_loc, color=col, thickness=2)

		# find a point between both nodes and label weight
		m = (int((n_loc[0]+m_loc[0])/2), int((n_loc[1]+m_loc[1])/2))
		m = (int((n_loc[0]+m[0])/2), int((n_loc[1]+m[1])/2 - 7))
		w = str(edge.weight)
		Img = cv2.putText(Img, w, m, FONT, 0.45, ORANGE, thickness=2)

	return Img


# Display Mode ---

# As opposed to interactive mode, this mode takes no user input other
# than 'n' for the next flip

# Displays the entrie graph and output window
#
# nodes     A list of Nodes of the graph
# edges     A list of Edges of the graph
# msg       The message to display
# cost      The cost of the current cut
#
# returns   None
def display_graph(nodes, edges, msg, cost):

	# set the vertex locations in the image
	triplets, dim = set_vertex_locations(nodes, WINDOW)

	# create a blank image
	Img = np.zeros(dim, np.uint8)
	Img[:] = BACKGROUND

	# draw the graph
	G = draw_graph(Img.copy(), triplets, edges)

	# seperate edges that join [A, B]
	bridge_edges = [e for e in edges if e.n.group != e.m.group]

	# populate display window
	G = display_window(G, WINDOW, bridge_edges, msg, cost)

	# display
	cv2.imshow('graph', G)

	while True:
		k = cv2.waitKey(5) & 0xFF
		if k == 27 or k == ord('n'):
			break


# Populates the display window with relevant information
#
# Img             OpenCV image to fill the display window
# win             Window width for displaying information
# bridging_edges  Edges that join partition [A, B]
# msg             Message to print in the display window
# cost            Cost of the current cut
# marg            Margin - vertical padding for the display window
#
# returns   The modified OpenCV image
def display_window(Img, win, bridge_edges, msg, cost, marg=90):
	
	# Draw bounding line
	h, w, _ = Img.shape
	pt1, pt2 = (w-win, h-marg), (w-win, marg)
	Img = cv2.line(Img, pt1, pt2, BLACK, thickness=2)
	begin_text_width, midpoint = (w-win) + 20, int(h/2)

	# Current Cost
	text = "cost of cut: " + str(cost)
	pt = (begin_text_width, midpoint - 25)
	Img = cv2.putText(Img, text, pt, FONT, 0.4, BLACK)

	# Print Message
	pt = (begin_text_width, midpoint + 25)
	Img = cv2.putText(Img, msg, pt, FONT, 0.4, BLACK)

	return Img
