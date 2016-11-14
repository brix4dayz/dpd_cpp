
ifdef LINUX
STDFLAG=-std=gnu++11
CC=g++-4.8
else
STDFLAG=-std=c++11
CC=g++
endif

WARN=-Wall
DEBUG=-g

makeLibVect:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c -DPYTHON_LIB -fPIC vect.cpp -o vect.o
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -shared -Wl,-soname,libvect.so -o libvect.so  vect.o