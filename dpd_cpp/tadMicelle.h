#include "dpdTrajectory.h"

/**
 * Chain where tail's consist of only a single bead. A clever idea Tad
 * thought of to simplify the clustering algorithm. The awesome part, is
 * we can reuse this for just about any micelle/copolymer system.
 */
class EndpointChain : public CopolymerChain {
  public:
    byte numTails;
    HydrophobicTail** tails;
    TriblockConfiguration config;
    uintptr_t determineConfiguration();
    virtual void printChain( FILE* stream );
    virtual void unlink();
    void linkTails();
    EndpointChain();
    EndpointChain(byte numTails, std::ifstream& inFile, 
      idx* box_length, const float& pbc_correction_factor);
    ~EndpointChain();

};

class TadMicelle : public Micelle {
  public:
    std::vector< EndpointChain* > chainList;
    TadMicelle();
    TadMicelle( DPDFrame<CopolymerChain>* frame );
    ~TadMicelle();
    void printMicelle( FILE* stream );
    void addChain( EndpointChain* chain );
    void deriveChainList();
    void pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor );
    void calcCenterOfMass( idx* box_length, const float& pbc_correction_factor );
    void unlink();
};

class TadFrame : public CopolymerMicelleFrame {
  public:
    std::vector< TadMicelle* > micelleList;
    std::map< uintptr_t, std::vector<Stem*> > stems;
    float avg_agg_number;
    float percent_stem_chains;
    float percent_petal_chains;
    idx num_cores;
    double avg_distance_btwn_cores;
    TadFrame(unsigned int num_atoms, idx box_length, 
      idx chain_length, idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor);
    TadFrame(unsigned int num_atoms, idx box_length, 
      idx chain_length, idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor, std::ifstream* inFile );
    TadFrame();
    ~TadFrame();
    void process();
    void deriveMicelleList();
    void printChains( FILE* fp );
    void fillBins();
    void printBins( FILE* fp );
    bool areAllFilledBinsGrouped();
    int numFilledBinsArentGrouped();
    void compareBin( Bin* b, HydrophobicCore* core );
    void deriveStems();
    void compareCore( HydrophobicCore* core, TadMicelle* micelle );
    void printData( FILE* fp );
    void calcChainConfigFractions();
    void calcAvgAggNum();
    void calcAvgDistBtwnCores();
    void colorChains();
    void colorCores();
    void correctFrame();
    void printCores( FILE* fp );
};

class TadFrameData : public ObjDPD {
  public:
    float avg_agg_number;
    float percent_stem_chains;
    float percent_petal_chains;
    idx num_cores;
    double avg_distance_btwn_cores;
    TadFrameData( TadFrame* f );
    void unlink() {}
    ~TadFrameData() {}
};

class TadTrajectory : public DPDTrajectory {
  public:
    float AVG_avg_agg_number;
    float STDDEV_avg_agg_number;
    float AVG_percent_stem_chains;
    float STDDEV_percent_stem_chains;
    float AVG_percent_petal_chains;
    float STDDEV_percent_petal_chains;
    float AVG_num_cores;
    float STDDEV_num_cores;
    double AVG_avg_distance_btwn_cores;
    double STDDEV_avg_distance_btwn_cores;
    std::vector< TadFrameData* > frameData;
    idx chain_length;
    float micelle_cutoff;
    float pbc_correction_factor;
    void process();
    void analyzeHelp( std::ifstream& inFile, FILE* fp );
    void setupHelp( FILE* fp );
    void calcHelp();
    TadTrajectory();
    ~TadTrajectory();
};

