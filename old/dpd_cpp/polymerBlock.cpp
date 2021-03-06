#include "bin.h"
#include <map>

const float default_correction_factor = DEFAULT_PBC;

PolymerBlock::PolymerBlock() {
  this->com = new PosVect();
  this->cursor = 0;
}

PolymerBlock::PolymerBlock( CopolymerChain* chain, byte type, idx length )	:
                            PolymerBlock() {
	this->bead_type = type;
	this->chain = chain;
	this->length = length;
	this->beadList = new Bead*[ length ];
}

PolymerBlock::PolymerBlock( CopolymerChain*chain, byte type, idx length, 
                            std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor ) :
                            PolymerBlock( chain, type, length ) {
	std::string line;
	for (idx i = 0; i < length; i++ ) {
		std::getline( *inFile, line );
		Bead *b = new Bead( line );
		this->addBead( b );
	}

	this->calcCenterOfMass( box_length, pbc_correction_factor );
}

PolymerBlock::PolymerBlock( CopolymerChain* chain, byte type, 
	                          idx length, DirVect* d, 
	                          idx* box_length, PosVect* r, unsigned int* idTracker,
	                          unsigned int mol_id ) :
                            PolymerBlock( chain, type, length ) {
  Bead* b = new Bead( r, type, idTracker, mol_id );
	this->addBead( b );

	for (idx i = 1; i < length; i++ ) {
		PosVect* rr = new PosVect( this->beadList[ i - 1 ]->r, d );
		b = new Bead( rr, type, idTracker, mol_id );
		this->addBead( b );
	}

	this->calcCenterOfMass( box_length, default_correction_factor );
}

PolymerBlock::~PolymerBlock() {
	if ( this->beadList ) {
		for ( idx i = 0; i < this->length; i++ ) {
    	delete this->beadList[ i ];
  	}	
	}
  delete[] this->beadList;
  this->beadList = NULL;
  delete this->com;
  this->com = NULL;
  this->unlink();
}

void PolymerBlock::unlink() {
  if ( this->beadList ) {
  	for ( idx i = 0; i < this->length; i++ ) {
    	this->beadList[ i ] = NULL;
  	}	
  }
  this->chain = NULL;
}

void PolymerBlock::calcCenterOfMass( idx* box_length, const float& pbc_correction_factor ) {
	Bead* b;
	this->com->reset();
	for ( idx i = 0; i < this->length; i++ ) {
		b = this->beadList[ i ];
		b->pbcCorrectBeadInChain( this->beadList[ 0 ], box_length, pbc_correction_factor );
    this->com->addCoords( b->r );
	}
  int block_length = this->length;
  this->com->divideCoords( &block_length );
}

bool PolymerBlock::addBead( Bead *bead ) {
	if (this->cursor >= this->length )
		return false;
	this->beadList[ this->cursor ] = bead;
	this->cursor++;
	return true;
}

Bead* PolymerBlock::getBead( idx i ) {
	if ( i < 0 || i >= this->length )
		return NULL;
	else
		return this->beadList[ i ];
}

void PolymerBlock::printBlock( FILE *stream ) {
	for (idx i = 0; i < this->length; i++ ) {
		this->beadList[ i ]->printBead( stream );
	}
}

void PolymerBlock::printData( FILE* stream ) {
		for (idx i = 0; i < this->length; i++ ) {
		this->beadList[ i ]->printData( stream );
	}
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length ) : 
																	PolymerBlock( chain, HYDROPHOBIC, length ) {
	this->bin = NULL;
	this->other = NULL;
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length, 
	                                std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor ) :
                                  PolymerBlock( chain, HYDROPHOBIC, length,
	                                inFile, box_length, pbc_correction_factor ) {
  this->bin = NULL;
  this->other = NULL;
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length, 
	                                DirVect* d, idx* box_length,
	                                PosVect* r, unsigned int* idTracker,
	                                unsigned int mol_id ) :
	                                PolymerBlock( chain,
	                                HYDROPHOBIC, length, d, 
	                                box_length, r, idTracker, mol_id ) {
	this->bin = NULL;
	this->other = NULL;
}

HydrophobicTail::HydrophobicTail() : PolymerBlock() {
	this->bead_type = HYDROPHOBIC;
}

void HydrophobicTail::unlink() {
  this->other = NULL;
  this->bin = NULL;
}

HydrophobicCore* HydrophobicTail::getCore() {
	if ( this->bin && this->bin->core )
		return this->bin->core;
	return NULL;
}

HydrophobicTail::~HydrophobicTail() {
  this->unlink();
  // Don't delete the other tail or Bin, let them take care of themselves
}

ChargedBlock::ChargedBlock( CopolymerChain *chain, byte type, idx length, 
										std::ifstream* inFile, idx* box_length, const float& pbc_correction_factor ) :
										PolymerBlock( chain , type, length, inFile, box_length, pbc_correction_factor ) {}

bool HydrophobicTail::addBead( Bead* bead ) {
	if ( this->cursor >= this->length || this->bead_type != bead->type )
		return false;
	this->beadList[ this->cursor ] = bead;
	this->cursor++;
	return true;
}

ChargedBlock::ChargedBlock( CopolymerChain* chain, byte type,
              							idx length, DirVect* d,
               							idx* box_length, PosVect* r, unsigned int* id,
              	 						unsigned int mol_id ) : PolymerBlock( chain, 
              	 						type, length, d, box_length, r, id, mol_id ) {}
     
