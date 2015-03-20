#include "polymerBlock.h"

class CopolymerChain : public ObjDPD {
	public:
		idx chain_length;
		PosVect* com;
		Micelle* micelle;
    unsigned short id;
    virtual void printChain( FILE* stream ) {}
    virtual void printData( FILE* stream ) {}
		~CopolymerChain() {
      delete this->com;
      this->com = NULL;
      this->unlink();
    }
    void unlink() {
      this->micelle = NULL;
    }
};

enum TriblockConfiguration { stem, petal, neither };

class PECTriblock : public CopolymerChain {
	public:
		CopolymerMicelleFrame<PECTriblock>* frame;
    PolymerBlock* pec_block;
		HydrophobicTail* tail1;
		HydrophobicTail* tail2;
		TriblockConfiguration config;
		idx tail_length;
		idx pec_length;
		// Should I add a void ptr to the frame as a parameter?
		PECTriblock();
		PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length,
                       unsigned int* idTracker, unsigned short id);
		PECTriblock( idx tail_length, idx pec_length, idx length );
		PECTriblock( idx tail_length, idx pec_length, idx length, std::ifstream* inFile, idx* box_length );
		~PECTriblock();
    void unlink();
		void printChain( FILE* stream );
		void printData( FILE* stream );
		void determineConfiguration();
};