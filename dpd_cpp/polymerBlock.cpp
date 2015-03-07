#include "polymerBlock.h"

PolymerBlock::PolymerBlock( CopolymerChain* chain, idx type, idx length )	: 
                            PolymerBlock() {
	this->bead_type = type;
	this->chain = chain;
	this->length = length;
	this->beadList = new Bead[ length ];
}

PolymerBlock::PolymerBlock( CopolymerChain*chain, idx type, idx length, 
                            std::ifstream* inFile, idx* box_length ) :
                            PolymerBlock( chain, type, length ) {
	std::string line;
	for (idx i = 0; i < length; i++ ) {
		std::getline( *inFile, line );
		Bead *b = new Bead( line );
		this->addBead( b );
	}

	this->calcCenterOfMass( box_length );
}

PolymerBlock::PolymerBlock( CopolymerChain* chain, idx type, 
	                          idx length, DirVect* d, 
	                          idx* box_length, PosVect* r, unsigned int* idTracker ) :
                            PolymerBlock( chain, type, length ) {
  Bead* b = new Bead( r, type, idTracker );
	this->addBead( b );

	for (idx i = 1; i < length; i++ ) {
		PosVect* rr = new PosVect( this->beadList[ i - 1 ].r, d );
		b = new Bead( rr, type, idTracker);
		this->addBead( b );
	}

	this->calcCenterOfMass( box_length );
} 

PolymerBlock::PolymerBlock() {
	this->com = new PosVect();
	this->cursor = 0;
}

PolymerBlock::~PolymerBlock() {}

void PolymerBlock::calcCenterOfMass( idx* box_length ) {
	Bead* b;
	this->com->reset();
	for ( idx i = 0; i < this->length; i++ ) {
		b = this->beadList + i;
		b->pbcCorrectBeadInChain( this->beadList, box_length );
    this->com->addCoords( b->r );
	}
  int block_length = this->length;
  this->com->divideCoords( &block_length );
}

bool PolymerBlock::addBead( Bead *bead ) {
	if (this->cursor >= this->length || bead->type != this->bead_type )
		return false;
	this->beadList[ this->cursor ] = *bead;
	this->cursor++;
	return true;
}

Bead* PolymerBlock::getBead( idx idx ) {
	if ( idx < 0 || idx >= this->length )
		return NULL;
	else
		return this->beadList + idx;
}

void PolymerBlock::printBlock( FILE *stream ) {
	for (idx i = 0; i < this->length; i++ ) {
		this->beadList[ i ].printBead( stream );
	}
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length ) : 
																	PolymerBlock( chain, HYDROPHOBIC, length ) {
	this->bin = NULL;
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length, 
	                                std::ifstream* inFile, idx* box_length ) : 
                                  PolymerBlock( chain, HYDROPHOBIC, length,
	                                inFile, box_length ) {
  this->bin = NULL;
}

HydrophobicTail::HydrophobicTail( CopolymerChain* chain, idx length, 
	                                DirVect* d, idx* box_length, 
	                                PosVect* r, unsigned int* idTracker ) : PolymerBlock( chain,
	                                HYDROPHOBIC, length, d, 
	                                box_length, r, idTracker ) {
	this->bin = NULL;
}

HydrophobicTail::HydrophobicTail() : PolymerBlock() {
	this->bead_type = HYDROPHOBIC;
}

HydrophobicTail::~HydrophobicTail() {}

#if defined( TESTING )

int main() {

	//Setup test
	Bead b1(-1, 2, -1, 1);
	Bead b2(1, -2, 1, 1);
	Bead b3(3, 3, 3, 1);
	Bead b4(-3, -3, -3, 1);
	Bead b5(5, -1, 2, 1);

	Bead b6(0, 0, 0, 2);

	idx test_box_length = 15;

	PolymerBlock hydrophobic_block(NULL, 1, 5);

	// Test add
	if ( !hydrophobic_block.addBead( &b1 ) || hydrophobic_block.cursor != 1 )
		return 1;

	if ( hydrophobic_block.addBead( &b6 ) || hydrophobic_block.cursor != 1 )
		return 1;

	if ( !hydrophobic_block.addBead( &b2 ) || hydrophobic_block.cursor != 2 )
		return 1;

	if ( !hydrophobic_block.addBead( &b3 ) || hydrophobic_block.cursor != 3 )
		return 1;

	if ( !hydrophobic_block.addBead( &b4 ) || hydrophobic_block.cursor != 4 )
		return 1;

	if ( !hydrophobic_block.addBead( &b5 ) || hydrophobic_block.cursor != 5 )
		return 1;

	// Test com
	hydrophobic_block.calcCenterOfMass( &test_box_length );

	hydrophobic_block.com->print( stdout );

	// Last test for add
	if ( hydrophobic_block.addBead( &b1 ) || hydrophobic_block.cursor != 5 )
		printf("Failed\n");


	// Test get
	hydrophobic_block.getBead( 0 )->printBead( stdout );

	hydrophobic_block.getBead( 4 )->printBead( stdout );

	if ( hydrophobic_block.getBead( 6 ) )
		printf("Failed\n");

	// TEST CHILD CLASS HYDROPHOBIC_TAIL
	HydrophobicTail hydrophobic_block1(NULL, 5);

	// Test add
	if ( !hydrophobic_block1.addBead( &b1 ) || hydrophobic_block1.cursor != 1 )
		return 1;

	if ( hydrophobic_block1.addBead( &b6 ) || hydrophobic_block1.cursor != 1 )
		return 1;

	if ( !hydrophobic_block1.addBead( &b2 ) || hydrophobic_block1.cursor != 2 )
		return 1;

	if ( !hydrophobic_block1.addBead( &b3 ) || hydrophobic_block1.cursor != 3 )
		return 1;

	if ( !hydrophobic_block1.addBead( &b4 ) || hydrophobic_block1.cursor != 4 )
		return 1;

	if ( !hydrophobic_block1.addBead( &b5 ) || hydrophobic_block1.cursor != 5 )
		return 1;

	// Test com
	hydrophobic_block1.calcCenterOfMass( &test_box_length );

	hydrophobic_block1.com->print( stdout );

	// Last test for add
	if ( hydrophobic_block1.addBead( &b1 ) || hydrophobic_block1.cursor != 5 )
		printf("Failed\n");

	// Test get
	hydrophobic_block1.getBead( 0 )->printBead( stdout );

	hydrophobic_block1.getBead( 4 )->printBead( stdout );

	if ( hydrophobic_block1.getBead( 6 ) )
		printf("Failed\n");


	// Test file read
	idx box_length = 36;
	std::ifstream infile("bead_test.txt");

	HydrophobicTail test( NULL, 4, &infile, &box_length );
	test.com->print( stdout );

	test.printBlock( stdout );

	return 0;
}

#endif