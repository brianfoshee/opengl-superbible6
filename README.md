These examples are all taken from the OpenGL SuperBible, sixth edition.
I'm learning OpenGL so as I go I'll be working on these here. If you find mistakes or ways to
improve what I have, create an issue or a pull request.
I am re-writing everything in C (with the help of GLFW) because the official examples are in C++ and 
use a big framework the Authors have written on top of GLFW. The book is written for OpenGL version 4.4,
which is the latest and greatest. My daily machine is a Mac, and Apple is always behind the
game with OpenGL so I have to use 4.1. The authors of the book have examples that somewhat work
on a Mac, but they didn't go through every single one and get them working (understandably). Hopefully I can
make everything work except for where features not available in 4.1 are used.

To build any one example (one day I'll use a makefile to build them all):

```
gcc chapter2a.c -o chapter2a.out `pkg-config --cflags glfw3` `pkg-config --libs glfw3` -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
```
