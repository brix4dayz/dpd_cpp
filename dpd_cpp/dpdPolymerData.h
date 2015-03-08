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
    unsigned int molIDTracker;
    idx Fluid_type;
    DPDPolymerData();
    DPDPolymerData( std::string filename, idx density, 
                    idx box_length, float bond_length, idx Fluid_type );
    DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length,
                    idx chain_length, unsigned short num_chains, idx Fluid_type );
    virtual ~DPDPolymerData() {}
    void calcNumFluid();
    void calcNumBonds();
    void calcNumAtoms();
    void addBond( Bond* bond );
    void makeNewBond( idx type, Bead* bead1, Bead* bead2 );
    bool addFluid( Bead* fluid );
    void deriveFluidList();
    virtual void deriveBondList() {}
    virtual void deriveChainList() {}
    virtual void generate() {}
    virtual void printLAMMPS( FILE* fp ) {} // does this need to be virtual?
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
    void printLAMMPS( FILE* fp );
    void addChain( PECTriblock* chain );
};

class ChargeTriblockData : public TriblockData {
    public:
        float charge_density;
};

