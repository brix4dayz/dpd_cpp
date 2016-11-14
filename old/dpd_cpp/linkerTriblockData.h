#include "dpdPolymerData.h"
#include "linkerPECTriblock.h"

class LinkerTriblockData : public DPDPolymerData {
  public:
    LinkerPECTriblock** chainList;
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
    void addChain( LinkerPECTriblock* chain );
    void printChainList( FILE* fp );
    void unlink();
};