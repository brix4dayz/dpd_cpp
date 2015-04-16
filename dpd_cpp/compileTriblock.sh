#!/bin/bash

echo 'Starting compilation...'

echo 'Removing old executables...'
rm -rf triblockProcessor
rm -rf triblockBuilder
echo '... executables removed.'

echo 'Compiling triblockProcessor...'
make triblockProcessor LINUX=1 HPC=1
echo '... triblockProcessor compiled.'

echo 'Compiling triblockBuilder...'
make triblockBuilder LINUX=1 HPC=1
echo '... triblockBuilder compiled.'

echo '... compilation completed.'
echo 'Exiting...'

