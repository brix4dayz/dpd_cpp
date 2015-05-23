#ifndef LINKTRIBLOCK
#define LINKTRIBLOCK 1

#include "copolymerChain.h"
#include <map>

class LinkerPECTriblock : public SymmetricAmphiphilicTriblock {
  public:
    ChargedBlock* pec1;
    ChargedBlock* pec2;
    idx stiff_link_length;
    PolymerBlock* stiffLink;
    LinkerTriblock();
    LinkerTriblock( idx pec_length, idx tail_length, idx length, std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor );
    ~LinkerTriblock();
    void unlink();
    void printChain( FILE* stream );
    void printData( FILE* stream );
    void colorChain( byte type );
    LinkerTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length,
                    unsigned int* idTracker, unsigned short id, byte& uncharged_type, idx& num_uncharged,
                    IntegerDice<idx>& chargeDice );
};

class CoarseDNABlock : public ObjDPD {
  public:
    ChargedBlock* pec1;
    ChargedBlock* pec2;
    idx bond_start_index;
    idx pec_length;
    idx hybridized_length;

};

class HybridizedSSDDNATriblock : public SymmetricAmphiphilicTriblock {

};

#endif