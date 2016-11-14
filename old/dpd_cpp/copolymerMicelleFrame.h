#include "micelle.h"
#include <map>

#define STEM 1
#define PETAL 2

//
class CopolymerMicelleFrame : public DPDFrame<CopolymerChain> {
	private:
		idx coordToBin( double coord );
	public:
    const float& pbc_correction_factor;
		unsigned int num_atoms;
		idx box_length;
		idx chain_length;
		idx bin_size;
		idx num_bins;
		unsigned short num_chains;
		CopolymerChain** chainList;
		unsigned short chainCursor;
		float* micelle_cutoff;
		Bin**** box; //3d array of bin pointers making up simulation box
		CopolymerMicelleFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor );
		CopolymerMicelleFrame();
		~CopolymerMicelleFrame();
		//Bin binBlock( PolymerBlock* block );
		Bin* binBlock( HydrophobicTail* tail );
  	void addChain( CopolymerChain* chain );
		virtual void process() {}
    void unlink();
};

class TriblockFrame : public CopolymerMicelleFrame {
	public:
		std::vector< TriblockMicelle* > micelleList;
    idx tail_length;
    idx pec_length;
    std::map< uintptr_t, std::vector< Stem* > > stems;
    float avg_agg_number;
		float percent_stem_chains;
		float percent_petal_chains;
		idx num_cores;
		double avg_distance_btwn_cores;
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor, idx tail_length, idx pec_length );
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor, idx tail_length, idx pec_length, std::ifstream* inFile );
		TriblockFrame();
		~TriblockFrame();
		void process();
		void deriveMicelleList();
		void printChains( FILE* fp );
		void fillBins();
		void printBins( FILE* fp );
		bool areAllFilledBinsGrouped();
		int numFilledBinsArentGrouped();
		void compareBin( Bin* b, HydrophobicCore* core );
		void deriveStems();
		void compareCore( HydrophobicCore* core, TriblockMicelle* micelle );
		void printData( FILE* fp );
		void calcChainConfigFractions();
		void calcAvgAggNum();
		void calcAvgDistBtwnCores();
    void colorChains();
    void colorCores();
    void correctFrame();
    void printCores( FILE* fp );
};

class TriblockFrameData : public ObjDPD {
  public:
    float avg_agg_number;
    float percent_stem_chains;
    float percent_petal_chains;
    idx num_cores;
    double avg_distance_btwn_cores;
    TriblockFrameData( TriblockFrame* f );
    void unlink() {}
    ~TriblockFrameData() {}
};
