#include "micelle.h"

Micelle::Micelle( DPDFrame* frame ) {
	this->frame = frame;
	this->aggreg_num = 0;
  this->com = new PosVect();
}

Micelle::Micelle(): Micelle( NULL ) {}

Micelle::~Micelle() {}

void Micelle::addCore( HydrophobicCore* core ) {
	this->aggreg_num += core->aggregation_num;
	core->micelle = this;
	this->coreList.push_back( core );
}

void Micelle::printMicelleCore( FILE* stream ) {
	for ( auto core = std::begin( this->coreList ) ; core != std::end( this->coreList ) ; core++ ) {
    ( *core )->printCore( stream );
	}
}

TriblockMicelle::TriblockMicelle() : Micelle() {}

TriblockMicelle::TriblockMicelle( DPDFrame* frame ): Micelle( frame ) {}

void TriblockMicelle::addChain( PECTriblock* chain ) {
	this->chainList.push_back( chain );
	chain->micelle = this;
}

void TriblockMicelle::deriveChainList() {
	PECTriblock* chain;
	for ( auto core = std::begin( this->coreList ) ; core != std::end( this->coreList ) ; core++ ) {
		for ( auto bin = std::begin( ( *core )->binList ) ; bin != std::end( ( *core )->binList ) ; bin++ ) {
			for ( auto tail = std::begin( ( *bin )->tailList ) ; tail != std::end( ( *bin )->tailList ) ; tail++ ) {
				chain = ( PECTriblock* ) ( *tail )->chain;
				if ( chain && !chain->micelle )
					this->addChain( chain );
			}
		}
	}
	if ( this->chainList.size() != this->aggreg_num )
		printf( "Error in deriving chain in micelle.\n" );
}

void TriblockMicelle::printMicelle( FILE* stream ) {
	for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
		( *chain )->printChain( stream );
	}
}

// Looks good but should test a little more to be sure...
void TriblockMicelle::pbcCorrectMicelle( idx* box_length ) {
  Bead* baseBead = this->chainList.at( 0 )->tail1->beadList;
  PECTriblock* triblock = NULL;
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    triblock = ( *chain );
    triblock->pec_block->beadList->pbcCorrectBeadInChain( baseBead, box_length );
    triblock->tail1->beadList->pbcCorrectBeadInChain( baseBead, box_length );
    triblock->tail2->beadList->pbcCorrectBeadInChain( baseBead, box_length );
    
    for ( idx i = 1 ; i < triblock->pec_block->length ; i ++ ) {
      ( triblock->pec_block->beadList + i )->pbcCorrectBeadInChain( triblock->pec_block->beadList, box_length );
    }
    
    for ( idx i = 1 ; i < triblock->tail1->length ; i++ ) {
      ( triblock->tail1->beadList + i )->pbcCorrectBeadInChain( triblock->tail1->beadList, box_length );
      ( triblock->tail2->beadList + i )->pbcCorrectBeadInChain( triblock->tail2->beadList, box_length );
    }
  
  }
}

// Needs to be reviewed before I write tests, not a crucical function though
void TriblockMicelle::calcCenterOfMass( idx* box_length ) {
  this->com->reset();
  this->pbcCorrectMicelle( box_length );
  
  PECTriblock* triblock = NULL;
  Bead *current = NULL;
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    triblock = ( *chain );
    
    for ( idx i = 0 ; i < triblock->pec_block->length ; i ++ ) {
      current = ( triblock->pec_block->beadList + i );
      this->com->addCoords( current->r );
    }
    
    for ( idx i = 0 ; i < triblock->tail1->length ; i++ ) {
      current = ( triblock->tail1->beadList + i );
      this->com->addCoords( current->r );
    
      current = ( triblock->tail2->beadList + i );
      this->com->addCoords( current->r );
    }
  
  }
  int ttlBeads = this->aggreg_num * triblock->chain_length;
  this->com->divideCoords( &ttlBeads );
}

#if defined( TESTING )
#include <iostream>

int main() {

	std::ifstream infile( "bin_test.txt" );
	idx box_length = 36;
	// Make two triblocks from file
	std::cout << "Chain1: " << std::endl;
	PECTriblock* chain1 = new PECTriblock( 50, 4, &infile, &box_length );
	chain1->printChain( stdout );
	std::cout << ( short ) chain1->chain_length << std::endl;

	std::cout << "Chain2: " << std::endl;
	PECTriblock* chain2 = new PECTriblock( 50, 4, &infile, &box_length );
	chain2->printChain( stdout );
	std::cout << ( short ) chain2->chain_length << std::endl;

	// Initialize two bins
	Bin* b1 = new Bin();
	Bin* b2 = new Bin();

	b1->init( NULL, 2, 0, 0, 0 );
	b2->init( NULL, 2, 0, 1, 0 );

	//Add tails
	b1->addTail( chain1->tail1 );
	b1->addTail( chain1->tail2 );

	b2->addTail( chain2->tail1 );
	b2->addTail( chain2->tail2 );

	// Make core and add bins
	HydrophobicCore* core1 = new HydrophobicCore();
	HydrophobicCore* core2 = new HydrophobicCore();

	core1->addBin( b1 );
	core2->addBin( b2 );

	// Test core functionality
	TriblockMicelle* micelle = new TriblockMicelle();

	micelle->addCore( core1 );
	micelle->addCore( core2 );

	std::cout << "MicelleCore: " << std::endl;
	micelle->printMicelleCore( stdout );

	if ( core1->micelle != micelle || core2->micelle != micelle || 
	 micelle->aggreg_num != 2 )
		std::cout << "Fail" << std::endl;

	// Test chain functionality
	micelle->deriveChainList();

	if ( chain1->micelle != micelle || chain2->micelle != micelle )
		std::cout << "Fail" << std::endl;

	std::cout << "Micelle: " << std::endl;
	micelle->printMicelle( stdout );
  
  // Only test so far for pbcCorrectMicelle
  micelle->pbcCorrectMicelle( &box_length );
  std::cout << "Micelle (corrected): " << std::endl;
  micelle->printMicelle( stdout );

  // Test calcCenterOfMass
  // but its not that important of a function
  
	return 0;
}

#endif