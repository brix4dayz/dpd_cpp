#include "micelle.h"

Micelle::Micelle( DPDFrame<CopolymerChain>* frame ) {
	this->frame = frame;
	this->aggreg_num = 0;
  this->com = new PosVect();
}

Micelle::Micelle(): Micelle( NULL ) {}

void Micelle::unlink() {
  this->frame = NULL;
  for ( auto core = std::begin( this->coreList ); core != std::end( this->coreList ); core++ ) {
    *core = NULL;
  }
}

Micelle::~Micelle() {
  delete this->com;

  for ( auto core = std::begin( this->coreList ); core != std::end( this->coreList ); core++ ) {
    delete *core;
  }

  this->unlink();
}

void Micelle::addCore( HydrophobicCore* core ) {
	if ( !core->grouped ) {
    core->grouped = true;
    core->micelle = this;
    this->coreList.push_back( core );
  }
}

void Micelle::printMicelleCore( FILE* stream ) {
	for ( auto core = std::begin( this->coreList ) ; core != std::end( this->coreList ) ; core++ ) {
    ( *core )->printCore( stream );
	}
}

TriblockMicelle::TriblockMicelle() : Micelle() {}

TriblockMicelle::TriblockMicelle( DPDFrame<CopolymerChain>* frame ): Micelle( frame ) {}

void TriblockMicelle::addChain( PECTriblock* chain ) {
	this->chainList.push_back( chain );
	chain->micelle = this;
}

void TriblockMicelle::deriveChainList() {
	PECTriblock* chain;
  HydrophobicCore* c1;
  HydrophobicCore* c2;
	for ( auto core = std::begin( this->coreList ) ; core != std::end( this->coreList ) ; core++ ) {
		for ( auto bin = std::begin( ( *core )->binList ) ; bin != std::end( ( *core )->binList ) ; bin++ ) {
			for ( auto tail = std::begin( ( *bin )->tailList ) ; tail != std::end( ( *bin )->tailList ) ; tail++ ) {
				chain = ( PECTriblock* ) ( *tail )->chain;
				if ( chain && !chain->micelle ) {
          this->addChain( chain );
          c1 = chain->tail1->getCore();
          c2 = chain->tail2->getCore();
          if ( c1 )
            c1->aggregation_num++;
          if ( c2 && c1 != c2 )
            c2->aggregation_num++;        
        }

			}
		}
	}
	//if ( this->chainList.size() != this->aggreg_num )
		//printf( "Error in deriving chain in micelle.\n" );
}

void TriblockMicelle::printMicelle( FILE* stream ) {
	for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
		( *chain )->printChain( stream );
	}
}

// Multiple base beads?? Randomly chosen???
void TriblockMicelle::pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor ) {
  /*Bead* baseBeads[ 3 ];
  baseBeads[ 0 ] = this->chainList.at( 0 )->tail1->beadList[ 0 ];
  
  PECTriblock* triblock = this->chainList.at( this->chainList.size() - 1 );
  baseBeads[ 1 ] = triblock->tail2->beadList[ triblock->tail_length - 1 ];

  int randChain = rand() % this->chainList.size();
  triblock = this->chainList.at( randChain );
  int randBead = rand() % triblock->tail_length;
  baseBeads[ 2 ] = triblock->tail1->beadList[ randBead ];

  baseBeads[ 1 ]->pbcCorrectBeadInChain( baseBeads[ 0 ], box_length );
  baseBeads[ 2 ]->pbcCorrectBeadInChain( baseBeads[ 1 ], box_length );
  baseBeads[ 2 ]->pbcCorrectBeadInChain( baseBeads[ 0 ], box_length );
*/

  Bead* baseBead = this->chainList.at( 0 )->tail1->beadList[ 0 ];
  PECTriblock* triblock = NULL;
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    triblock = ( *chain );
    /*for ( int i = 0 ; i < 3; i++ ) {
      triblock->tail1->beadList[ 0 ]->pbcCorrectBeadInChain( baseBeads[ i ], box_length );
    }*/
    triblock->tail1->beadList[ 0 ]->pbcCorrectBeadInChain( baseBead, box_length, pbc_correction_factor );
    
    for ( idx i = 0 ; i < triblock->pec_block->length ; i ++ ) {
      ( triblock->pec_block->getBead( i ) )->pbcCorrectBeadInChain( triblock->tail1->beadList[ 0 ], box_length, pbc_correction_factor );
    }
    
    for ( idx i = 0 ; i < triblock->tail1->length ; i++ ) {
      ( triblock->tail1->getBead( i ) )->pbcCorrectBeadInChain( triblock->tail1->beadList[ 0 ], box_length, pbc_correction_factor );
      ( triblock->tail2->getBead( i ) )->pbcCorrectBeadInChain( triblock->tail1->beadList[ 0 ], box_length, pbc_correction_factor );
    }
  
  }
}

// Needs to be reviewed before I write tests, not a crucical function though
void TriblockMicelle::calcCenterOfMass( idx* box_length, const float& pbc_correction_factor ) {
  this->com->reset();
  this->pbcCorrectMicelle( box_length, pbc_correction_factor );
  
  PECTriblock* triblock = NULL;
  Bead *current = NULL;
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    triblock = ( *chain );
    
    for ( idx i = 0 ; i < triblock->pec_block->length ; i ++ ) {
      current = ( triblock->pec_block->getBead( i ) );
      this->com->addCoords( current->r );
    }
    
    for ( idx i = 0 ; i < triblock->tail1->length ; i++ ) {
      current = ( triblock->tail1->getBead( i ) );
      this->com->addCoords( current->r );
    
      current = ( triblock->tail2->getBead( i ) );
      this->com->addCoords( current->r );
    }
  
  }
  int ttlBeads = this->aggreg_num * triblock->chain_length;
  this->com->divideCoords( &ttlBeads );
}

void TriblockMicelle::unlink() {
  for ( auto chain = std::begin( this->chainList ); chain != std::end( this->chainList ); chain++ ) {
    *chain = NULL;
  }
}

TriblockMicelle::~TriblockMicelle() {
  this->unlink();
}

#if defined( TESTING )
#include <iostream>

int main() {

  const float pbc_correction_factor = 0.5f;

	std::ifstream infile( "bin_test.txt" );
	idx box_length = 36;
	// Make two triblocks from file
	std::cout << "Chain1: " << std::endl;
	PECTriblock* chain1 = new PECTriblock( 50, 4, 58, &infile, &box_length, pbc_correction_factor );
	chain1->printChain( stdout );
	std::cout << ( short ) chain1->chain_length << std::endl;

	std::cout << "Chain2: " << std::endl;
	PECTriblock* chain2 = new PECTriblock( 50, 4, 58, &infile, &box_length, pbc_correction_factor );
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

	if ( core1->micelle != micelle || core2->micelle != micelle )
		std::cout << "Fail" << std::endl;

	// Test chain functionality
	micelle->deriveChainList();

	if ( chain1->micelle != micelle || chain2->micelle != micelle )
		std::cout << "Fail" << std::endl;

	std::cout << "Micelle: " << std::endl;
	micelle->printMicelle( stdout );
  
  // Only test so far for pbcCorrectMicelle
  micelle->pbcCorrectMicelle( &box_length, pbc_correction_factor );
  std::cout << "Micelle (corrected): " << std::endl;
  micelle->printMicelle( stdout );

  // Test calcCenterOfMass
  // but its not that important of a function

  delete chain1;
  delete chain2;

  delete b1;
  delete b2;

  delete micelle;
  
	return 0;
}

#endif