
# Traffic Sign Detection

Sign detection, recognition, and identification are imperative for many autonomous robotic applications such as self-driving cars. To solve this problem, in many cases, learning-based approaches are favored where there are no explicit, human-given instructions rather machine-derived solutions. This approach is different in that it relies exclusivly on computer vision techniques to detect (not identify) traffic signs in a video. Initially, this was a team project where each member presented different solutions to the problem (associated [white paper](/Project_Files/Traffic_Sign_Detection/whitepaper.pdf)). The following is my solution.

## Architecture

<br><p align="center"><img width="70%" height="70%" src="Project_Files/Traffic_Sign_Detection/media/traffic_algo.png"></p><br>

The graphic above shows the gist of the algorithm. A more specific description is here below,
	
  For each frame in the video	
1. Color Threshold using HSV equivalents of each frame to isolate sign colors only
2. Produce an edge map of the thresholded image using sobel or canny
3. Find contours in the binary edge map
4. Refine the contours by their position and area (i.e eliminate contours around the edges of the image or contours that define an area too small for consideration)
5. Classify all remaning contours into shapes (Triangle, Square, Octagon, etc) by approximating the curve of the contours using the Ramer–Douglas–Peucker algorithm. [opencv provides an implementation of this](https://docs.opencv.org/3.1.0/dd/d49/tutorial_py_contour_features.html)
6. Draw a bounding box around the resulting shapes that are likely to be signs

## Implementation

The program is about 400 lines of optimized python code. I put great efforts into maximizing the speed and efficiency of the code including using lambda function where appropriate and relying on NumPy subroutine alternatives when possible. Runtime is no more then 3-4 times slower than normal video playback speed (e.g. a video's frame rate reduces from 30fps to 10fps) if real-time computation is done. The well-commented code can be found [here](/Project_Files/Traffic_Sign_Detection/sign_detection.py).

Libraries Used:
* [OpenCV 3.3](https://opencv.org/opencv-3-3.html) - Image management and computer vision algorithm implementations
* [NumPy](https://www.numpy.org/) - Powerful and efficient n-dimensional array implementation (and much more!)

## Demos

<br><p align="center"><img width="100%" height="100%" src="Project_Files/Traffic_Sign_Detection/media/demo.gif"></p><br>

## Run it yourself

