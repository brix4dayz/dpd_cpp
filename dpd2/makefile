CC=g++
STDFLAG=-std=c++11
ifdef LINUX
STDFLAG=-std=gnu++11
ifndef HPC
CC=g++-4.8
endif
endif

CFLAGS=-Wall -g -fPIC -O3

SRCS = dpd2.cpp object.cpp linalg.cpp geom.cpp cluster.cpp binbox.cpp

OBJS =  $(SRCS:.cpp=.o)

MAIN = ./lib/libdpd2.so

all: $(MAIN)
	@echo libdpd2.so has been compiled
	
$(MAIN): $(OBJS)
	$(CC) $(STDFLAG) $(CFLAGS) -shared $^ -o $@

.cpp.o:
	$(CC) $(STDFLAG) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)