#include <iostream>
#include <string>
#include <cstdio>
#include <fstream> 
#include <cstdlib>
#include <ctime>

class DPDTrajectory {
  public:
    std::string* fileNames;
    unsigned int numFiles;
    unsigned int num_atoms;
    //idx box_length;
    //idx bin_length;
    unsigned int numFrames;
    unsigned int startFile;
    unsigned int startFileOffset;
    unsigned long numBytesInFile( std::string filename );
    void determineNumFrames();
    DPDTrajectory();
    ~DPDTrajectory();
};