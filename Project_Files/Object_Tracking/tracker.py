
import numpy as np
import cv2
import sys

# -------------------------------------------------------------------
#                            Administrative
# -------------------------------------------------------------------

# Constants

BLACK = [0,0,0]
WHITE = [255, 255, 255]
GREEN = [0, 255, 0]
MAX_PIXELS = 200

# Function to load up definitions in the interpreted terminal

def wrapper(path, areaSize=5):
	buildVideo(path)
	kpVideo(path)

def trackSeveral():
	#trackObj("Dataset/Boat/img/")
	#trackObj("Dataset/MotorcycleChase/img/")
	trackObj("Dataset/IceSkating/img/")
	#trackObj("Dataset/Drone2/img/")
	#trackObj("Dataset/Jet1/img/")
	#trackObj("Dataset/DriftCar1/img/")
	#trackObj("Dataset/DriftCar2/img/")

# -------------------------------------------------------------------
#                       Video/Img Managing
# -------------------------------------------------------------------


# builds the video from the collection of frames in the given path
def buildVideo(path):
	# creates a video with sufficient parameters
	Img = cv2.imread(path + "00001.jpg")
	h, w = Img.shape[:2]
	video_name = path + "video.avi"
	fourcc = cv2.VideoWriter_fourcc(*'MPEG')
	video = cv2.VideoWriter(video_name, fourcc, 20.0, (w,h))
	#fourcc = cv2.VideoWriter_fourcc(*'DIVX')
	#video = cv2.VideoWriter(video_name, fourcc, 15.0, (w, h), False)

	# loads up all the frames to the video
	count = 1
	zeros = "00000"
	while Img is not None:
		video.write(Img)
		count += 1
		length = len(str(count))
		name = path + zeros[length:] + str(count)  + ".jpg"
		Img = cv2.imread(name)
	cv2.destroyAllWindows()
	video.release()

# loads up each frame in the directory and writes it to a video
def trackObj(path, areaSize=8):
	# open the video file
	cap = cv2.VideoCapture(path + "kp_video.avi")
	cap2 = cv2.VideoCapture(path + "video.avi")

	# Sets several values to assist writing to the video
	out_file = path + "tracked_mask.avi"
	out_file2 = path + "org_tracked_mask.avi"
	frame_rate = int(cap.get(cv2.CAP_PROP_FPS))
	fh = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
	fw = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
	frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

	# second capture
	frame_rate2 = int(cap2.get(cv2.CAP_PROP_FPS))
	fh2 = int(cap2.get(cv2.CAP_PROP_FRAME_HEIGHT))
	fw2 = int(cap2.get(cv2.CAP_PROP_FRAME_WIDTH))
	fcc2 = int(cap2.get(cv2.CAP_PROP_FOURCC))

	# Defines VideoWriter object
	out = cv2.VideoWriter(out_file, fcc, frame_rate, (fw,fh))
	out2 = cv2.VideoWriter(out_file2, fcc2, frame_rate2, (fw2,fh2))

	# loads the first images needed for initial object detection
	fstImg = cv2.imread(path + "00001.jpg")
	featureImg = cornerDetector(fstImg, 0)
	trainImg = cv2.imread(path + "train.png")
	size = trainImg.shape[:2]

	# Initial object detection in the first frame
	coords, mask = matchKP(featureImg, trainImg)[1:]
	count = 0
	# loops through each frame
	while(cap.isOpened()):
		# grabs the next frame
		ret, frame = cap.read()
		ret2, Img2 = cap2.read()

	    #if reached video's end, exit function
		if ret == False:
			break

	    # draws a rectangle containing the tracked object
		Img = cornerDetector(frame, 0)
		triplet = maskOverArea(Img, mask, coords, areaSize, size)
		pct, coords, newMask = triplet
		Imgs = iterativeMasking(Img, coords, mask, size)
		buildVideoFrames(Imgs)
		Img = drawMask(Img, mask, coords, size)
		mask = newMask
		absPt = (coords[1] + size[1], coords[0] + size[0])
		cv2.rectangle(Img2, (coords[1], coords[0]), absPt, GREEN, 2)

        # write the tracked frame to the output video
		out.write(Img)
		out2.write(Img2)
		
		print("#", count, " out of ", frames, " | confidence: ", pct)
		count+=1

	# Release everything if job is finished
	cap.release()
	out.release()
	cv2.destroyAllWindows()


