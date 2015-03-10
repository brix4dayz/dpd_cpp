#include "dpdPolymerData.h"
#include <cstdlib>
#include <cmath>

// Consider putting a few of the functions and data in polymerData in dpdData.

#define PHOBE_PHOBE 1
#define PHIL_PHIL 2

#define FLUID_ID_TRIBLOCK 3

DPDPolymerData::DPDPolymerData() {}

DPDPolymerData::DPDPolymerData( std::string filename, idx density, 
                                idx box_length, float bond_length, idx Fluid_type ) {
  this->Fluid_type = Fluid_type;
  this->filename = filename;
  this->density = density;
  this->box_length = box_length;
  this->bond_length = bond_length;
  this->calcNumAtoms();
}

DPDPolymerData::DPDPolymerData( std::string filename, idx density, idx box_length, float bond_length,
                                idx chain_length, unsigned short num_chains, idx Fluid_type ):
                                DPDPolymerData( filename, density, box_length, bond_length, Fluid_type ) {
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
                            DPDPolymerData( filename, FLUID_DENSITY, box_length, 
                            bond_length, FLUID_ID_TRIBLOCK ) {
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
  this->deriveFluidList();
  FILE* fp = fopen( this->filename.c_str(), "w" );
  this->printLAMMPS( fp );
  fclose( fp );
}

void DPDPolymerData::wereAllBeadsMade() {
  unsigned int atoms_added = this->idTracker - 1; // -1 for idTracker being one ahead in index
  if ( this->num_atoms != atoms_added ) {
    fprintf( stdout, "Error in number of atoms added to system.\nExpected: %d\nActual: %d\n", 
             this->num_atoms, atoms_added );
    exit( 1 );
  }
}

void DPDPolymerData::wereAllBondsMade() {
  if ( this->num_bonds != this->bondCursor ) {
    fprintf( stdout, "Error in number of bonds added to system.\nExpected: %d\nActual: %d\n", 
             this->num_bonds, this->bondCursor );
    exit( 1 );
  }
}

void TriblockData::deriveBondList() {
  PECTriblock* chain = NULL;

  for ( unsigned short i = 0; i  < this->num_chains; i++ ) {
    chain = this->chainList + i;
  
    this->addBlockBonds( chain->tail1, PHOBE_PHOBE );
  
    this->makeNewBond( PHIL_PHIL, chain->tail1->beadList + chain->tail_length - 1,
                chain->pec_block->beadList );
    this->addBlockBonds( chain->pec_block, PHIL_PHIL );

    this->makeNewBond( PHIL_PHIL, chain->pec_block->beadList + chain->pec_length - 1,
                chain->tail2->beadList );
    this->addBlockBonds( chain->tail2, PHOBE_PHOBE );
  }
  this->wereAllBondsMade();
}


void DPDPolymerData::addBlockBonds( PolymerBlock* block, idx bond_type ) {
  for ( idx i = 0; i < block->length - 1; i++ ) {
    this->makeNewBond( bond_type, block->beadList + i,
                       block->beadList + i + 1 );
  }
}

void DPDPolymerData::makeNewBond( idx type, Bead* bead1, Bead* bead2 ) {
  Bond* b = new Bond( type, this->bondCursor + 1, bead1, bead2 );
  this->addBond( b );
}

void TriblockData::deriveChainList() {
  PECTriblock* chain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = new PECTriblock( &( this->box_length ), &( this->bond_length ),
                                this->pec_length, this->tail_length, this->chain_length,
                                &( this->idTracker ), this->chainCursor );
    this->addChain( chain );
  }
  this->molIDTracker = this->num_chains;
  if ( this->chainCursor != this->molIDTracker ) {
    fprintf( stdout, "Error in current number of chains added to system.\nExpected: %d\nActual: %d\n", 
             this->molIDTracker, this->chainCursor );
    exit( 1 );
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

bool DPDPolymerData::addFluid( Bead* bead ) { 
  if ( this->FluidCursor >= this->num_Fluid ) return false;
  this->FluidList[ this->FluidCursor ] = *bead;
  this->FluidCursor++;
  return true;
}

// add collision detection with binlist
void DPDPolymerData::deriveFluidList() {
  PosVect* r = NULL;
  Bead* b = NULL;

  double fluid_offset = 0.0;

  while ( this->FluidCursor < this->num_Fluid ) { // leq to guarantees addFluid is done one extra time
    // puts a fluid bead at every integer position plus a changing offset within the box
    for ( idx i = 0; i <= box_length; i++ ) {
      for ( idx j = 0; j <= box_length; j++ ) {
        for ( idx k = 0; k <= box_length; k++ ) {
          r = new PosVect( ( ( double ) i ) + fluid_offset, 
                         ( ( double ) j ) + fluid_offset,
                         ( ( double ) k ) + fluid_offset );
          b = new Bead( r, this->Fluid_type, &( this->idTracker ), this->molIDTracker + 1 );
          this->molIDTracker++;
          if ( !this->addFluid( b ) ) {
            this->idTracker--; // idTracker gets incremented one too many times, fix it
            delete b; // extra bead made, free the memory
            k = box_length + 1;
            j = box_length + 1;
            i = box_length + 1;
          } 
        }
      }
    }
    fluid_offset = ( 1.0 - fluid_offset )/2.0; // offset initially zero than goes to .5, and approaches 1/3
  }

  this->wereAllBeadsMade();
}

void TriblockData::printLAMMPS( FILE* fp ) {

  fprintf( fp, "LAMMPS Description\n\n");
  fprintf( fp, "%d atoms\n%d bonds\n\n", this->num_atoms, this->num_bonds );
  fprintf( fp, "%d atom types\n2 bond types\n\n", this->Fluid_type );

  fprintf( fp, "%e %e xlo xhi\n", 0.0, ( (double) this->box_length ) );
  fprintf( fp, "%e %e ylo yhi\n", 0.0, ( (double) this->box_length ) );
  fprintf( fp, "%e %e zlo zhi\n", 0.0, ( (double) this->box_length ) );
  fprintf( fp, "\nAtoms\n\n");

  for ( int i = 0; i < this->num_chains; i++ ) {
    this->chainList[ i ].printData( fp );
  }

  for ( int i = 0; i < this->num_Fluid; i++ ) {
    this->FluidList[ i ].printData( fp );
  }

  fprintf( fp, "\nBonds\n\n" );

  for ( int i = 0; i < this->num_bonds; i++ ) {
    this->bondList[ i ].printBond( fp );
  }

}

// Should properly delete everything
TriblockData::~TriblockData() {}

ChargeTriblockData::ChargeTriblockData( std::string filename, idx box_length, float bond_length,
                                       float polymer_volume_fraction, idx pec_length, idx tail_length, float charge_density ):
                                        TriblockData( filename, box_length, bond_length,
                                        polymer_volume_fraction, pec_length, tail_length ) {
  this->Fluid_type += 1;
  this->charge_density = charge_density;
}

void ChargeTriblockData::deriveChainList() {
  PECTriblock* chain = NULL;
  float uncharged_density = 1.0f - this->charge_density;
  idx num_uncharged = uncharged_density*this->pec_length + 1.0f;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = new PECTriblock( &( this->box_length ), &( this->bond_length ),
                            this->pec_length, this->tail_length, this->chain_length,
                            &( this->idTracker ), this->chainCursor );
    idx uncharged_counter = 0;
    while ( uncharged_counter < num_uncharged ) {
      for ( idx pec_counter = 0; pec_counter < this->pec_length; pec_counter++ ) {
        if ( gauss() <= uncharged_density ) {
          uncharged_counter++;
          chain->pec_block->beadList[ pec_counter ].type = FLUID_ID_TRIBLOCK;
          if ( uncharged_counter >= num_uncharged )
            break;
        }
      }
    }
    this->addChain( chain );
  }
  this->molIDTracker = this->num_chains;
  if ( this->chainCursor != this->molIDTracker )
    printf("%d problem2\n", this->chainCursor);
}

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
  data = new TriblockData( "newdataTest.dat", 3, .1f, .1f, 3, 2 );
  cout << (int) data->chain_length << endl;
  cout << data->num_atoms << endl;
  cout << data->num_chains << endl;
  cout << data->num_Fluid << endl;
  data->generate();
  int diff = (data->idTracker - data->num_Fluid);
  cout << diff << endl;

  data->printLAMMPS( stdout );

  return 0;
}

#endif

