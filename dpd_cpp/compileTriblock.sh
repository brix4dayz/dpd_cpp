#!/bin/bash

echo 'Starting compilation...'

echo 'Removing old executables...'
rm -rf triblockProcessor
rm -rf triblockBuilder
rm -rf colorTriblock
rm -rf triblockTime
rm -rf triCoresTime
echo '...executables removed.'

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
echo 'Exiting...'
