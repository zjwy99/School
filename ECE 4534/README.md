# Hide and Seek

## Team Members
Zachary Yee - Mapping Rover

Puvanai Avakul - Hide and Seek Rovers

Steven Frederiksen - Communication between rovers and servers

## Project Description
This project consists of two parts, the mapping phase and the hide and seek phase. The first phase, the mapping phase, has one rover move around a room and gather data using a sensor to generate a map. After the mapping is complete, two rovers will play a game of hide and seek. During this phase, one rover will hide using the map data and the other rover will attempt to find the hiding rover using sensor as well. 

## Project Setup
 1. Clone Repo
 2. Create an MPLABx Harmony Project (set board to MX795..512L)
  1. Use a separate directory from the source
  2. Add dir name to the .gitignore
 3. Point Harmony to use the Main.mhc in the source directory as the base config
 4. In the source directory run `find . -type f ! -name *.mhc -exec ln -sr {} <path-to-mplabx-project-dir>/firmware/src/{} \;`
  - This will make simlinks of your source files and put them where MPLABx can see them
 5. Run the generate code step in Harmony
 6. Add your simlinks (in the \*/firmware/src dir) to the projects as files
 7. Remove the generated \<project-name>.[c|h] files from the project
 8. Start developing
 
 __Note: when you add new files, make sure to add them to the actual source directory and then simlink them into the MPLABx dir
 - `ln -sr <source-dir>/<name> <mplabx-source-dir>/<name>`

## Python Modules needed:
 1. numpy
 2. pymongo
 3. PyQt5
 4. Python3.6

## Stepper Motor Pinout
MAX32 pins, Stepper Signal, PIC32 Pin
	70 -> A -> 17 (RA0)
	71 -> B -> 38 (RA1)
	72 -> C -> 60 (RA4)
	73 -> D -> 61 (RA5)
