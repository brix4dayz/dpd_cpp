CC=g++
STDFLAG=-std=c++11
LFLAG=-ldpd2
LIBFLAG=-L/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/lib/
INCLUDES=-I/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/
ifdef LINUX
STDFLAG=-std=gnu++11
ifndef HPC
CC=g++-4.8
endif
endif

CFLAGS=-Wall -g -O3

SRCS = testcluster.cpp testdpd2.cpp
OBJS =  $(SRCS:.cpp=.o)

MAIN = ../bin/testdpd2

all: $(MAIN)
	@echo testdpd2 has been compiled
	
$(MAIN): $(OBJS)
	$(CC) $(STDFLAG) $(CFLAGS) $(INCLUDES) $(LFLAG) $(LIBFLAG) $^ -o $@

.cpp.o:
	$(CC) $(STDFLAG) $(CFLAGS) $(INCLUDES) $(LFLAG) $(LIBFLAG) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
