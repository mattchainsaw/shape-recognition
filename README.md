Shape Recognition
========
Project for recognizing shapes by means of the medial axis. This part of the project deals 
with analysis of the shape. The main functionality is to input a shape and output its medial 
axis information, as well as pruning information that is used to match the shape.

This project has been implemented by Matthew Meyer (B.S. Computer Science 2016) for Dr. Erin 
Chambers of Saint Louis University and funded by the the NSF. 

## Dependencies:
 - [CMake](http://www.cmake.org/)
 - [CGAL](http://www.cgal.org/)
 - [MATLAB](http://www.mathworks.com/products/matlab/) (Optional for viewing the medial 
information)

## Compilation:
After cloning or download this repository and its dependencies, you can compile with CMake.

    $ cd /path/to/dir/
    $ cmake .
    $ make

## Input Format
The input file format is a simple text file that lists the coordinates of each point on the 
boundary. An example file would look like:

    x_0 y_0
    x_1 y_1
    ...
    x_n y_n

Note that `tools/normalize.cpp` can be compiled and used to translate and scale an image into 
a boundary. See the file for more information and compilation instructions.

## Output Format:
The program will output two files. The default names are `out.medial` and `out.path`. 

##### out.medial
In `out.medial` the format consists of the line segments of the medial axis. There will be 6 
columns for each line denoting the two points that make up the line segment, the first points 
radius, and the first points EDF. An example file would look like:

    x_n  y_n  x_m  y_m  radius_n  EDF_n
    ...

##### out.path
In `out.path` the format consists of only line segments of the partial medial axis in the 
pruning path. They are listed in order, starting from the center and the path of the first 
branch, then the start of the next branch to the end of the next branch and so on. An example 
file would look like:

    x_0  y_0  x_1  y_1
    x_1  y_1  x_2  y_2
    ...

## How to use:
There are different options for running the executable.
    
    ./shape-recognition INPUT_FILE
    ./shape-recognition INPUT_FILE OUTPUT_NAME
    ./shape-recognition INPUT_FILE OUTPUT_MEDIAL_INFO OUTPUT_PATH

If only the `INPUT_FILE` argument is given, the program will write to the files `out.medial` 
for medial axis information, and `out.path` for the pruning path information.

If the `INPUT_FILE` and `OUTPUT_NAME` is given, the program will write to the files 
`OUTPUT_NAME.medial` for the medial axis information, and `OUTPUT_NAME.path` for the pruning path 
information.

If you want to name the two output files specifically, use the third option that requires `INPUT_FILE`,
 `OUTPUT_MEDIAL_INFO`, and `OUTPUT_PATH`.

## Project structure
The directory `data/` holds the tested input files. The directory `include/` holds the header files and 
`src/` holds the source files. The `tools/` directory contains programs to supplement the project. 
`normalize.cpp` translates and scales input file for cleaner files. `visualize.m` and `drawCircle.m` are 
MATLAB functions that help show the information visually. `clean.sh` is a bash script that cleans the project 
of unwanted files, such as the CMake output and executable. 