# tracks the object, displaying the KP image and mask overlay
def trackWithMask(path, areaSize=8):
	# open the video file
	cap = cv2.VideoCapture(path + "video.avi")

	# Sets several values to assist writing to the video
	out_file = path + "mask_video.avi"
	frame_rate = int(cap.get(cv2.CAP_PROP_FPS))
	fh = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
	fw = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
	frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

	# Defines VideoWriter object
	out = cv2.VideoWriter(out_file, fcc, frame_rate, (fw,fh))

	# loads the first images needed for initial object detection
	fstImg = cv2.imread(path + "00001.jpg")
	featureImg = cornerDetector(fstImg, 0)
	trainImg = cv2.imread(path + "train.png")
	size = trainImg.shape[:2]

	# prepares for obj tracking
	coords, mask = matchKP(featureImg, trainImg)[1:]
	count = 0

	# loops through each frame
	while(cap.isOpened()):
		# grabs the next frame
		ret, frame = cap.read()

	    #if reached video's end, exit function
		if ret == False:
			break

	    # tracks the object and displays related marks
		Img = cornerDetector(frame, 0)
		triplet = maskOverArea(Img, mask, coords, areaSize, size)
		pct, coords, newMask = triplet
		Img = drawMask(Img, mask, coords, size)
		mask = newMask

        # write the tracked frame to the output video
		out.write(Img)
		 # displays progress
		print("#", count, " out of ", frames, " | confidence: ", pct)
		cv2.imshow('frame',Img)
		cv2.waitKey(1)
		count+=1

	# Release everything if job is finished
	cap.release()
	out.release()
	cv2.destroyAllWindows()

# Turns a video into a simple video of keypoints
def kpVideo(path):
	# open the video file
	cap = cv2.VideoCapture(path + "video.avi")

	# Sets several values to assist writing to the video
	out_file = path + "kp_video.avi"
	frame_rate = int(cap.get(cv2.CAP_PROP_FPS))
	fh = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
	fw = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
	fcc = int(cap.get(cv2.CAP_PROP_FOURCC))
	frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

	# Defines VideoWriter object
	out = cv2.VideoWriter(out_file, fcc, frame_rate, (fw,fh))
	count = 0

	# loops through each frame
	while(cap.isOpened()):
		# grabs the next frame
		ret, frame = cap.read()

	    #if reached video's end, exit function
		if ret == False:
			break

	    # draws a rectangle containing the tracked object
		Img = cornerDetector(frame, 0)

        # write the tracked frame to the output video
		out.write(Img)
		print("#", count, " out of ", frames) # displays progress
		count+=1

	# Release everything if job is finished
	cap.release()
	out.release()
	cv2.destroyAllWindows()


# -------------------------------------------------------------------
#                          Corner Detector
# -------------------------------------------------------------------

# input <- 
# 	img_file   file path of the image
# 	dst_file   file path of file where we will write the result to
# 	blockSize  size of the blocks upon which the detection is 
# 	           performed
# 	k          constant in the harris corner detection formula, a
#              measure of how corner-like corners have to be for them
# 	           to be detected
#   opt        returns a black image with white keypoints if 0,
#              otherwise returns the original image with red KP's
#
# detects the corners in the image using the harris
# corner detection method in OpenCV. Returns the same image with
# they found keypoints highlighted
def cornerDetector(Img, opt, blockSize=2, k=0.04):
	ksize = 1 # default constant value

	# opens the image and flips it to grayscale
	#Img = cv2.imread(img_file)
	grayscaleImg = cv2.cvtColor(Img, cv2.COLOR_BGR2GRAY)

	# performs the corner detection with the specified parameters
	grayscaleImg = np.float32(grayscaleImg)
	detectedCornersImg = cv2.cornerHarris(grayscaleImg, blockSize, ksize, k)

	# marks the corners in the original image
	dcImg = cv2.dilate(detectedCornersImg, None)

	# writes to a given image or the input
	if opt != 0:
		Img[dcImg > 0.01 * dcImg.max()] = [0,0,255]
		return Img
	else:
		height, width = Img.shape[:2]
		blackImg = np.zeros((height,width,3), np.uint8)
		blackImg[dcImg > 0.01 * dcImg.max()] = [255,255,255]
		return blackImg


