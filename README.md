# fbutils
Framebuffer drawing utilities made in C.

### Commands for compiling the library
- Compile to object with PIC `gcc -c -fPIC -O2 fbuffer.c pixel.c rectangle.c circle.c line.c`
- Combine the object to a library `gcc -shared -o libfbutils.so fbuffer.o pixel.o rectangle.o circle.o line.o`

### Commands for compiling and running test programs
- Compile a test program `gcc -I/absolute_path_to_fbutils/src -L/absolute_path_to_fbutils/src -l:libfbutils.so -o main main.c`
- Run the program with an environment variable to find the linked library `sudo LD_LIBRARY_PATH=/absolute_path_to_sourcefolder/fbutils/src ./main`

_**Notice**: Programs require superuser permissions by default_


Created and maintained by Niklas Halonen
