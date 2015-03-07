#include "dpdPolymerData.h"
#include <cstdlib>

#define PHOBE_PHOBE 1
#define PHIL_PHIL 2

DPDPolymerData::DPDPolymerData() {}

DPDPolymerData::DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length ) {
  this->filename = filename;
  this->density = density;
  this->box_length = box_length;
  this->bond_length = bond_length;
  this->calcNumAtoms();
}

DPDPolymerData::DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length,
                  idx chain_length, unsigned short num_chains ):
                  DPDPolymerData( filename, density, box_length, bond_length ) {
  this->chain_length = chain_length;
  this->num_chains = num_chains;
  this->calcNumFluid();
  this->calcNumBonds();
}

void DPDPolymerData::calcNumAtoms() {
  this->num_atoms = box_length*box_length*box_length*density;
}

void DPDPolymerData::calcNumFluid() {
  this->num_Fluid = this->num_atoms - this->num_chains*this->chain_length;
}

void DPDPolymerData::calcNumBonds() {
  this->num_bonds = this->num_chains*( this->chain_length - 1 );
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TriblockData::TriblockData(): DPDPolymerData() {}

TriblockData::TriblockData( std::string filename, idx box_length, float bond_length,
                            float polymer_volume_fraction, idx pec_length, idx tail_length ):
                            DPDPolymerData( filename, FLUID_DENSITY, box_length, bond_length ) {
  this->pec_length = pec_length;
  this->tail_length = tail_length;
  this->calcChainLength();
  this->calcNumChains( &polymer_volume_fraction );
  this->calcNumFluid();
  this->calcNumBonds();
  this->chainList = new PECTriblock[ this->num_chains ];
  this->chainCursor = 0;
  this->FluidList = new Bead[ this->num_Fluid ];
  this->FluidCursor = 0;
  this->bondList = new Bond[ this->num_bonds ];
  this->bondCursor = 0;
  this->idTracker = 1;
}

void TriblockData::calcChainLength() {
  this->chain_length = this->pec_length + 2*this->tail_length;
}

void TriblockData::calcNumChains( float* polymer_volume_fraction ) {
  this->num_chains = ( *polymer_volume_fraction * this->num_atoms / this->chain_length ) + .5;
}

void TriblockData::generate() {
  this->deriveChainList();
  this->deriveBondList();
  this->print();
}

void TriblockData::deriveBondList() {
  PECTriblock* chain = NULL;

  for ( unsigned short i = 0; i  < this->num_chains; i++ ) {
    chain = this->chainList + i;
  
    for( idx i = 0; i < chain->tail_length - 1; i++ ) {
      this->makeNewBond( PHOBE_PHOBE, chain->tail1->beadList + i,
                    chain->tail1->beadList + i + 1 );
    }
  
    this->makeNewBond( PHIL_PHIL, chain->tail1->beadList + chain->tail_length - 1,
                chain->pec_block->beadList );
    for( idx i = 0; i < chain->pec_length - 1; i++ ) {
      this->makeNewBond( PHIL_PHIL, chain->pec_block->beadList + i,
                    chain->pec_block->beadList + i + 1 );
    }

    this->makeNewBond( PHIL_PHIL, chain->pec_block->beadList + chain->pec_length - 1,
                chain->tail2->beadList );
    for( idx i = 0; i < chain->tail_length - 1; i++ ) {
      this->makeNewBond( PHOBE_PHOBE, chain->tail2->beadList + i,
                    chain->tail2->beadList + i + 1 );
    }
  }
  
}

void DPDPolymerData::makeNewBond( idx type, Bead* bead1, Bead* bead2 ) {
  Bond* b = new Bond( type, this->bondCursor + 1, bead1, bead2 );
  this->addBond( b );
}

// Write this function
// DEBUG
void TriblockData::deriveChainList() {
  PECTriblock* chain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = new PECTriblock( &( this->box_length ), &( this->bond_length ),
                               this->pec_length, this->tail_length, this->chain_length,
                               &( this->idTracker ), this->chainCursor );
    this->addChain( chain );
  }
}

void TriblockData::addChain( PECTriblock* chain ) {
  if ( this->chainCursor >= this->num_chains ) {
    fprintf( stdout, "Error, adding too many chains." );
    exit( 1 );
  }
  this->chainList[ this->chainCursor ] = *chain;
  this->chainCursor++;
}

void DPDPolymerData::addBond( Bond* bond ) {
  if ( this->bondCursor >= this->num_bonds ) {
    fprintf( stdout, "Error, adding too many chains." );
    exit( 1 );
  }
  this->bondList[ this->bondCursor ] = *bond;
  this->bondCursor++;
}

void TriblockData::print() {}

// Should properly delete everything
TriblockData::~TriblockData() {}

////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(TESTING)
#include <iostream>
#include <ctime>
using namespace std;

int main() {

  TriblockData *data = new TriblockData( "tester", 36, .1f, .1f, 30, 4 );
  cout << (int) data->chain_length << endl;
  cout << data->num_atoms << endl;
  cout << data->num_chains << endl;
  cout << data->num_Fluid << endl;

  delete data;

  srand(2);
  data = new TriblockData( "again", 3, .1f, .1f, 3, 2 );

  data->generate();

  for ( int i = 0; i < data->num_chains; i++ ) {
    data->chainList[ i ].printChain( stdout );
  }
  
  for ( int i = 0; i < data->num_bonds; i++ ) {
    data->bondList[ i ].printBond( stdout );
  }

  return 0;
}

#endif

