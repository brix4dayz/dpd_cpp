#ifndef Bin

#define BIN 1

#include "copolymerChain.h"
#include <vector>

class Bin : public ObjDPD {
	public:
		idx size;
		idx i, j, k;
		idx num_chains;
		CopolymerMicelleFrame* frame;
		std::vector< HydrophobicTail* > tailList; 
		// when using iterators must deference the auto to use it like a pointer
		HydrophobicCore* core;
		bool checked;
		bool grouped;
		Bin();
		~Bin();
		void init( CopolymerMicelleFrame* frame, idx size, idx i, idx j,
			idx k );
		bool groupBins( Bin* other, idx* box_length, 
			float* micelle_cutoff  );
		void addTail( HydrophobicTail* tail );
		void printBin( FILE* stream );
		void unlink();
		bool isEmpty();
};

#endif