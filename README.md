# fbutils
Framebuffer drawing utilities made in C.
Only tested on Raspberry Pi 3B.

### Commands for compiling the library
- Compile to object with PIC `gcc -c -fPIC -O2 fbuffer.c pixel.c rectangle.c circle.c line.c`
- Combine the object to a library `gcc -shared -o libfbutils.so fbuffer.o pixel.o rectangle.o circle.o line.o`

### Commands for compiling and running test programs
- Compile a test program
	`gcc -I /absolute_path_to_fbutils/src -L /absolute_path_to_fbutils/src -l:libfbutils.so -o main main.c -lm`
- Run the program with an environment variable to find the linked library
	`sudo LD_LIBRARY_PATH=/absolute_path_to_sourcefolder/fbutils/src ./main`

_**Notice**: Programs require superuser permissions by default_

### Tests
For example (on Raspberry Pi) compile the tests with
`gcc -I /home/pi/fbutils/src -L /home/pi/fbutils/src -l:libfbutils.so -o main main.c -lm`
List of current test programs and their functions:
- **openclose**
	function: open framebuffer and print its dimensions, closes right after
- **pixel**
	function: draw 4 cyan pixels in each corner for 4 seconds
- **rectangles**
	function: draw random unfilled and filled rectangles, listen for signals to stop the program
- **circles**
	ridiculously inefficient
	function: draw a huge circle with a brim
- **lines**
	function: draw lots of random lines
- **ts**
	list of touchscreen example programs to implement fbutils
	touchsceen library: [tslib](https://github.com/kergoth/tslib/)
	- **button**
		threaded drawing
		function: draw a button that can be pressed
	- **tsbug**
		nothing to do with framebuffers
		a debug tool for ts_read_mt
	- **touches**
		function: show multitouch locations

## TODO
- makefile / cmake
- graphics support (cairo?)
- opengl
- website
- python


Created and maintained by Niklas Halonen
