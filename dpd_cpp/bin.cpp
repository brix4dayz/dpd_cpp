#include "bin.h"

Bin::Bin() {
	this->checked = false;
	this->grouped = false;
	this->num_chains = 0;
	this->core = NULL;
}

Bin::~Bin() {
	this->unlink();
}

void Bin::unlink() {
	this->core = NULL;
	this->frame = NULL;
}

void Bin::init( CopolymerMicelleFrame<PECTriblock*>* frame, idx size, idx i, idx j, idx k ) {
	this->frame = frame;
	this->size = size;
	this->i = i;
	this->j = j;
	this->k = k;
}

/*void Bin::addBlock( PolymerBlock* block ) {
	this->blockList.push_back( *block );
	this->num_chains++;
}*/

void Bin::addTail( HydrophobicTail* tail ) {
	tail->bin = this;
	this->tailList.push_back( tail );
	if ( tail->other->bin != this )
		this->num_chains++;
}

bool Bin::groupBins( Bin* other, idx* box_length, 
			float* micelle_cutoff ) {
	other->checked = true;
	for ( idx i = 0 ; i < this->tailList.size() ; i++ ) {
		for ( idx j = 0 ; j < other->tailList.size() ; j++ ) {
			if ( this->tailList[ i ]->com->getCorrectedDist( other->tailList[ j ]->com, box_length,
			 micelle_cutoff ) <= ( *micelle_cutoff ) * ( *micelle_cutoff ) ) {
				return true;
			}
		}
	}
	return false;
}

void Bin::printBin( FILE* stream ) {
	for ( auto tail = std::begin( this->tailList ) ; tail != std::end( this->tailList ) ; tail++ ) {
		( *tail )->printBlock( stream );
	}
}

// To test compile with -DTESTING
#if defined( TESTING )
#include <iostream>

int main() {

	std::ifstream infile( "bin_test.txt" );
	idx box_length = 36;
	// Make two triblocks from file
	std::cout << "Chain1: " << std::endl;
	PECTriblock* chain1 = new PECTriblock( 50, 4, 58, &infile, &box_length );
	chain1->printChain( stdout );
	std::cout << ( short ) chain1->chain_length << std::endl;	

	std::cout << "Chain2: " << std::endl;
	PECTriblock* chain2 = new PECTriblock( 50, 4, 58, &infile, &box_length );
	chain2->printChain( stdout );
	std::cout << ( short ) chain2->chain_length << std::endl;

	// Initialize two bins
	Bin b1;
	Bin b2;

	b1.init( NULL, 2, 0, 0, 0 );
	b2.init( NULL, 2, 0, 1, 0 );

	// Test add block/tail
	// Add the first tail of the chains to the bins
	b1.addTail( chain1->tail1 );
	b2.addTail( chain2->tail1 );

	if (chain2->tail1->bin == NULL )
		std::cout << "Fail" << std::endl;

	// Should print
	if ( chain1->tail1->bin == &b1 )
		std::cout << "YAY" << std::endl;

	// Test bin group
	float micelle_cutoff = 3.25;
	
	// Should print
	if ( b1.groupBins( &b2, &box_length, &micelle_cutoff ) && b2.checked )
		std::cout << "Hooray1!" << std::endl;

	// Change y in chain2's first tail so they're too far apart
	chain2->tail1->com->y += 3;

	// Should not print
	if ( b2.groupBins( &b1, &box_length, &micelle_cutoff ) && b1.checked )
		std::cout << "Hooray2!" << std::endl;

	b1.addTail( chain2->tail1 );
	// Should print
	if ( chain2->tail1->bin == &b1 )
		std::cout << "YAY2" << std::endl;

	std::cout << "Bin1: (so far) " << std::endl; 
	b1.printBin( stdout );

	b1.addTail( chain2->tail2 );
	chain2->determineConfiguration();
	if ( chain2->config == neither )
		std::cout << "Working..." << std::endl;

	chain1->determineConfiguration();
	if ( chain1->config == neither )
		std::cout << "Still works..." << std::endl;

	b1.addTail( chain1->tail2 );
	/*chain1->determineConfiguration(); // this test is wrong
	if ( chain1->config != petal )
		std::cout << "Fail" << std::endl;
*/
	b2.addTail( chain1->tail2 );
/*chain1->determineConfiguration(); // this test is wrong
	if ( chain1->config != stem )
		std::cout << "Fail" << std::endl;
	*/
	// 2 and 2
	std::cout << "Number of different chains in Bin1: " << (int) b1.num_chains << std::endl;
	std::cout << "Number of different chains in Bin2: " << (int) b2.num_chains << std::endl;

	delete chain1;

	delete chain2;

	return 0;
}

#endif