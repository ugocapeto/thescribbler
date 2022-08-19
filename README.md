# thescribbler

Written and tested on Ubuntu 22.04.

thescribbler simulates the scribbling effect where one kinda draws circles following a tracing path. This is an implementation of "Tone- and Feature-Aware Circular Scribble Art" by Chun-Chia Chiu, Yi-Hsiang Lo, Ruen-Rone Lee and Hung-Kuo Chu.

To create thescribbler:

compile the code in directory "thescribbler" using "make -f Makefile_g/Makefile_O"

compile the code in directory "thescribbler/main" using "make"

Test cases are given in the "test" directory.

What you would typically do is to run thestippler on your input photograph (or image). This creates a file called "sample.tsp" that contains the stipples. Then, you execute "./linkern.sh". That creates a file called "sample.cyc". Then, you run thescribbler to get your rendered image (should be "output_image.png" unless you have changed the input file "thescribbler_input.txt").

Info about thescribbler (theory behind it and how to use it) can be found here:

[Painting Software - The Scribbler](https://3dstereophoto.blogspot.com/2019/04/painting-software-scribbler.html)

Dependencies (check the Makefiles):

"common" repo
