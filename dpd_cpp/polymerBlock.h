#include "bead.h"
#include <fstream>

class PolymerBlock : public ObjDPD {
	public:
		idx bead_type;
		idx length;
		idx cursor;
		Bead** beadList;
		PosVect* com;
		CopolymerChain* chain;
		PolymerBlock( CopolymerChain* chain, idx type, 
	                idx length, DirVect* d, 
	                idx* box_length, PosVect* r, unsigned int* id, 
	                unsigned int mold_id );
		PolymerBlock( CopolymerChain *chain, idx type, idx length );
		PolymerBlock();
		PolymerBlock( CopolymerChain *chain, idx type, idx length, 
			std::ifstream* inFile, idx* box_length );
		~PolymerBlock();
		void printBlock( FILE *stream );
		void printData( FILE* stream );
		void calcCenterOfMass( idx* box_length );
		bool addBead( Bead* bead );
		Bead* getBead( idx idx );
    void unlink();
};

class HydrophobicTail : public PolymerBlock {
	public:
		HydrophobicTail* other;
		Bin* bin;
		HydrophobicTail( CopolymerChain* chain, idx length,
		                 DirVect* d, idx* box_length, 
		                 PosVect* r, unsigned int* idTracker,
		                 unsigned int mold_id );
		HydrophobicTail( CopolymerChain* chain, idx length );
		HydrophobicTail( CopolymerChain* chain, idx length, 
			std::ifstream* inFile, idx* box_length );
		HydrophobicTail();
		bool addBead( Bead* bead );
		~HydrophobicTail();
		HydrophobicCore* getCore();
    void unlink();
};

class ChargedBlock : public PolymerBlock {
	public:
			ChargedBlock( CopolymerChain *chain, idx type, idx length, 
										std::ifstream* inFile, idx* box_length );
      ChargedBlock( CopolymerChain* chain, idx type,
                    idx length, DirVect* d,
                    idx* box_length, PosVect* r, unsigned int* id,
                    unsigned int mold_id );
      ChargedBlock( CopolymerChain *chain, idx type, idx length );
      ChargedBlock();
};
