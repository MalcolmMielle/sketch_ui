# Specs :

## Workflow :

1. Run sketch_ui.exe
2. Let the user draw a sketch map.
3. Click on a "Done" button to export the result. Exported files are defined in the Output section.
4. Send those files to a specific path to the robot. Everything could be stored in a `.smokebot/commander` folder. Then the algorithm would fetch those images and files.
5. The robot might transfer some images back to the Commander (the robot's internal map with sketch elements on it for example). But it would be better to do this later when we know what will the GDIM contain and how to visualize it.

## Outputs :

* Sketch image drawn by the user -> PNG image : obstacle.png
* Path image drawn by the user -> PNG image : path.png
* A list of pair of points defining keypoint positions in the obstacle map. -> Text file : keypoints.txt. Every line contains 4 numbers (x-first-keypoint y-first-keypoint x-second-keypoint y-second-keypoint) separated by spaces. In between every keypoints there is a new line.

## What is expected of Taurob :

* Add UI elements so that sketch_ui.exe can be launched within the commander.
* Make sure that the output files are transferred to the robot when the 'Done' button is clicked.

## What is expected of AASS :

* Separate the algorithm and interface to allow for easy integration into Windows.
* Make a simpler version of the interface for an easy use.