ChargedBlock::ChargedBlock( CopolymerChain *chain, byte type, idx length ) : 
														PolymerBlock( chain, type, length ) {}
 
ChargedBlock::ChargedBlock() : PolymerBlock() {}

ChargedBlock::ChargedBlock( CopolymerChain* chain, byte type,
              							idx length, DirVect* d,
               							idx* box_length, PosVect* r, unsigned int* id,
              	 						unsigned int mol_id, byte& uncharged_type, idx& num_uncharged,
              	 						IntegerDice<idx>& chargeDice ) : ChargedBlock( chain, type,
              																															length, d, 
              																															box_length,r, id,
              	 																														mol_id )  {
  std::map< idx, idx > unchargedBeads;
  idx unchargedIdx;
  while ( ( (idx) unchargedBeads.size() ) != num_uncharged ) {
    #if defined(TEST)
    unchargedIdx = rand() % this->length;
    #else
    unchargedIdx = chargeDice.roll();
    #endif
    if ( unchargedBeads.find( unchargedIdx ) == unchargedBeads.end() ) {
      unchargedBeads.insert( std::pair< idx, idx >( unchargedIdx, unchargedIdx ) );
      this->getBead( unchargedIdx )->type = uncharged_type;
    }
  }
}

void PolymerBlock::color( byte type ) {
	for ( idx i = 0; i < this->length; i++ ) {
		this->getBead( i )->type = type;
	}
}

#if defined( TESTING )
#include <iostream>

int main() {

	float pbc_correction_factor = 0.5f;

	//Setup test
	Bead* b1 = new Bead(-1, 2, -1, 1);
	Bead* b2 = new Bead(1, -2, 1, 1);
	Bead* b3 = new Bead(3, 3, 3, 1);
	Bead* b4 = new Bead(-3, -3, -3, 1);
	Bead* b5 = new Bead(5, -1, 2, 1);

	Bead* b6 = new Bead(0.0, 0.0, 0.0, 2);

	/*Bead* bb1 = new Bead(-1, 2, -1, 1);
	Bead* bb2 = new Bead(1, -2, 1, 1);
	Bead* bb3 = new Bead(3, 3, 3, 1);
	Bead* bb4 = new Bead(-3, -3, -3, 1);
	Bead* bb5 = new Bead(5, -1, 2, 1);

	Bead* bb6 = new Bead(0.0, 0.0, 0.0, 2);
*/
	idx test_box_length = 15;

	PolymerBlock hydrophobic_block(NULL, 1, 5);

	// Test add
	if ( !hydrophobic_block.addBead( b1 ) || hydrophobic_block.cursor != 1 )
		return 1;

	if ( !hydrophobic_block.addBead( b2 ) || hydrophobic_block.cursor != 2 )
		return 1;

	if ( !hydrophobic_block.addBead( b3 ) || hydrophobic_block.cursor != 3 )
		return 1;

	if ( !hydrophobic_block.addBead( b4 ) || hydrophobic_block.cursor != 4 )
		return 1;

	if ( !hydrophobic_block.addBead( b5 ) || hydrophobic_block.cursor != 5 )
		return 1;

	// Test com
	hydrophobic_block.calcCenterOfMass( &test_box_length, pbc_correction_factor );

	hydrophobic_block.com->print( stdout );

	// Last test for add
	if ( hydrophobic_block.addBead( b1 ) || hydrophobic_block.cursor != 5 )
		printf("Failed\n");


	// Test get
	hydrophobic_block.getBead( 0 )->printBead( stdout );

	hydrophobic_block.getBead( 4 )->printBead( stdout );

	if ( hydrophobic_block.getBead( 6 ) )
		printf("Failed\n");

	// TEST CHILD CLASS HYDROPHOBIC_TAIL
	HydrophobicTail hydrophobic_block1(NULL, 5);

	// Test add
	if ( !hydrophobic_block1.addBead( b1 ) || hydrophobic_block1.cursor != 1 )
		return 1;

	if ( hydrophobic_block1.addBead( b6 ) || hydrophobic_block1.cursor != 1 )
		return 1;

	if ( !hydrophobic_block1.addBead( b2 ) || hydrophobic_block1.cursor != 2 )
		return 1;

	if ( !hydrophobic_block1.addBead( b3 ) || hydrophobic_block1.cursor != 3 )
		return 1;

	if ( !hydrophobic_block1.addBead( b4 ) || hydrophobic_block1.cursor != 4 )
		return 1;

	if ( !hydrophobic_block1.addBead( b5 ) || hydrophobic_block1.cursor != 5 )
		return 1;

	// Test com
	hydrophobic_block1.calcCenterOfMass( &test_box_length, pbc_correction_factor );

	hydrophobic_block1.com->print( stdout );

	// Last test for add
	if ( hydrophobic_block1.addBead( b1 ) || hydrophobic_block1.cursor != 5 )
		printf("Failed\n");

	// Test get
	hydrophobic_block1.getBead( 0 )->printBead( stdout );

	hydrophobic_block1.getBead( 4 )->printBead( stdout );

	if ( hydrophobic_block1.getBead( 6 ) )
		printf("Failed\n");


	// Test file read
	idx box_length = 36;
	std::ifstream infile("bead_test.txt");

	HydrophobicTail test( NULL, 4, &infile, &box_length, pbc_correction_factor );
	test.com->print( stdout );

	test.printBlock( stdout );

	infile.close();

	delete b6;

	hydrophobic_block.unlink();

	return 0;
}

#endif