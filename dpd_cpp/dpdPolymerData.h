#include "bin.h"

class DPDPolymerData : public DPDData {
  public:
    std::string filename;
    unsigned int num_atoms;
    idx density;
    idx box_length;
    unsigned short num_chains;
    idx chain_length;
    float bond_length;
    unsigned int num_Fluid;
    unsigned int num_bonds;
    Bead* FluidList;
    unsigned int FluidCursor;
    unsigned short chainCursor;
    Bond* bondList;
    unsigned int bondCursor;
    unsigned int idTracker;
    DPDPolymerData();
    DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length );
    DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length,
             idx chain_length, unsigned short num_chains );
    virtual ~DPDPolymerData() {}
    void calcNumFluid();
    void calcNumBonds();
    void calcNumAtoms();
    void addBond( Bond* bond );
    void makeNewBond( idx type, Bead* bead1, Bead* bead2 );
    virtual void deriveBondList() {}
    virtual void deriveChainList() {}
    virtual void generate() {}
    virtual void print() {} // does this need to be virtual?
};

class TriblockData : public DPDPolymerData {
  public:
    PECTriblock* chainList;
    idx pec_length;
    idx tail_length;
    void calcNumChains( float* polymer_volume_fraction );
    void calcChainLength();
    TriblockData();
    TriblockData( std::string filename, idx box_length, float bond_length,
                  float polymer_volume_fraction, idx pec_length, idx tail_length );
    ~TriblockData();
    void deriveBondList();
    void deriveChainList();
    void generate();
    void print();
    void addChain( PECTriblock* chain );
};

class ChargeTriblockData : public TriblockData {
    public:
        float charge_density;
};

