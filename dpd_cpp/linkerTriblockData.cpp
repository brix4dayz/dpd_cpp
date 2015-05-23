#include "linkerTriblockData.h"

#define LINK_LINK 3

LinkerTriblockData::LinkerTriblockData( std::string filename, idx box_length, float bond_length,
                  float polymer_volume_fraction, idx pec_length, idx tail_length, idx link_length,
                  float charge_density ) : DPDPolymerData( filename, FLUID_DENSITY, box_length, 
                            bond_length, FLUID_ID_TRIBLOCK + 1 ) {

  if ( charge_density < 1.0 )
    this->Fluid_type += 1;
  this->charge_density = charge_density;

  this->pec_length = pec_length;
  this->tail_length = tail_length;
  this->link_length = link_length;
  this->calcChainLength();
  this->calcNumChains( &polymer_volume_fraction );
  this->calcNumFluid();
  this->calcNumBonds();
  this->chainList = new LinkerPECTriblock[ this->num_chains ];
  this->chainCursor = 0;
  this->FluidList = new Bead[ this->num_Fluid ];
  this->FluidCursor = 0;
  this->bondList = new Bond[ this->num_bonds ];
  this->bondCursor = 0;
  this->idTracker = 1;
}

void LinkerTriblockData::deriveBondList() {
  LinkerPECTriblock* chain = NULL;

  for ( unsigned short i = 0; i  < this->num_chains; i++ ) {
    chain = this->chainList + i;
  
    this->addBlockBonds( chain->tail1, PHOBE_PHOBE );
  
    this->makeNewBond( PHIL_PHIL, chain->tail1->getBead( chain->tail_length - 1 ),
                chain->pec1->getBead( 0 ) );
    this->addBlockBonds( chain->pec1, PHIL_PHIL );

    this->makeNewBond( PHIL_PHIL, chain->pec1->getBead( chain->pec_length - 1 ),
                chain->stiffLink->getBead( 0 ) );
    this->addBlockBonds( chain->stiffLink, LINK_LINK );
  
    this->makeNewBond( PHIL_PHIL, chain->stiffLink->getBead( chain->link_length - 1 ),
                chain->pec2->getBead( 0 ) );
    this->addBlockBonds( chain->pec2, PHIL_PHIL );

    this->makeNewBond( PHIL_PHIL, chain->pec2->getBead( chain->tail_length - 1 ),
                chain->tail2->getBead( 0 ) );
    this->addBlockBonds( chain->tail2, PHOBE_PHOBE );
  }
  this->wereAllBondsMade();
}

void LinkerTriblockData::calcChainLength() {
  this->chain_length = this->link_length + 2*this->pec_length + 2*this->tail_length;
}

void LinkerTriblockData::deriveChainList() {
  IntegerDice<idx> chargeDice( 0, this->pec_length - 1 );
  LinkerPECTriblock* chain = NULL;
  byte uncharged_type = FLUID_ID_TRIBLOCK;
  float uncharged_density = 1.0f - this->charge_density;
  idx num_uncharged = uncharged_density*this->pec_length + 0.5f;
  byte linker_type = this->Fluid_type - 1;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = new LinkerPECTriblock( &( this->box_length ), &( this->bond_length ),
                            this->pec_length, this->tail_length, this->link_length, linker_type, 
                            this->chain_length, &( this->idTracker ), this->chainCursor, uncharged_type,
                            num_uncharged, chargeDice );

    this->addChain( chain );
  }
  this->molIDTracker = this->num_chains;
  if ( this->chainCursor != this->molIDTracker ) {
    fprintf( stdout, "Error in current number of chains added to system.\nExpected: %d\nActual: %d\n", 
             this->molIDTracker, this->chainCursor );
    exit( 1 );
  } 
}

void LinkerTriblockData::addChain( LinkerPECTriblock* chain ) {
  if ( this->chainCursor >= this->num_chains ) {
    fprintf( stdout, "Error, adding too many chains." );
    exit( 1 );
  }
  this->chainList[ this->chainCursor ] = *chain;
  this->chainCursor++;
}

void LinkerTriblockData::generate() {
  this->deriveChainList();
  this->deriveBondList();
  this->deriveFluidList();
  FILE* fp = fopen( this->filename.c_str(), "w" );
  this->printLAMMPS( fp );
  fclose( fp );
}

void LinkerTriblockData::printLAMMPS( FILE* fp ) {
  this->printLAMMPSHeader( fp );
  
  for ( unsigned int i = 0; i < this->num_chains; i++ ) {
    this->chainList[ i ].printData( fp );
  }

  for ( unsigned int i = 0; i < this->num_Fluid; i++ ) {
    this->FluidList[ i ].printData( fp );
  }

  fprintf( fp, "\nBonds\n\n" );

  for ( unsigned int i = 0; i < this->num_bonds; i++ ) {
    this->bondList[ i ].printBond( fp );
  }
}

LinkerTriblockData::~LinkerTriblockData() {}

#if defined(USING)
#include <ctime>
#include <iostream>
#include <string>

int main() {

  // Make this input handling a function within a ChargeTriblockBuilder class which descends from
  // DPDBuilder.
  float bond_length;
  float polymer_volume_fraction;
  float pec_charge_density;
  int temp;
  idx box_length;
  idx pec_length;
  idx tail_length;
  idx link_length;
  std::string datFile;

  srand( time( NULL ) );
  
  std::cout << "Enter box length: ";
  std::cin >> temp;
  box_length = (idx) temp;
  
  std::cout << "Enter bond length: ";
  std::cin >> bond_length;
  
  std::cout << "Enter polymer volume fraction: ";
  std::cin >> polymer_volume_fraction;
  
  std::cout << "Enter polyelectrolyte (pec) block length: ";
  std::cin >> temp;
  pec_length = (idx) temp;
  
  std::cout << "Enter hydrophobic tail length: ";
  std::cin >> temp;
  tail_length = (idx) temp;
  
  std::cout << "Enter linker block length: ";
  std::cin >> temp;
  link_length = (idx) temp;

  std::cout << "Enter pec block charge density: ";
  std::cin >> pec_charge_density;
  
  std::cout << "Enter desired filename: ";
  std::cin >> datFile;
  
  LinkerTriblockData* t = new LinkerTriblockData( datFile, box_length, bond_length, polymer_volume_fraction, 
                                                  pec_length, tail_length, link_length, pec_charge_density );

  t->generate();
  
  delete t;

  return 0;
}

#endif