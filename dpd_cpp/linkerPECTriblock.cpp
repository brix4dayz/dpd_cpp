#include "linkerPECTriblock.h"

LinkerPECTriblock::LinkerPECTriblock() : SymmetricAmphiphilicTriblock() {}

// Constructs from reading input file
// Should I add a void ptr to the frame as a parameter?
LinkerPECTriblock::LinkerPECTriblock( idx pec_length, idx tail_length, idx link_length, byte linker_type, idx length, 
                                      std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor ) : 
                                      SymmetricAmphiphilicTriblock( pec_length, tail_length, length ) {
  this->link_length = link_length;
  this->chain_length = length;
  this->tail1 = new HydrophobicTail( this, tail_length, 
   inFile, box_length, pbc_correction_factor );
  this->pec1 = new ChargedBlock( this, 
   HYDROPHILIC, pec_length, inFile, box_length, pbc_correction_factor );
  this->stiffLink = new PolymerBlock( this, linker_type, link_length, inFile, box_length, pbc_correction_factor );
  this->pec2 = new ChargedBlock( this, 
   HYDROPHILIC, pec_length, inFile, box_length, pbc_correction_factor );
  this->tail2 = new HydrophobicTail( this, tail_length, 
   inFile, box_length, pbc_correction_factor );
  this->linkTails();
}

// Constructs a linear chain with a random position and direction 
// within a box, the edges are not avoided b/c using PBC, each
// bead is bond_length distance apart
LinkerPECTriblock::LinkerPECTriblock( idx* box_length, float* bond_length, idx pec_length, idx tail_length, idx link_length, 
                    byte linker_type, idx length, unsigned int* idTracker, unsigned short id, 
                    byte uncharged_type, idx num_uncharged, IntegerDice<idx>& chargeDice ) : 
                    SymmetricAmphiphilicTriblock( pec_length, tail_length, length ) {
  this->id = id + 1;
  
  DirVect* d = new DirVect( bond_length ); // assigns random direction for chain
  PosVect* first = new PosVect( box_length ); // assigns random position for first bead
  this->tail1 = new HydrophobicTail( this, tail_length, d, box_length, first, idTracker, this->id ); // makes tail1 from
  // first position and random direction
  
  // determines first position of pec block using random direction and 
  // last position of tail block
  first = new PosVect( this->tail1->getBead( tail_length - 1 )->r, d );
  this->pec1 = new ChargedBlock( this, HYDROPHILIC, pec_length, d, box_length,
                                      first, idTracker, this->id, uncharged_type, 
                                      num_uncharged, chargeDice );

  first = new PosVect( this->pec1->getBead( pec_length - 1 )->r, d );
  this->stiffLink = new PolymerBlock( this, linker_type, link_length, d, box_length, first, idTracker, this->id );
 
  // determines first position of pec block using random direction and 
  // last position of tail block
  first = new PosVect( this->stiffLink->getBead( link_length - 1 )->r, d );
  this->pec2 = new ChargedBlock( this, HYDROPHILIC, pec_length, d, box_length,
                                      first, idTracker, this->id, uncharged_type, 
                                      num_uncharged, chargeDice ); 

  // determines first position of tail2 using random direction and
  // last position of pec block
  first = new PosVect( this->pec2->getBead( pec_length - 1 )->r, d );
  this->tail2 = new HydrophobicTail( this, tail_length, d, box_length,
                                     first, idTracker, this->id );
  delete d;

  this->linkTails();
}

void LinkerPECTriblock::printChain( FILE* stream ) {
  this->tail1->printBlock( stream );
  this->pec1->printBlock( stream );
  this->stiffLink->printBlock( stream );
  this->pec2->printBlock( stream );
  this->tail2->printBlock( stream );
}

void LinkerPECTriblock::printData( FILE* stream ) {
  this->tail1->printData( stream );
  this->pec1->printData( stream );
  this->stiffLink->printData( stream );
  this->pec2->printData( stream );
  this->tail2->printData( stream );
}

void LinkerPECTriblock::unlink() {
  this->pec1 = NULL;
  this->pec2 = NULL;
  this->stiffLink = NULL;
}

void LinkerPECTriblock::colorChain( byte type ) {
  this->pec1->color( type );
  this->pec2->color( type );
  this->stiffLink->color( type );
  this->colorTails( type );
}

//Deconstructor... clean up
LinkerPECTriblock::~LinkerPECTriblock() {
  delete pec1;
  delete pec2;
  delete stiffLink;
  this->unlink();
}
