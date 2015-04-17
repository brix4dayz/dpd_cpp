//#include "copolymerChain.h"
#include "bin.h"
#include <cstdlib>

PECTriblock::PECTriblock() {}

// Generic constuctor
PECTriblock::PECTriblock( idx pec_length, idx tail_length, idx length ) {
  this->com = new PosVect();
  
  this->pec_length = pec_length;
	this->tail_length = tail_length;
	this->pec_block = new ChargedBlock( this, HYDROPHILIC, pec_length );
	this->tail1 = new HydrophobicTail( this, tail_length );
	this->tail2 = new HydrophobicTail( this, tail_length );
  this->chain_length = length;
	this->tail1->other = this->tail2;
	this->tail2->other = this->tail1;
	this->micelle = NULL;
}

// Constructs from reading input file
// Should I add a void ptr to the frame as a parameter?
PECTriblock::PECTriblock( idx pec_length, idx tail_length, idx length,
                          std::ifstream* inFile,
                          idx* box_length, const float& pbc_correction_factor ) {
	this->com = new PosVect();
  
  this->pec_length = pec_length;
	this->tail_length = tail_length;
	this->tail1 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length, pbc_correction_factor );
	this->pec_block = new ChargedBlock( this, 
	 HYDROPHILIC, pec_length, inFile, box_length, pbc_correction_factor );
	this->tail2 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length, pbc_correction_factor );
  this->chain_length = length;
	this->tail1->other = this->tail2;
	this->tail2->other = this->tail1;
	this->micelle = NULL;
}

// Chain length should be calculated once and passed as a parameter...

// Constructs a chain with a random position within a box
// DEBUG
PECTriblock::PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length, 
                          unsigned int* idTracker, unsigned short id ) {
  this->com = new PosVect();
  
  this->id = id + 1;
  this->pec_length = pec_length;
  this->tail_length = tail_length;
  this->chain_length = length;
  
  DirVect* d = new DirVect( bond_length ); // assigns random direction for chain
  PosVect* first = new PosVect( box_length ); // assigns random position for first bead
  this->tail1 = new HydrophobicTail( this, tail_length, d, box_length, first, idTracker, this->id ); // makes tail1 from
  // first position and random direction
  
  // determines first position of pec block using random direction and 
  // last position of tail block
  first = new PosVect( this->tail1->beadList[ tail_length - 1 ]->r, d );
  this->pec_block = new ChargedBlock( this, HYDROPHILIC, pec_length, d, box_length,
                                      first, idTracker, this->id );
  

  // determines first position of tail2 using random direction and
  // last position of pec block
  first = new PosVect( this->pec_block->beadList[ pec_length - 1 ]->r, d );
  this->tail2 = new HydrophobicTail( this, tail_length, d, box_length,
                                     first, idTracker, this->id );
  delete d;

  this->tail1->other = this->tail2;
  this->tail2->other = this->tail1;
  this->micelle = NULL;
}

void PECTriblock::printChain( FILE* stream ) {
  this->tail1->printBlock( stream );
	this->pec_block->printBlock( stream );
	this->tail2->printBlock( stream );
}

void PECTriblock::printData( FILE* stream ) {
  this->tail1->printData( stream );
  this->pec_block->printData( stream );
  this->tail2->printData( stream ); 
}

void PECTriblock::unlink() {
  this->pec_block = NULL;
  this->tail1 = NULL;
  this->tail2 = NULL;
  this->frame = NULL;
}

//Deconstructor... clean up
PECTriblock::~PECTriblock() {
  delete pec_block;
  delete tail1;
  delete tail2;
  this->unlink();
}

void PECTriblock::colorChain( idx type ) {
  for ( idx i = 0; i < this->pec_length; i++ ) {
    this->pec_block->getBead( i )->type = type;
  }

  for ( idx i = 0; i < this->tail_length; i++ ) {
    this->tail1->getBead( i )->type = type;
    this->tail2->getBead( i )->type = type;
  }
}

// maybe builds "edge"
uintptr_t PECTriblock::determineConfiguration() {
  HydrophobicCore* core1 = this->tail1->getCore();
  HydrophobicCore* core2 = this->tail2->getCore();
  if ( core1 == core2 && core1 ) {
    this->config = petal;
    return (uintptr_t) NULL;
  }
  else if ( core1 && core2 ) {
    this->config = stem;
    return Stem::hashCores( core1, core2 ); 
  } else {
    this->config = neither;
    return (uintptr_t) NULL;
  }
}

Stem::Stem( HydrophobicCore* c1, HydrophobicCore* c2 ) {
  this->core1 = c1;
  this->core2 = c2;
  this->count = 1;
  this->grouped = false;
}

void Stem::unlink() {
  this->core1 = NULL;
  this->core2 = NULL;
}

Stem::~Stem() {
  this->unlink();
}

void Stem::inc() {
  this->count++;
}

uintptr_t Stem::getKey() {
  return Stem::hashCores( this->core1, this->core2 );
}

uintptr_t Stem::hashCores( HydrophobicCore* c1, HydrophobicCore* c2 ) {
  return ( (uintptr_t) c1 ) ^ ( (uintptr_t) c2 );
}


#if defined( TESTING )
#include <iostream>
#include <ctime>

int main() {
	const float pbc_correction_factor = 0.5f;

  std::ifstream infile( "bead_test.txt" );
	idx box_length = 36;
	PECTriblock* chain = new PECTriblock( 50, 4, 58, &infile, &box_length, pbc_correction_factor );
	chain->printChain( stdout );
	std::cout << (short) chain->chain_length << std::endl;
	
  // test determineConfiguration
  chain->determineConfiguration();
	if ( chain->config != neither )
		std::cout << "Fail" << std::endl;

  delete chain;
  chain = NULL;

  // Test random chain maker instead  
  srand(2);
  float bond_length = 0.1f;
  unsigned int id = 0;
  chain = new PECTriblock( &box_length, &bond_length, 30, 5, 40, &id, 1 );
  chain->printChain( stdout );

  delete chain;
  
  
  /*DirVect* dir = new DirVect( bond_length );
  std::cout << "dx: " << dir->dx << std::endl;
  std::cout << "dy: " << dir->dy << std::endl;
  std::cout << "dz: " << dir->dz << std::endl;
  PosVect* r = new PosVect( &box_length );
  std::cout << "x: " << r->x << std::endl;
  std::cout << "y: " << r->y << std::endl;
  std::cout << "z: " << r->z << std::endl;*/

	return 0;
}

#endif