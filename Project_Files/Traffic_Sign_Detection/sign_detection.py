
# Sign Detection
# Author: Andrew Morato

# imports
import cv2
import numpy as np

'''

Real-time sign detection

	High-Level Algorithm

	For each frame in the video
		
	1 Color Threshold using HSV equivalents of each frame to isolate
	  sign colors only
	2 Produce an edge map of the thresholded image using sobel or canny
	3 Find contours in the binary edge map
	4 Refine the contours by their position and area (i.e eliminate
	  contours around the edges of the image or contours that define
	  an area too small for consideration)
	5 Classify all remaning contours into shapes (Triangle, Square,
	  Octagon, etc) by approximating the curve of the contours using
	  the Ramer–Douglas–Peucker algorithm. [opencv provides an
	  implementation of this]
	6 Draw a bounding box around the resulting shapes that are likely
	  to be signs

'''

# ---------------------------------------------------------------------
# --------------------------- Video Stream ----------------------------
# ---------------------------------------------------------------------

# Author: Andrew Morato
# Reads a video frame-by-frame. If the video is not a video file but
# a camera capture, pass 0 instead of the video file name
# In:
# 	filename      Path to the video file or 0 for camera livestream
# 	threshPct     Smallest percent of the image each contour has to 
# 	              cover to be considered
# Returns:  
# 	None
def stream(filename, threshPct=0.02):
	# opens the stream & checks validity
	cap = cv2.VideoCapture(filename)
	if (cap.isOpened() == False): 
		print("Error opening video stream or video file")

	frame_width = int(cap.get(3))
	frame_height = int(cap.get(4))
	fps = cap.get(cv2.CAP_PROP_FPS)
	out = cv2.VideoWriter('test34.avi',cv2.VideoWriter_fourcc('M','J','P','G'), fps, (frame_height,frame_width))

	# computes the Area and appropriate Threshold for contours
	ret, frame = cap.read()
	areaThresh = 0
	if ret == True:
		areaThresh = int((frame.shape[0] * frame.shape[1]) * threshPct)
	# loops until video is completed
	while(cap.isOpened()):
		# capture frame-by-frame
		ret, frame = cap.read()
		if ret == True:
			frame = np.rot90(frame)

			# -------- work with each frame of the video here ---------

			Color = colorSelect(frame, 'y')
			cv2.imshow('crs', Color)
			EdgeMap, cntrs = contours(Color, operator=1)
			Cntrs = drawContours(cv2.cvtColor(EdgeMap, cv2.COLOR_GRAY2BGR), cntrs, line=4)
			EdgeMap, cntrs = contours(Cntrs, operator=1, filter=True)
			cntrs = filterContours(cntrs)
			Img = drawContours(frame, cntrs)
			cv2.imshow('contours', Img)
			out.write(Img)

			if cv2.waitKey(1) & 0xFF == ord('q'):
				break # exit on 'q' key press

			# ---------------------------------------------------------

		else: 
			break

	# releases vides cap & destoys opencv windows
	cap.release()
	out.release()
	cv2.destroyAllWindows()

# ---------------------------------------------------------------------
# ----------------------------- Edge Map ------------------------------
# ---------------------------------------------------------------------

# Note: Applying any function that derives an edge map to each frame
#       of the the video stream will significantly slow down the
#       video's playback speed, thus computations are no longer in
#       real time and therefore useless. This can be mitigated by
#       skipping frames in the video stream (say execute computations
#       on every 5th frame) or having superior, dedicated hardware 
#       (likely the case for applications like self-driving cars) 

# ------------------------------- Sobel -------------------------------

# Author: Andrew Morato
# Isolates the edges using built-in opencv Sobel filters and computes
# the magnitude of horizontal and vertical edges
# In:
# 	Img        Input image [numpy 2D-array, uint8]
# 	ksize      Kernel size for the sobel filter 
# Returns:  
# 	Raw grayscale image [numpy 2D-array, int] where values indicate the
# 	degree (or intensity) of an edge 
def sobelMagnitude(Img, ksize=5):
	# produces sobel edge
	sobelx = cv2.Sobel(Img, cv2.CV_64F, 1, 0, ksize)
	sobely = cv2.Sobel(Img, cv2.CV_64F, 0, 1, ksize)
	# computes the magnitude
	magn = lambda x, y: np.sqrt(x**2 + y**2)
	Magnitude = magn(sobelx, sobely)
	return Magnitude.astype(int)

