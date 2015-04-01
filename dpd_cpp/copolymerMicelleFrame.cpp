#include "copolymerMicelleFrame.h"

CopolymerMicelleFrame::CopolymerMicelleFrame( unsigned int num_atoms, idx box_length,
	                                            idx chain_length, idx bin_size ) {
	this->chainCursor = 0;
	this->num_atoms = num_atoms;
	this->box_length = box_length;
	this->chain_length = chain_length;
	this->num_chains = num_atoms / chain_length;
	this->chainList = new CopolymerChain*[ this->num_chains ];
	this->bin_size = bin_size;
	this->num_bins = box_length / bin_size;
	this->box = new Bin***[ this->num_bins ];
	for ( idx i = 0; i < this->num_bins; i++ ) {
		this->box[ i ] = new Bin**[ this->num_bins ];
		for ( idx j = 0; j < this->num_bins; j++ ) {
			this->box[ i ][ j ] = new Bin*[ this->num_bins ];
			for ( idx k = 0; k < this->num_bins; k++ ) {
				Bin *temp = new Bin();
				temp->init( this, bin_size, i, j, k );
				this->box[ i ][ j ][ k ] = temp;
			}
		}
	} 
}

CopolymerMicelleFrame::~CopolymerMicelleFrame() {
	if ( this->chainList ) {
		for ( unsigned short i = 0; i < this->num_chains; i++ ) {
			delete this->chainList[ i ];	
		}
		delete[] this->chainList;
	}

	if ( this->box ) {
		for ( idx i = 0; i < this->num_bins; i++ ) {
			for ( idx j = 0; j < this->num_bins; j++ ) {
				for ( idx k = 0; k < this->num_bins; k++ ) {
					delete this->box[ i ][ j ][ k ];
				}
				delete[] this->box[ i ][ j ];
			}
			delete[] this->box[ i ];
		}
		delete[] this->box;
	}

	this->unlink();
}

/*Bin CopolymerMicelleFrame::binBlock( PolymerBlock* block ) {
	idx i = coordToBin( block->com->x );
	idx j = coordToBin( block->com->y );
	idx k = coordToBin( block->com->z );
	this->box[ i ][ j ][ k ].addBlock( block );
	return this->box[ i ][ j ][ k ];
}*/

Bin* CopolymerMicelleFrame::binBlock( HydrophobicTail* tail ) {
	idx i = coordToBin( tail->com->x );
	idx j = coordToBin( tail->com->y );
	idx k = coordToBin( tail->com->z );
	this->box[ i ][ j ][ k ]->addTail( tail );
	return this->box[ i ][ j ][ k ];
}

idx CopolymerMicelleFrame::coordToBin( double coord ) {
	if ( coord < 0 )
		coord += this->box_length;
	else if ( ( ( idx ) coord ) >= this->box_length )
		coord -= this->box_length;
	return ( ( idx ) ( coord / this->bin_size ) );
}


void CopolymerMicelleFrame::addChain( CopolymerChain* chain ) {
  if ( this->chainCursor >= this->num_chains ) {
    fprintf( stdout, "Error, adding too many chains." );
    exit( 1 );
  }
  this->chainList[ this->chainCursor ] = chain;
  this->chainCursor++;
}

void CopolymerMicelleFrame::unlink() {
	this->chainList = NULL;
	this->box = NULL;
}

// Triblock
TriblockFrame::TriblockFrame( unsigned int num_atoms, idx box_length, idx chain_length, 
	                            idx bin_size, idx tail_length, idx pec_length ): 
															CopolymerMicelleFrame( num_atoms, box_length, chain_length, bin_size ) {
  this->tail_length = tail_length;
  this->pec_length = pec_length;
  this->avg_agg_num_of_cores = 0;
  this->percent_stem_chains = 0.0f;
  this->percent_petal_chains = 0.0f;
  this->num_cores = 0;
  this->avg_distance_btwn_cores = 0.0;
}

TriblockFrame::TriblockFrame( unsigned int num_atoms, idx box_length, idx chain_length, 
															idx bin_size, idx tail_length, idx pec_length, 
															std::ifstream* inFile ):
														  TriblockFrame( num_atoms, box_length, chain_length, 
                              bin_size, tail_length, pec_length ) {
  PECTriblock* currentChain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    currentChain = new PECTriblock( this->pec_length, this->tail_length, this->chain_length,
                                    inFile, &box_length );
    this->addChain( currentChain ); 
  }
}

