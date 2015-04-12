#include "copolymerMicelleFrame.h"

class DPDTrajectory : public ObjDPD {
  public:
    std::string* fileNames;
    unsigned int numFiles;
    unsigned int num_atoms;
    idx box_length;
    idx bin_length;
    unsigned int numFrames;
    unsigned int startFile;
    unsigned int startFileOffset;
    unsigned long numBytesInFile( std::string filename );
    void determineNumFrames();
    DPDTrajectory();
    ~DPDTrajectory();
    void unlink();
};

class TriblockTrajectory : public DPDTrajectory {
  public:
    idx pec_length;
    idx tail_length;
    idx chain_length;
    float micelle_cutoff;
};
