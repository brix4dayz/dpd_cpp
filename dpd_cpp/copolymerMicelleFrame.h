#include "micelle.h"

// 
class CopolymerMicelleFrame : public DPDFrame {
	private:
		idx coordToBin( double coord );
	public:
		unsigned int num_atoms;
		idx box_length;
		idx chain_length;
		idx bin_size;
		idx num_bins;
		unsigned short num_chains;
		CopolymerChain* chainList;
		Bin*** box; //3d array of bins making up simulation box
		CopolymerMicelleFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size );
		CopolymerMicelleFrame();
		~CopolymerMicelleFrame();
		//Bin binBlock( PolymerBlock* block );
		Bin binBlock( HydrophobicTail* tail );
		virtual void process() {}
    void unlink() {}
};

class TriblockFrame : public CopolymerMicelleFrame {
	public:
		std::vector< TriblockMicelle* > micelleList;
    idx tail_length;
    idx pec_length;
		unsigned short avg_agg_num_of_cores;
		float percent_stem_chains;
		float percent_petal_chains;
		idx num_cores;
		double avg_distance_btwn_cores;
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, idx tail_length, idx pec_length );
		TriblockFrame( unsigned int num_atoms, idx box_length, 
			idx chain_length, idx bin_size, idx tail_length, idx pec_length, std::ifstream* inFile );
		TriblockFrame();
		~TriblockFrame();
		void process();
  void unlink() {}
};