void TriblockFrame::deriveMicelleList() {
  this->fillBins();

  // calculate mic_cutoff_sqrd in frame or even processor?

  //this->buildCorePool();

  // this->deriveEdgeList();

  // Determine micelles from edgeList and corePool and maybe edgeQueue?

  /////////////////////////////////////////////////////////////////////
  // Eh old stuff

  std::queue<Bin*> ungroupedBinsQ;
  Bin* current = NULL;

  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        current = box[ i ][ j ][ k ];
        if ( !current->isEmpty() && !current->grouped )
          ungroupedBinsQ.push(current);
      }
    }
  }

  // Test
  printf("Ungrouped q size: %lu\n", ungroupedBinsQ.size() );
  if ( ungroupedBinsQ.size() != this->numFilledBinsArentGrouped() ) {
    printf("Fail.\n");
  }

  std::queue<HydrophobicCore*> coreList;
  HydrophobicCore* newCore = NULL;
  Bin* other = NULL;

  /*while ( !ungroupedBinsQ.empty() ) {
    
  }*/
  /////////////////////////////////////////////////////////////////////

}

bool TriblockFrame::areAllFilledBinsGrouped() {
  Bin* current = NULL;
  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        current = this->box[ i ][ j ][ k ];
        if ( !current->isEmpty() && !current->grouped )
          return false;
      }
    }
  }
  return true;
}

int TriblockFrame::numFilledBinsArentGrouped() {
  Bin* current = NULL;
  int counter = 0;
  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        current = this->box[ i ][ j ][ k ];
        if ( !current->isEmpty() && !current->grouped )
          counter++;
      }
    }
  }
  return counter;
}

void TriblockFrame::fillBins() {
  Bin* current = NULL;
  PECTriblock* chain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = ( PECTriblock* ) this->chainList[ i ];
    current = this->binBlock( chain->tail1 );
    if ( !current ) {
      fprintf( stdout, "Tail1 in chain %d outside of bins.\n", i );
      exit( 1 );
    }
    current = NULL;
    current = this->binBlock( chain->tail2 );
    if ( !current ) {
      fprintf( stdout, "Tail2 in chain %d outside of bins.\n", i );
      exit( 1 );
    }
    current = NULL;
  }
}

void TriblockFrame::process() {

}

TriblockFrame::~TriblockFrame() {
  for ( auto micelle = std::begin( this->micelleList ); micelle != std::end( this->micelleList ); micelle++ ) {
    delete *micelle;
  }
}

void TriblockFrame::printChains( FILE* fp ) {
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
      fprintf( fp, "printing chain %d\n", i );
      this->chainList[ i ]->printChain( fp );
  }
}

void TriblockFrame::printBins( FILE* fp ) {
  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        fprintf( fp, "printing bin %d %d %d\n", (int) i, (int) j, (int) k );
        this->box[ i ][ j ][ k ]->printBin( fp );
      }
    }    
  }
}

// Testing

#if defined( TESTING )
#include <iostream> //for TESTING
#include <cstdlib>

int main() {
	CopolymerMicelleFrame* frame = new CopolymerMicelleFrame( 1, 36, 1, 2 );

	std::ifstream infile( "bead_test.txt" );
	PECTriblock* chain = new PECTriblock( 50, 4, 58, &infile, &( frame->box_length ) );

	frame->addChain( (CopolymerChain*) chain );

	std::cout << (short) chain->chain_length << std::endl;

	std::cout << "Tail1 com: ";
	chain->tail1->com->print( stdout );

	std::cout << "Tail2 com: ";
	chain->tail2->com->print( stdout );

	std::cout << "PEC com: ";
	chain->pec_block->com->print( stdout );

	Bin* b1 = frame->binBlock( chain->tail1 );
	// for testing
	std::cout << ( int ) b1->i << " " << ( int ) b1->j << " " << ( int ) b1->k << std::endl;

	Bin* b2 = frame->binBlock( chain->tail2 );
	// for testing
	std::cout << ( int ) b2->i << " " << ( int ) b2->j << " " << ( int ) b2->k << std::endl;	

	b1 = NULL;

	b2 = NULL;

  infile.close();

	delete frame;

  /////////////////////////////////////////////////////////////////////////////////////////
  //Triblock frame test

  std::ifstream triblockXYZ("last.xyz"); 
  std::string line;
  std::getline(triblockXYZ, line);

  unsigned int num_atoms = atoi( line.c_str() );
  std::cout << line << " " << num_atoms << std::endl;


  std::getline(triblockXYZ, line);

  TriblockFrame* tframe = new TriblockFrame( num_atoms, 36, 38, 
                                             2, 4, 30, &triblockXYZ );

  printf( "Printing chains.\n" );
  tframe->printChains( stdout );

  tframe->deriveMicelleList();

  if ( !tframe->box[ 0 ][ 0 ][ 0 ]->isEmpty() )
    printf( "Fail empty\n" );

  if ( tframe->areAllFilledBinsGrouped() )
    printf( "Fail bins grouped\n" );

  printf( "Printing bins.\n" );
  tframe->printBins( stdout );

  delete tframe;

  /////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}

#endif