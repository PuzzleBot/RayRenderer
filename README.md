# RayRenderer
A simple program which uses ray tracing to display a scene of simple shapes specified by the user.


## System Requirements

Mac OS X - clang and OpenGL installed

Linux - clang and OpenGL installed
(Note that this program was developed for OS X, so it might not work for Linux, though the proper linux header files are included when the program detects it is running on Linux.)

## Compilation
Navigate to the directory this README is located in using the terminal, then type "make" to compile the project as "raytracer.o" inside the bin directory.

## Running the program
The program takes the name of an input file as the only argument. The program can be run using either of these commands, replacing "inputFile.csv" with the path to the input file:

./bin/raytracer.o inputFile.csv

make run FILE=inputFile.csv

## Input file format
The input file should follow a CSV file format. Each line of the input file describes a single object which will be drawn in the scene. The first value of each line should be a string saying what type of object will be placed in the scene ("sphere", "triangle", "light", etc), and the other values on the line describe the position and other details of the shape (all of which are floating-point values). Look at the file "format.csv" under the docs directory for more details.

By default, the view plane is 102.4 units wide by 76.8 units tall, with its center
at the origin and facing the positive z-direction.

Example input files are provided under the "input" directory.


## Controls

Press q to quit.

Press 1 to draw all objects, with lens flares (not implemented yet).

Press 2 to draw only the lens flares (not implemented yet).


### Implemented scene objects

sphere

triangle

light (point light source)


### Implemented lighting effects

Ambient, Diffuse and Specular Lighting

Reflectivity

Transparency (untested)