# Author: Andrew Morato
# Computes and normalizes the sobel EdgeMap using opencv functions
# In:
# 	Img        Input image [numpy 2D-array, uint8]
# 	thresh     Threshold for small edge magnitudes 
# Returns:  
# 	Normalized grayscale image [numpy 2D-array, uint8] where values
# 	indicate the degree (or intensity) of an edge 
def sobelEdge(Img, thresh=20):
	#Img = cv2.cvtColor(ColorImg, cv2.COLOR_BGR2GRAY)
	EdgeMagnitude = sobelMagnitude(Img)

	# normalizes the sobel EdgeMap to fit in a display-able range i.e
	# where values range from [0, 255]
	EdgeMagnitude = EdgeMagnitude * (255 / EdgeMagnitude.max())
	EdgeMap = EdgeMagnitude.astype(np.uint8)
	EdgeMap[EdgeMap < thresh] = 0
	return EdgeMap

# ------------------------------- Canny -------------------------------

# Author: Andrew Morato
# Isolates the edges using built-in opencv Canny edge detector
# In:
# 	Img        Input image [numpy 2D-array, uint8]
# 	thresh1    First Canny threshold
# 	thresh2    Second Canny threshold
# Returns:  
# 	Grayscale image [numpy 2D-array, uint8] where values are either 0
# 	or 255 (i.e. not a real binary image but an effective one)
def cannyEdge(Img, thresh1=100, thresh2=200):
	# Invokes opencv function to do all the work
	return cv2.Canny(Img, thresh1, thresh2)

# ---------------------------------------------------------------------
# --------------------- Shape Matching [Contours] ---------------------
# ---------------------------------------------------------------------

# Author: Andrew Morato
# Finds contours in the given Image. First produces a binary edge map
# (using Canny or Sobel), then computes the contours. Applies a
# threshold to eliminate any contours with a negligable area.
# In:
# 	Img        Input image [numpy 2D-array, uint8, grayscale or color]
# 	thresh     Threshold to determine 'negligable' contour areas
# 	operator   Edge Map detector [0 for Sobel, 1 for Canny]
# 	filter     If true, ignores contours with 'negligable' size
# Returns:  
# 	EdgeImg    Edge map of the input image
# 	contours   List of contours that survived the threshold
def contours(Img, thresh=300, operator=1, filter=False):
	# Computes a binary edge map
	EdgeMap = None
	if operator == 0: # sobel
		EdgeMap = sobelEdge(Img)
	else: # Canny
		EdgeMap = cannyEdge(Img)

	# Computes, Thresholds & Draws the Contours
	t1, t2 = cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
	_, contours, _ = cv2.findContours(EdgeMap, t1, t2)

	if filter == False:
		return EdgeMap, contours
	contourAreas = np.array([cv2.contourArea(i) for i in contours])
	contours = np.array(contours)
	contours = contours[contourAreas > thresh]
	return EdgeMap, contours


# Author: Andrew Morato
# Attempts to filter the contours to those that represents basic shapes
# such as triangles, rectangles, hexagons, octagons, etc. Uses opencv
# functions that implement the Ramer–Douglas–Peucker algorithm, which
# decimates a curve composed of line segments to a similar curve with 
# fewer points.
# In:
# 	contours   List of thresholded contours [produced from contours()]
# 	vertices   Threshold for the maximum number of vertices to consider
# 	epsilon    Accuracy parameter. 'How square-like must squares be'.
# 	           High percentages correspond to liberal classification 
# Returns:  
# 	filteredContours   List of contours that represent the probable
# 	                   locations of basic shapes in the input image
def filterContours(contours, vertices=6, epsilon=0.1):
	# Approximates shapes for each contour
	e, c, t, thresh = epsilon, contours, True, 100
	figs = [cv2.approxPolyDP(i, e*cv2.arcLength(i,t),t) for i in c]

	# Removes shapes with too many or too few vertices
	vts, figs = np.array([len(f) for f in figs]), np.array(figs)
	figs = figs[vts > 3]
	vts = np.array([len(f) for f in figs])
	figs = figs[vts <= vertices]

	# Removes areas that are too small
	contourAreas = np.array([cv2.contourArea(i) for i in figs])
	contours = np.array(figs)
	contours = contours[contourAreas > thresh]

	return contours

# ---------------------------------------------------------------------
# ------------------------ Color Thresholding -------------------------
# ---------------------------------------------------------------------

# Author: Andrew Morato
# Saturates the given color by the given scalar. This is intened to be
# used before color thresholding to facilitate segmneting the image
# In:
# 	Img        Image to be saturated [must be 3-channel]
# 	channel    Color channel to be saturated ['b','g', or 'r']
# 	scalar     Amount by which to saturate
# Returns:  
# 	Img witht the specified channel saturated by 'scalar'
def saturate(Img, channel='r', scalar=1.5):
	chan = 0 if channel=='b' else 1 if channel=='g' else 2
	ImgSat = Img.copy().astype(float)
	ImgSat[...,chan] = ImgSat[...,chan]*scalar
	ImgSat[ImgSat > 255] = 255
	return ImgSat.astype(np.uint8)