def iterativeMasking(kpImg, previousLocation, mask, maskSize, areaSize=10):
	pts = getNeighboringCoords(previousLocation, areaSize)
	Imgs = []
	for p in pts:
		Img = kpImg.copy()
		frame = drawMask(Img, mask, p, maskSize)
		Imgs.append(frame)
	return Imgs

# builds the video from the collection of frames given
bvf = 1
def buildVideoFrames(ImgFrames):
	global bvf
	h, w = ImgFrames[0].shape[:2]
	video_name = "video(" + str(bvf) + ").avi"
	fourcc = cv2.VideoWriter_fourcc(*'MPEG')
	video = cv2.VideoWriter(video_name, fourcc, 20.0, (w,h))
	bvf += 1
	# loads up all the frames to the video
	for Img in ImgFrames:
		video.write(Img)
	video.release()

# -------------------------------------------------------------------
#                         Image Matching
# -------------------------------------------------------------------

# extracts the key points from the image
def extractKPs(objImg):
	h, w = objImg.shape[:2]
	keyPoints, k = [None] * (h*w), 0
	for j in range(0, h):
		for i in range(0, w):
			if (objImg[j][i] != BLACK).all():
				keyPoints[k] = (j, i)
				k+=1
	# filters out empty spaces
	return (list(filter(None,keyPoints)), keyPoints[0])

# extracts the key points from the image
def extractMaskFromRange(objImg, startPoint, endPoint):
	h, w = startPoint
	h2, w2 = endPoint
	mask = []
	for j in range(h, h2):
		for i in range(w, w2):
			if (objImg[j][i] != BLACK).all():
				mask.append((j-h, i-w))
	# filters out empty spaces
	return mask

# matches a mask
def matchKP(Img, trainImg):
	# extracts key features from the training img and possible
	# locations for those key points from Img
	KPs, offset = extractKPs(trainImg)
	OPs = extractKPs(Img)[0]
	hWin, wWin = trainImg.shape[:2]
	h, w = Img.shape[:2]

	# finds the best Key Point match in Img
	bestMatch = (0, (0,0))
	for k in range(0, len(OPs)):
		# sets up the origin point for the next window to apply the
		# matching algorithim
		candaditePointY,  candaditePointX = OPs[k]
		yOffset, xOffset = offset
		origin = candaditePointY - yOffset, candaditePointX - xOffset

		# attempts to match the KeyPoints obtained from the training
		# set to current window from Img
		match = matchMask(Img, origin, KPs)
		print(k, " out of ", len(OPs), ", pct: ", match[0])
		bestMatch = match if match[0] > bestMatch[0] else bestMatch
		if bestMatch[0] == 100:
			return bestMatch

	print(bestMatch)
	x1, y1 = bestMatch[1]
	x2, y2 = x1 + hWin, y1 + wWin

	#cv2.rectangle(Img, (y1, x1), (y2, x2), GREEN, 2)

	return ((x1, y1), bestMatch[0])


# Arguments: 
#	ImgKP - Mat with Key Points isolated
#	subArea - Defines the sub area of the Mat to compare in the form
#             of a rectangle's coordinates. [(xo, y0), w, h]
#	ObjKP - The original object's Key Points isolated
#	Comparator - The means by which to compare if ImgKP and ObjKP are
#                equal
#
# Attempts to match the key points from the given Mat sub-area to the
# Mat holding the original key points. There will be a match if both
# Mats are the same under the given comparator. Returns True on match
def matchMask(Img, originPoint, KPs):
	# declares terms to facilitate the matching
	matched_KPs = 0
	h, w = originPoint
	hLimit, wLimit = Img.shape[:2]
	newMask = []

	# aquires the percent of key points found in this image
	for k in range(len(KPs)):
		hk, wk = KPs[k]
		actual_w, actual_h = w + wk, h + hk
		if actual_w < wLimit and actual_h < hLimit:
			if (Img[actual_h][actual_w] != BLACK).all():
				matched_KPs += 1

	return (int((matched_KPs/len(KPs))*100), originPoint, KPs)

# -------------------------------------------------------------------
#                         Image Manipulation
# -------------------------------------------------------------------

# Note: All of these functions are operations on OpenCV images, i.e.
#       Mats (matrices). The functions below take in Mats and return
#       Mats. Naming convention is Img


