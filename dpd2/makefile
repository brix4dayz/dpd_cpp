ifdef LINUX
STDFLAG=-std=gnu++11
ifdef HPC
CC=g++
else
CC=g++-4.8
endif
else
STDFLAG=-std=c++11
CC=g++
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