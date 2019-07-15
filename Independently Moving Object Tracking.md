
# Independently-Moving Object Tracker

&nbsp;&nbsp;&nbsp;&nbsp;Object detection and tracking are very important for autonomous robots. Self-navigating vehicles, drones, and several other automatons require object tracking to function. This may be a fairly easy problem if a proven machine learning model were implemented to detect and track an object throughout a video, however I attempted a somewhat different approach. My solution uses feature correspondence to identify a particular object in the first frames of the video for detection, followed by feature correspondence with an adaptive mask to track the changing object throught the sequence. The whitepaper I wrote to present and thoroughly describe my work can be found [here](/Project_Files/Object_Tracking/Independently-Moving%20Object%20Detection.pdf).

Written in Python, using OpenCV and NumPy, in November 2018.

## Architecture

<br><p align="center"><img width="70%" height="70%" src="Project_Files/Object_Tracking/media/obj_algo.png"></p><br>

&nbsp;&nbsp;&nbsp;&nbsp;The first step is to find the desired object in the video. This is done by observing features from the same object in other images and searching for those features in the beginning of the video. Thus, the algorithm begins by extracting key points or features from some input images (other images containing the target object). Using [SIFT](Project_Files/Object_Tracking/SIFT.pdf) and the [Harris Corner Detector] (Project_Files/Object_Tracking/HCD.pdf), the program creates a list of features from the given images. After obtaining a satisfactory list of key features, the first few frames of the video are scanned to correspond the key points between the input images and the initial video frames. If a sufficient correspondence is found around position X, we conclude the desired object has been found at that location. An example of feature correspondence is pictured below,  

<br><p align="center"> <img width="60%" height="60%" src="Project_Files/Object_Tracking/media/bike.jpg" alt> </p><br>

&nbsp;&nbsp;&nbsp;&nbsp;Once the object is detected in the first frame, the algorithm tracks the object throughout the video by noting the slight linear transformations in each frame. In other words, the algorithm must keep track of the object's change in position, rotation, and scale as the video progresses with the key observation that these changes will be minor. This is handled by an adaptive mask that adopts a mold for each new face of the object. The algorithm is thus,
	
  For each frame in the video	
1. 
2. 
3. 
4. 
5. 
6. 

## Implementation

&nbsp;&nbsp;&nbsp;&nbsp;The program is about 400 lines of optimized python code. I put great efforts into maximizing the speed and efficiency of the code including using lambda function where appropriate and relying on NumPy subroutine alternatives when possible. Runtime is no more then 3-4 times slower than normal video playback speed (e.g. a video's frame rate reduces from 30fps to 10fps) if real-time computation is done. The well-commented code can be found [here](/Project_Files/Traffic_Sign_Detection/sign_detection.py).

Libraries Used:
* [OpenCV 3.3](https://opencv.org/opencv-3-3.html) - Image management and computer vision algorithm implementations
* [NumPy](https://www.numpy.org/) - Powerful and efficient n-dimensional array implementation (and much more!)

&nbsp;&nbsp;&nbsp;&nbsp;It is imperative to isolate edges from non-edges in the image before scanning for possible sign shapes or any application of the polygon approximation function. Opencv 3.3 offers an implementation of the [Canny edge detector](https://docs.opencv.org/3.1.0/da/d22/tutorial_py_canny.html), however it does not offer a sufficient degree of precision in the detected edges. As shown below, the canny edge detector isolates only the most significant edges whereas my implementation of the [sobel filter](https://en.wikipedia.org/wiki/Sobel_operator) produces an edge map with much more detail, highlighting the edges based on their significance. Thus, the canny operator returns a binary image where pixels can be either white or black (1 or 0) while the sobel operator returns a grayscale image. A sample of both edge operators applied to a recording of a drive in NYC is shown below (top left: canny, top right: original stream, bottom: sobel), 

<p float="center"> 
	<img width="48%" height="100%" src="Project_Files/Traffic_Sign_Detection/media/canny.gif">
	<img width="48%" height="100%" src="Project_Files/Traffic_Sign_Detection/media/original.gif">
</p>
<p float="center"> <img width="96%" height="100%" src="Project_Files/Traffic_Sign_Detection/media/sobel.gif"> </p>

## Demos

The following is a highlight reel of the final version of this software applied to several videos from the Berkley DeepDrive dataset,

<br><p align="center"><img width="100%" height="100%" src="Project_Files/Traffic_Sign_Detection/media/demo.gif"></p><br>

&nbsp;&nbsp;&nbsp;&nbsp;Although imperfect, this implementation detects traffic signs with a high level of accuracy. The miss rate (missing a sign altogether) is quite low but the rate of false positives is more common then expected. Machine learning models, though costly to build, are much quicker and computationally efficient with near-perfect results. 

