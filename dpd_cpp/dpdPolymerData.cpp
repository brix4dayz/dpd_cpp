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
  this->deriveFluidList();
  //if ( this->num_atoms != this->idTracker ) {
    //fprintf( stdout, "Not all atoms made...\n" );
    //exit( 1 );
  //}
  //printf( "%d %d\n", this->num_atoms, this->idTracker );
  this->deriveBondList();
  FILE* fp = fopen( this->filename.c_str(), "w" );
  this->printLAMMPS( fp );
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
  
  if ( this->num_bonds != this->bondCursor )
    printf("problem3");

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
    //unsigned int oldID = this->idTracker;
    chain = new PECTriblock( &( this->box_length ), &( this->bond_length ),
                                this->pec_length, this->tail_length, this->chain_length,
                                &( this->idTracker ), this->chainCursor );
    //if ( this->idTracker - oldID != this->chain_length || 
      //   this->idTracker - oldID != chain->chain_length )
      //printf("%d problem", (int) chain->chain_length );
    this->addChain( chain );
  }
  this->molIDTracker = this->num_chains;
  if ( this->chainCursor != this->molIDTracker )
    printf("%d problem2\n", this->chainCursor); 
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
  if ( this->FluidCursor >= this->num_Fluid )
    return false;
  this->FluidList[ this->FluidCursor ] = *bead;
  this->FluidCursor++;
  return true;
}

void DPDPolymerData::deriveFluidList() {
  PosVect* r = NULL;
  Bead* b = NULL;

  int oldID = this->idTracker;

  // puts a fluid bead at every whole number position within box
  for ( idx i = 0; i <= box_length; i++ ) {
    for ( idx j = 0; j <= box_length; j++ ) {
      for ( idx k = 0; k <= box_length; k++ ) {
        r = new PosVect( ( ( double ) i ), 
                       ( ( double ) j ),
                       ( ( double ) k ) );
        b = new Bead( r, this->Fluid_type, &( this->idTracker ), this->molIDTracker + 1 );
        this->molIDTracker++;
        if ( !this->addFluid( b ) ) {
          delete b; // extra bead made
          k = box_length + 1;
          j = box_length + 1;
          i = box_length + 1;
        } 
      }
    }
  }

  // if that didn't make all the fluid beads, put a bead at every half position within box
  if ( this->idTracker - oldID - 1 != this->num_Fluid ) {
     for ( idx i = 0; i <= box_length; i++ ) {
      for ( idx j = 0; j <= box_length; j++ ) {
        for ( idx k = 0; k <= box_length; k++ ) {
          r = new PosVect( ( ( double ) i ) + .5, 
                       ( ( double ) j ) + .5,
                       ( ( double ) k ) + .5 );
          b = new Bead( r, this->Fluid_type, &( this->idTracker ), this->molIDTracker + 1 );
          this->molIDTracker++;
          if ( !this->addFluid( b ) ) {
            delete b;
            k = box_length + 1;
            j = box_length + 1;
            i = box_length + 1;
          } 
        }
      }
    }
  }

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

  fclose( fp );
}

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