# Resizes a matrix until its height and width are less than maxSize.
# Preserves the original Img and its aspect ratio.
def scaleTo(Img, maxSize):
	h, w = Img.shape[:2]
	largerVal = h if h > w else w
	if largerVal <= maxSize:
		return (Img, 1)
	else:	
		scalingFactor = 1 / (largerVal / maxSize)
		return (resize(Img, scalingFactor), scalingFactor)


# Takes an OpenCV Img and resizes by a factor of 'scalingFactor'
# Note: scalingFactor can scale to smaller images (less than 1) and
# larger images (more than 1)
def resize(Img, scalingFactor):
	sf = scalingFactor
	res = cv2.resize(Img, None, fx=sf, fy=sf,
					 interpolation=cv2.INTER_AREA)
	return res

# converts an image to a binary image and produces a list of contours
# found from the binary image
def getContours(Img):
	grayImg = cv2.cvtColor(Img, cv2.COLOR_BGR2GRAY)
	v, binImg = cv2.threshold(grayImg, 1, 255, cv2.THRESH_BINARY)
	res = cv2.findContours(binImg, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
	modImg, contours, hierarchy = res

	cv2.drawContours(modImg, contours, -1, (0,255,0), 3)
	cv2.imshow("gray", grayImg)
	cv2.imshow("bin", binImg)
	cv2.imshow("img", modImg)
	cv2.waitKey(0)

	return binImg


# -------------------------------------------------------------------
#                      Frame-To-Frame Tracker
# -------------------------------------------------------------------

# Tracks object from frame to frame, based on the key observation
# that the key points in the object are expected to change a small
# amount in each instance. The high level algorithm is to convolve
# the mask of the previous key points of the detected object in the
# previous frame with the key points from similar locations in the 
# current frame.

# method that ties the object detection and frame by frame tracking
# together
def track(Img, trainImg, areaSize, nextFrame):
	# initial object detection
	percentage, coordinates, mask = matchKP(Img, trainImg)
	# call with next frames, use video stream at this point!!!
	maskOverArea(nextFrame, mask, coordinates, areaSize)


# convolves the mask of previous key points over neighboring
# locations in the current frame
def maskOverArea(Img, mask, previousLocation, areaSize, maskSize):
	coordinates = getNeighboringCoords(previousLocation, areaSize)
	triplet = (0, previousLocation, mask)
	for c in coordinates:
		pct, pt, newMask = matchMask(Img, c, mask)
		triplet = (pct, pt, newMask) if pct > triplet[0] else triplet 
	pt = triplet[1]
	endPt = (pt[0] + maskSize[0], pt[1] + maskSize[1])
	newMask = extractMaskFromRange(Img, pt, endPt)
	return (triplet[0], pt, newMask)


# produces a list of coordinates to try matches on
def getNeighboringCoords(previousLocation, searchBreadth):
	xo, yo = previousLocation
	totalLength, coordinates = (2 * searchBreadth) + 1, []
	j, width = searchBreadth, 1
	while width < totalLength:
		offset = int(width/2)
		for k in range(0, width):
			coordinates.append((k - offset, j))
		width, j = width + 2, j - 1
	# replicates the top half but with the y coordinates reversed
	lowerCoordinates = []
	for point in coordinates:
		lowerCoordinates.append((point[0], -point[1]))
	# builds the middle row that has been, until now, neglected
	for i in range(-searchBreadth, searchBreadth+1):
		coordinates.append((i, 0))
	coordinates = coordinates + lowerCoordinates
	# adds the original points to all the created offsets
	for i in range(0, len(coordinates)):
		coordinates[i] = (coordinates[i][0]+xo, coordinates[i][1]+yo)
	return coordinates


# -------------------------------------------------------------------
#                              Main
# -------------------------------------------------------------------

# main function
if __name__ == "__main__":
	trackObj("Dataset/Boat/img/", int(sys.argv[1]))


# -------------------------------------------------------------------
#                      OpenCV Drawing Functions
# -------------------------------------------------------------------

# draws a mask on a the image
def drawMask(Img, mask, pt, maskSize, color=GREEN):
	absPt = (pt[1] + maskSize[1], pt[0] + maskSize[0])
	cv2.rectangle(Img, (pt[1], pt[0]), absPt, color, 2)
	cv2.circle(Img, (pt[1], pt[0]), 2, (0, 0, 255), 5)
	for point in mask:
		# absolute address calculation
		addr = (pt[0] + point[0], pt[1] + point[1])
		Img[addr[0], addr[1]] = color
	return Img

