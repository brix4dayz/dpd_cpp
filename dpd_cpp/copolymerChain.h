#ifndef COPOLYMER

#define COPOLYMER 1

#include "polymerBlock.h"
#include <cstdint>

class CopolymerChain : public ObjDPD {
	public:
		idx chain_length;
		PosVect* com;
		Micelle* micelle;
    idx id;
    virtual void printChain( FILE* stream ) {}
    virtual void printData( FILE* stream ) {}
    virtual void colorChain( byte type ) {}
		virtual ~CopolymerChain() {
      delete this->com;
      this->com = NULL;
      this->unlink();
    }
    virtual void unlink() {
      this->micelle = NULL;
    }
};

enum TriblockConfiguration { stem, petal, neither };

class SymmetricAmphiphilicTriblock : public CopolymerChain {
  public:
    HydrophobicTail* tail1;
    HydrophobicTail* tail2;
    TriblockConfiguration config;
    idx tail_length;
    idx pec_length;
    void unlink();
    uintptr_t determineConfiguration();
    SymmetricAmphiphilicTriblock();
    SymmetricAmphiphilicTriblock( idx pec_length, idx tail_length, idx length );
    ~SymmetricAmphiphilicTriblock();
    void linkTails();
    void colorTails( byte type );
};

class PECTriblock : public SymmetricAmphiphilicTriblock {
	public:
    ChargedBlock* pec_block;
		PECTriblock();
		PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length,
                       unsigned int* idTracker, unsigned short id);
		PECTriblock( idx pec_length, idx tail_length, idx length );
		PECTriblock( idx pec_length, idx tail_length, idx length, std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor );
		~PECTriblock();
    void unlink();
		void printChain( FILE* stream );
		void printData( FILE* stream );
		void colorChain( byte type );
    PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length,
                 unsigned int* idTracker, unsigned short id, byte& uncharged_type, idx& num_uncharged,
                 IntegerDice<idx>& chargeDice );
};

// XOR ptrs source: http://stackoverflow.com/questions/3531972/c-code-for-xor-linked-list
class Stem : public ObjDPD {
	public:
		unsigned short count;
		HydrophobicCore* core1;
		HydrophobicCore* core2;
		bool grouped;
		Stem( HydrophobicCore* c1, HydrophobicCore* c2 );
		void unlink();
		~Stem();
		void inc();
		uintptr_t getKey();
		static uintptr_t hashCores( HydrophobicCore* c1, HydrophobicCore* c2 );
};

#endif