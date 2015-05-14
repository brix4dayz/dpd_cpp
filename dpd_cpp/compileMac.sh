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
make triblockProcessor
echo '...triblockProcessor compiled.'

echo 'Compiling triblockBuilder...'
make triblockBuilder
echo '...triblockBuilder compiled.'

echo 'Compiling colorTriblock...'
make colorTriblock
echo '...colorTriblock compiled.'

echo 'Compiling triblockTime...'
make triblockTime
echo '...triblockTime compiled.'

echo 'Compiling triCoresTime...'
make triCoresTime
echo '...triCoresTime compiled.'

echo '...compilation completed.'
echo 'Exiting...'

