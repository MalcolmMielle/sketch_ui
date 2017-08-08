# Sketch UI

An simple interface for sketching maps.

Use the program `mapmakerv3`

Using Qt4, a user can either :

* Draw obstacles 
* Draw a path for the robot
* Add doors into the environment.

To export the result simply press the export button. Results will be exported as :

* Sketch image drawn by the user -> PNG image : obstacle.png
* Path image drawn by the user -> PNG image : path.png
* A list of pair of points defining keypoint positions in the obstacle map. -> Text file : keypoints.txt. Every line contains 4 numbers (x-first-keypoint y-first-keypoint x-second-keypoint y-second-keypoint) separated by spaces. In between every keypoints there is a new line.

# Install dependencies :

	sudo apt-get install qt4-dev-tools libqt4-dev libqt4-core libqt4-gui 

