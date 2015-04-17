#!/bin/bash

echo 'Starting compilation...'

echo 'Removing old executables...'
rm -rf triblockProcessor
rm -rf triblockBuilder
rm -rf colorTriblock
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


echo '...compilation completed.'
echo 'Exiting...'

