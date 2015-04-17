#include "copolymerMicelleFrame.h"

class DPDTrajectory : public ObjDPD {
  public:
    std::string* fileNames;
    std::string outFile;
    unsigned int numFiles;
    unsigned int num_atoms;
    idx box_length;
    idx bin_length;
    unsigned int numFrames;
    unsigned int startFile;
    unsigned int startFrameOffset;
    unsigned int framesAnalyzed;
    unsigned long numBytesInFile( std::string filename );
    void determineNumFrames();
    DPDTrajectory();
    ~DPDTrajectory();
    void unlink();
    void process();
    void skip( std::ifstream& inFile );
    void analyze( std::ifstream& inFile, FILE* fp );
    virtual void analyzeHelp( std::ifstream& inFile, FILE* fp ) { skip( inFile ); }
    void setupOutputFile( FILE* fp );
    virtual void setupHelp( FILE* fp ) {}
    void calcData();
    virtual void calcHelp() {}
};

class TriblockTrajectory : public DPDTrajectory {
  public:
    float AVG_avg_agg_number;
    float STDDEV_avg_agg_number;
    float AVG_percent_stem_chains;
    float STDDEV_percent_stem_chains;
    float AVG_percent_petal_chains;
    float STDDEV_percent_petal_chains;
    float AVG_num_cores;
    float STDDEV_num_cores;
    double AVG_rms_distance_btwn_cores;
    double STDDEV_rms_distance_btwn_cores;
    std::vector< TriblockFrameData* > frameData;
    idx pec_length;
    idx tail_length;
    idx chain_length;
    float micelle_cutoff;
    void analyzeHelp( std::ifstream& inFile, FILE* fp );
    void setupHelp( FILE* fp );
    void calcHelp();
    TriblockTrajectory();
    ~TriblockTrajectory();
};
