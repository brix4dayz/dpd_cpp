#include "bead.h"
#include <fstream>

class PolymerBlock : public ObjDPD {
	public:
		byte bead_type;
		idx length;
		idx cursor;
		Bead** beadList;
		PosVect* com;
		CopolymerChain* chain;
		PolymerBlock( CopolymerChain* chain, byte type, 
	                          idx length, DirVect* d, 
	                          idx* box_length, PosVect* r, unsigned int* idTracker,
	                          unsigned int mol_id );
		PolymerBlock( CopolymerChain *chain, byte type, idx length );
		PolymerBlock();
		PolymerBlock( CopolymerChain*chain, byte type, idx length,
                  std::ifstream* inFile, idx* box_length, 
                  const float& pbc_correction_factor );
		~PolymerBlock();
		void printBlock( FILE *stream );
		void printData( FILE* stream );
		void calcCenterOfMass( idx* box_length, const float& pbc_correction_factor );
		bool addBead( Bead* bead );
		Bead* getBead( idx index );
    void unlink();
    void color( byte type );
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
			std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor );
		HydrophobicTail();
		bool addBead( Bead* bead );
		~HydrophobicTail();
		HydrophobicCore* getCore();
    void unlink();
};

class ChargedBlock : public PolymerBlock {
	public:
			ChargedBlock( CopolymerChain *chain, byte type, idx length, 
										std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor );
      ChargedBlock( CopolymerChain* chain, byte type,
                    idx length, DirVect* d,
                    idx* box_length, PosVect* r, unsigned int* id,
                    unsigned int mold_id );
      ChargedBlock( CopolymerChain* chain, byte type,
                            idx length, DirVect* d,
                            idx* box_length, PosVect* r, unsigned int* id,
                            unsigned int mol_id, byte& uncharged_type, idx& num_uncharged,
                            IntegerDice<idx>& chargeDice );
      ChargedBlock( CopolymerChain *chain, byte type, idx length );
      ChargedBlock();
};
