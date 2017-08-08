# Sketch UI

This is to be used to test the [sketch map matching](https://github.com/MalcolmMielle/sketch_map_matching) algorithm.

The Sketch UI is a user interface for drawing sketch maps while the sketch maker is a server that receive the map and interprets it using the algorithm from before.

# How to use

In a nutshell:

* compile both package in a folder named release
* from release folder of each package launch `expert_ui` and `main`

Now you can draw in the UI and the result of the matching with the model will be exported to the folde rnamed `results`.
To change the model, just change the image in the model folder.

There is more detailled READMEs in each package.