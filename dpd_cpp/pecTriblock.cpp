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
                          idx* box_length ) {
	this->com = new PosVect();
  
  this->pec_length = pec_length;
	this->tail_length = tail_length;
	this->tail1 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length );
	this->pec_block = new ChargedBlock( this, 
	 HYDROPHILIC, pec_length, inFile, box_length );
	this->tail2 = new HydrophobicTail( this, tail_length, 
	 inFile, box_length );
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

// maybe builds "edge"
void PECTriblock::determineConfiguration() {
  Bin* bin1 = this->tail1->bin;
  Bin* bin2 = this->tail2->bin;
  if (bin1 && bin2) {
    if ( bin1->core == bin2->core && bin1->core )
      this->config = petal;
    else if ( bin1->core || bin2->core )
      this->config = stem;
    else
      this->config = neither;
  } else
  	this->config = neither;
}

#if defined( TESTING )
#include <iostream>
#include <ctime>

int main() {
	std::ifstream infile( "bead_test.txt" );
	idx box_length = 36;
	PECTriblock* chain = new PECTriblock( 50, 4, 58, &infile, &box_length );
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