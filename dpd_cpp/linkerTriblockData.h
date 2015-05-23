#include "dpdPolymerData.h"
#include "LinkerPECTriblock.h"

class LinkerTriblockData : public DPDPolymerData {
  public:
    LinkerPECTriblock* chainList;
    idx pec_length;
    idx tail_length;
    idx link_length;
    float charge_density;
    void calcChainLength();
    LinkerTriblockData();
    LinkerTriblockData( std::string filename, idx box_length, float bond_length,
                  float polymer_volume_fraction, idx pec_length, idx tail_length, idx link_length,
                  float charge_density );
    ~LinkerTriblockData();
    void deriveBondList();
    void deriveChainList();
    void generate();
    void printLAMMPS( FILE* fp );
    void addChain( LinkerPECTriblock* chain );
};