MDUDUZI NDLOVU - NDLMDU011

Hello

To run and execute this program:
1. Firstly, run "make" which compiles the object files for Tile.cpp and driver.cpp and 
creates driver.exe file
2. Then, run "make run" to run the driver.exe file as:
     make run options="-s 4 -n 15 -i mona.pgm" inputFileImage="mona512.pgm" 

or use driver.exe file

options: 
-s <int> : size of the grid (3,4,5, ...);
-n <int> : number of moves to move the black Tile
-s <String>: name of output images for '-n' moves images

additional options (can be left out and default values will be used):
-m <int> : number of pixels/margin space between the images
-x <String> : name of the Summary Image with file extension 

Finally:
<String> : name of the input image that should be read and Tiled

Sample run: 
    ./driver.exe -s 4 -n 15 -i mona.pgm -m 5 -x Summary_mona.pgm mona512.pgm // 12 arguments
    or
    ./driver.exe -s 4 -n 15 -i mona.pgm mona512.pgm                          // 8 arguments

3. You may call "make clean" to clear the generated binary files, driver.exe and generated PGM images before re-compiling