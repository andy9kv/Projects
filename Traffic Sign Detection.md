
# Traffic Sign Detection

Sign detection, recognition, and identification are imperative for many autonomous robotic applications such as self-driving cars. To solve this problem, in many cases, learning-based approaches are favored where there are no explicit, human-given instructions rather machine-derived solutions. This approach is different in that it relies exclusivly on computer vision techniques to detect (not identify) traffic signs in a video. Initially, this was a team project where each member presented different solutions to the problem (associated [white paper](/Project_Files/Traffic_Sign_Detection/whitepaper.pdf)). The following is my solution.

## Architecture



## Implementation

The program is about 400 lines of optimized python code. I put great efforts into maximizing the speed and efficiency of the code including using lambda function where appropriate and relying on NumPy subroutine alternatives when possible. Runtime is no more then 3-4 times slower than normal video playback speed (e.g. a video's frame rate reduces from 30fps to 10fps) if real-time computation is done. The well-commented code can be found [here](/Project_Files/Traffic_Sign_Detection/sign_detection.py).

Libraries Used:
* [OpenCV 3.3](https://opencv.org/opencv-3-3.html) - Image management and computer vision algorithm implementations
* [NumPy](https://www.numpy.org/) - Powerful and efficient n-dimensional array implementation (and much more!)

## Demos

## Run it yourself