# Author: Andrew Morato
# Thresholds the given image to turn any values outside (or inside
# depending on the mode of operation) the range to (0,0,0)
# In:
# 	Img        Image to color threshold [should be 3-channel]
# 	lower      Lower bound of the range [BGR triplet]
# 	upper      Upper bound of the range [BGR triplet]
# 	mode       Operation mode i.e. disregard inside (0) or outside(1)
# 	colorspace Which color space to use [0=BGR, 1=HSV, 2=HSL]
# Returns:  
# 	Img thresholded to the given range [lower, upper]
def colorThreshold(Img, lower, upper, mode=1, colorspace=0):
	Img = Img.copy()
	if colorspace == 0: # Standard BGR (Blue-Green-Red)
		pass
	elif colorspace == 1: # HSV (Hue-Saturation-Value)
		#lower = cv2.cvtColor(np.uint8([[lower]]), cv2.COLOR_BGR2HSV)
		#upper = cv2.cvtColor(np.uint8([[upper]]), cv2.COLOR_BGR2HSV)
		Img = cv2.cvtColor(Img, cv2.COLOR_BGR2HSV)
	elif colorspace == 2: # HSL (Hue-Saturation-Lightness)
		pass
	mask = cv2.inRange(Img, lower, upper)
	if mode == 0:
		Img[mask > 0] = [0,0,0]
	else:
		Img[mask == 0] = [0,0,0]
	return Img

# Author: Andrew Morato
# Thresholds the given image to turn any values that are not of the
# specified color to zero
# In:
# 	Img        Image to color threshold [should be 3-channel]
# 	color      Color to isolate
# Returns:  
# 	Img        Image that preserves areas only of the specified color
def colorSelect(Img, color):
	Img = Img.copy()
	lower, upper = [], []
	if color == 'y': # yellow
		lower = (22, 100, 30)
		upper = (35, 255, 255)
		Img = cv2.cvtColor(Img, cv2.COLOR_BGR2HSV)
	elif color == 'w': # white
		lower = (210, 210, 210)
		upper = (255, 255, 255)
	elif color == 'r': # red
		lower = (0, 50, 50)
		upper = (5, 255, 255)
		Img = cv2.cvtColor(Img, cv2.COLOR_BGR2HSV)
	elif color == 'g': # green
		lower = (45, 50, 50)
		upper = (60, 255, 255)
		Img = cv2.cvtColor(Img, cv2.COLOR_BGR2HSV)
	mask = cv2.inRange(Img, lower, upper)
	Img[mask == 0] = [0,0,0]
	return Img

# ---------------------------------------------------------------------
# ------------------------- Assorted Helpers --------------------------
# ---------------------------------------------------------------------

# makes an Image with the given dimensions with a square in the center 
def makeImg(width, height):
	Img = np.zeros((width, height, 3), np.uint8)
	Img[:] = 0
	# Central Square Dimensions
	sq_w, sq_h = int(width/2), int(height/2)
	rem_w, rem_h = width - sq_w, height - sq_h
	head_w, head_h = int(rem_w / 2), int(rem_h / 2)
	In = np.zeros((sq_w, sq_h, 3), np.uint8)
	In[:] = (0,0,255)
	Img[head_w:head_w+sq_w, head_h:head_h+sq_h] = In
	return Img

# morphological operations & basic image preprocessing functions
def erodeImg(Img, amnt=5):
	kernel = np.ones((amnt,amnt), np.uint8)
	return cv2.erode(Img, kernel, iterations=1)
def dilateImg(Img, amnt=5):
	kernel = np.ones((amnt,amnt), np.uint8)
	return cv2.dilate(Img, kernel, iterations=1)
def gaussian(Img, ksize=5):
	return cv2.GaussianBlur(Img, (ksize,ksize), cv2.BORDER_DEFAULT)
def equalization(Img):
	return cv2.equalizeHist(Img)

# Author: Andrew Morato
# Draws and displays the contours
# In:
# 	Img        Image to draw on [should be 3-channel]
# 	contours   List of contours to draw
# 	color      Color to draw the contours
# 	line       Line thickness when drawing the contours
# 	display    Displays the contours or just reutrns the drawn image
# Returns:  
# 	ImgCnt     Contours drawn on a copy of the input Img
def drawContours(Img, contours, color=(0,255,0), line=2,display=False):
	ImgCnt = Img.copy()
	for c in contours:
		x, y, w, h = cv2.boundingRect(c)
		cv2.rectangle(ImgCnt, (x, y), (x + w, y + h), color, line)
	#ImgCnt = cv2.drawContours(ImgCnt, contours, -1, color, line)
	if display == True:
		cv2.imshow('contours', ImgCnt)
		cv2.waitKey(0) & 0xFF == ord('q')
	return ImgCnt
