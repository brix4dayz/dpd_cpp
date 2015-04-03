#include "micelle.h"
#include <map>

//
class CopolymerMicelleFrame : public DPDFrame<CopolymerChain> {
	private:
		idx coordToBin( double coord );
	public:
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
			idx chain_length, idx bin_size, float* micelle_cutoff );
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
    std::map< uintptr_t, Stem* > stems;
    float avg_agg_number;
    float percent_neither_chains;
		float percent_stem_chains;
		float percent_petal_chains;
		idx num_cores;
		double rms_distance_btwn_cores;
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, float* micelle_cutoff, idx tail_length, idx pec_length );
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, float* micelle_cutoff, idx tail_length, idx pec_length, std::ifstream* inFile );
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
		void calcRMSDistBtwnCores();
};