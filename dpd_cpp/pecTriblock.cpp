//#include "copolymerChain.h"
#include "bin.h"
#include <cstdlib>

SymmetricAmphiphilicTriblock::SymmetricAmphiphilicTriblock() {}

SymmetricAmphiphilicTriblock::SymmetricAmphiphilicTriblock( idx pec_length, idx tail_length, idx length ) {
  this->com = new PosVect();
  this->pec_length = pec_length;
  this->tail_length = tail_length;
  this->chain_length = length;
  this->micelle = NULL;
}

void SymmetricAmphiphilicTriblock::linkTails() {
  this->tail1->other = this->tail2;
  this->tail2->other = this->tail1;
}

PECTriblock::PECTriblock() {}

// Generic constuctor
PECTriblock::PECTriblock( idx pec_length, idx tail_length, idx length ) : 
                          SymmetricAmphiphilicTriblock( pec_length, tail_length, 
                                                        length ) {
	this->pec_block = new ChargedBlock( this, HYDROPHILIC, pec_length );
	this->tail1 = new HydrophobicTail( this, tail_length );
	this->tail2 = new HydrophobicTail( this, tail_length );
  this->linkTails();
}

// Constructs from reading input file
// Should I add a void ptr to the frame as a parameter?
PECTriblock::PECTriblock( idx pec_length, idx tail_length, idx length,
                          std::ifstream* inFile,
                          idx* box_length, const float& pbc_correction_factor ) : 
                          SymmetricAmphiphilicTriblock( pec_length, tail_length, 
                                                        length ) {

	this->tail1 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length, pbc_correction_factor );
	this->pec_block = new ChargedBlock( this, 
	 HYDROPHILIC, pec_length, inFile, box_length, pbc_correction_factor );
	this->tail2 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length, pbc_correction_factor );
  this->linkTails();
}

PECTriblock::PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length, 
                          unsigned int* idTracker, unsigned short id ) : 
                          SymmetricAmphiphilicTriblock( pec_length, tail_length, length ) {
 this->id = id + 1;
  
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

  this->linkTails();
}

// Constructs a linear chain with a random position and direction 
// within a box, the edges are not avoided b/c using PBC, each
// bead is bond_length distance apart
PECTriblock::PECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx length, 
                          unsigned int* idTracker, unsigned short id, byte& uncharged_type, idx& num_uncharged,
                          IntegerDice<idx>& chargeDice ) : SymmetricAmphiphilicTriblock( pec_length, tail_length, 
                                                                                         length ) {
  
  this->id = id + 1;
  
  DirVect* d = new DirVect( bond_length ); // assigns random direction for chain
  PosVect* first = new PosVect( box_length ); // assigns random position for first bead
  this->tail1 = new HydrophobicTail( this, tail_length, d, box_length, first, idTracker, this->id ); // makes tail1 from
  // first position and random direction
  
  // determines first position of pec block using random direction and 
  // last position of tail block
  first = new PosVect( this->tail1->beadList[ tail_length - 1 ]->r, d );
  this->pec_block = new ChargedBlock( this, HYDROPHILIC, pec_length, d, box_length,
                                      first, idTracker, this->id, uncharged_type, 
                                      num_uncharged, chargeDice );
  

  // determines first position of tail2 using random direction and
  // last position of pec block
  first = new PosVect( this->pec_block->beadList[ pec_length - 1 ]->r, d );
  this->tail2 = new HydrophobicTail( this, tail_length, d, box_length,
                                     first, idTracker, this->id );
  delete d;

  this->linkTails();
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

void SymmetricAmphiphilicTriblock::unlink() {
  this->tail1 = NULL;
  this->tail2 = NULL;
}

SymmetricAmphiphilicTriblock::~SymmetricAmphiphilicTriblock() {
  delete tail1;
  delete tail2;
  this->unlink();
}

void PECTriblock::unlink() {
  this->pec_block = NULL;
}

//Deconstructor... clean up
PECTriblock::~PECTriblock() {
  delete pec_block;
  this->unlink();
}

void SymmetricAmphiphilicTriblock::colorTails( byte type ) {
  this->tail1->color( type );
  this->tail2->color( type );
}

void PECTriblock::colorChain( byte type ) {
  this->pec_block->color( type );
  this->colorTails( type );
}

// maybe builds "edge"
uintptr_t SymmetricAmphiphilicTriblock::determineConfiguration() {
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