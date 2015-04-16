#!/bin/bash

echo 'Starting compilation...'

echo 'Removing old executables...'
rm -rf triblockProcessor
rm -rf triblockBuilder
echo '...executables removed.'

echo 'Compiling triblockProcessor...'
make triblockProcessor
echo '...triblockProcessor compiled.'

echo 'Compiling triblockBuilder...'
make triblockBuilder
echo '...triblockBuilder compiled.'

echo '...compilation completed.'
echo 'Exiting...'

