#!/bin/bash

echo 'Starting compilation...'

echo 'Removing old executables...'
rm -rf ./bin/triblockProcessor
rm -rf ./bin/triblockBuilder
rm -rf ./bin/colorTriblock
rm -rf ./bin/triblockTime
rm -rf ./bin/triCoresTime
rm -rf ./lib/libdpd.so
echo '...executables removed.'

echo 'Compiling libdpd.so...'
make libdpd.so LINUX=1 HPC=1
mv -iv libdpd.so ./lib/
echo '...libdpd.so compiled.'

echo 'Compiling triblockProcessor...'
make triblockProcessor LINUX=1 HPC=1
echo '...triblockProcessor compiled.'

echo 'Compiling triblockBuilder...'
make triblockBuilder LINUX=1 HPC=1
echo '...triblockBuilder compiled.'

echo 'Compiling colorTriblock...'
make colorTriblock LINUX=1 HPC=1
echo '...colorTriblock compiled.'

echo 'Compiling triblockTime...'
make triblockTime LINUX=1 HPC=1
echo '...triblockTime compiled.'

echo 'Compiling triCoresTime...'
make triCoresTime LINUX=1 HPC=1
echo '...triCoresTime compiled.'

echo '...compilation completed.'
rm -rf *.o
mv -iv triblockProcessor ./bin/ 
mv -iv triblockBuilder ./bin/
mv -iv colorTriblock ./bin/
mv -iv triblockTime ./bin/
mv -iv triCoresTime ./bin/
./setup_dpd_cpp_path.sh
echo 'Exiting...'
