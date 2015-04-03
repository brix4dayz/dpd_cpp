#include "copolymerMicelleFrame.h"

CopolymerMicelleFrame::CopolymerMicelleFrame( unsigned int num_atoms, idx box_length,
	                                            idx chain_length, idx bin_size, float* micelle_cutoff ) {
	this->micelle_cutoff = micelle_cutoff;
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
	                            idx bin_size, float* micelle_cutoff, idx tail_length, idx pec_length ): 
															CopolymerMicelleFrame( num_atoms, box_length, chain_length, bin_size, micelle_cutoff ) {
  this->tail_length = tail_length;
  this->pec_length = pec_length;
  this->avg_agg_num_of_cores = 0;
  this->percent_stem_chains = 0.0f;
  this->percent_petal_chains = 0.0f;
  this->num_cores = 0;
  this->avg_distance_btwn_cores = 0.0;
}

TriblockFrame::TriblockFrame( unsigned int num_atoms, idx box_length, idx chain_length, 
															idx bin_size, float* micelle_cutoff, idx tail_length, idx pec_length, 
															std::ifstream* inFile ):
														  TriblockFrame( num_atoms, box_length, chain_length, 
                              bin_size, micelle_cutoff, tail_length, pec_length ) {
  PECTriblock* currentChain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    currentChain = new PECTriblock( this->pec_length, this->tail_length, this->chain_length,
                                    inFile, &box_length );
    this->addChain( currentChain ); 
  }
}

void TriblockFrame::compareBin( Bin* b, HydrophobicCore* core ) {
  Bin* current = NULL;
  int i, j, k;
  for ( int di = -1; di < 2; di++ ) {
    for ( int dj = -1; dj < 2; dj++ ) {
      for ( int dk = -1; dk < 2; dk++ ) {
        i = ( ( (int) b->i ) + di ) % this->num_bins;
        if ( i < 0 ) i += this->num_bins;
        j = ( ( (int) b->j ) + dj ) % this->num_bins;
        if ( j < 0 ) j += this->num_bins;
        k = ( ( (int) b->k ) + dk ) % this->num_bins;
        if ( k < 0 ) k += this->num_bins;
        //printf("%u %u %u\n", i, j, k );
        current = box[ i ][ j ][ k ];
        if ( !current->isEmpty() && !current->grouped && 
              b->groupBins( current, &(this->box_length), 
              this->micelle_cutoff ) ) {
          core->addBin( current );
          this->compareBin( current, core );
        }
      }
    }
  }
}

void TriblockFrame::deriveStems() {
  PECTriblock* chain = NULL;
  uintptr_t stemIdx = (uintptr_t) NULL;
  Stem* newStem = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = (PECTriblock*) this->chainList[ i ];
    stemIdx = chain->determineConfiguration();
    if ( stemIdx != (uintptr_t) NULL ) {
      // chain->printChain( stdout );  
      auto it = this->stems.find( stemIdx );
      if ( it == this->stems.end() ) {
        newStem = new Stem( chain->tail1->getCore(), chain->tail2->getCore() );
        if ( stemIdx != newStem->getKey() ) {
          fprintf( stdout, "Core mismatch.\n" );
          exit( 1 );
        }
        this->stems.insert( this->stems.begin(), std::pair< uintptr_t, Stem* >( stemIdx, newStem ) );
      } else {
        it->second->inc();
      }
    }
  }
}

void TriblockFrame::compareCore( HydrophobicCore* core, TriblockMicelle* micelle ) {
  for ( auto it = this->stems.begin(); it != this->stems.end(); it++ ) {
    if ( !it->second->grouped ) {
      if ( it->second->core1 == core ) {
        it->second->grouped = true;
        micelle->addCore( it->second->core2 );
        this->compareCore( it->second->core2, micelle );
      } else if ( it->second->core2 == core ) {
        it->second->grouped = true;
        micelle->addCore( it->second->core1 );
        this->compareCore( it->second->core1, micelle );
      }
    }
  }
}

void TriblockFrame::deriveMicelleList() {
  this->fillBins();

  // calculate mic_cutoff_sqrd in frame or even processor?

  //this->buildCorePool();

  Bin* current = NULL;
  HydrophobicCore* core = NULL;

  std::vector<HydrophobicCore*> corePool;

  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        current = box[ i ][ j ][ k ];
        if ( !current->isEmpty() && !current->grouped ) {
          core = new HydrophobicCore();
          core->addBin( current );
          this->compareBin( current, core );
          if ( core->num_tails > 1 )
            corePool.push_back( core );
          else {
            delete core;
            core = NULL;
          }
        }
      }
    }
  }

  //Test
  printf( "Number of cores: %lu\n", corePool.size() );
  int counter = 0;
  for ( auto it = corePool.begin() ; it != corePool.end(); it++) {
      char* filename = new char[ 10 ];
      sprintf( filename, "core%d.xyz", counter++ );
      FILE* fp = fopen( filename, "w" );
      core = *it;
      int numAtoms = core->num_tails * this->tail_length;
      fprintf( fp, "%d\nAtoms. Timestep: 4300\n", numAtoms );
      core->printCore( fp );
      delete filename;
      fclose( fp );
  }

  this->deriveStems();

  //Test
  printf( "Number of linked cores (aka stems): %lu\n", this->stems.size() );
  /*for ( auto it = this->stems.begin(); it != this->stems.end(); it++) {
    printf( "Stem %lu: %hu\n", it->first, it->second->count );
  }*/

  TriblockMicelle* micelle = NULL;

  if ( this->stems.size() == 0 ) {
    for ( auto it = corePool.begin(); it != corePool.end(); it++ ) {
      micelle = new TriblockMicelle();
      micelle->addCore( *it );

      micelle->deriveChainList();
      micelle->pbcCorrectMicelle( &(this->box_length) );

      this->micelleList.push_back( micelle );
    }
  } else {
    for ( auto it = this->stems.begin(); it != this->stems.end(); it++ ) {
      if ( !it->second->grouped ) {
        it->second->grouped = true;
        micelle = new TriblockMicelle();
        micelle->addCore( it->second->core1 );
        micelle->addCore( it->second->core2 );

        this->compareCore( it->second->core1, micelle );
        this->compareCore( it->second->core2, micelle );

        micelle->deriveChainList();
        micelle->pbcCorrectMicelle( &(this->box_length) );

        this->micelleList.push_back( micelle );
      }
  }

  }

  // Test
  printf( "Number of micelles: %lu\n", this->micelleList.size() );
  counter = 0;
  for ( auto it = this->micelleList.begin() ; it != this->micelleList.end(); it++) {
      char* filename = new char[ 12 ];
      sprintf( filename, "micelle%d.xyz", counter++ );
      FILE* fp = fopen( filename, "w" );
      micelle = *it;
      int numAtoms = micelle->chainList.size() * this->chain_length;
      fprintf( fp, "%d\nAtoms. Timestep: 4300\n", numAtoms );
      micelle->printMicelle( fp );
      delete filename;
      fclose( fp );
  }

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
  float micelle_cutoff = 1.25f;

	CopolymerMicelleFrame* frame = new CopolymerMicelleFrame( 1, 36, 1, 2, &micelle_cutoff );

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
                                             2, &micelle_cutoff, 4, 30, &triblockXYZ );

  printf( "Printing chains.\n" );
  tframe->printChains( stdout );

  tframe->deriveMicelleList();

  if ( !tframe->box[ 0 ][ 0 ][ 0 ]->isEmpty() )
    printf( "Fail empty\n" );

  //if ( tframe->areAllFilledBinsGrouped() )
  //  printf( "Fail bins grouped\n" );

  printf( "Printing bins.\n" );
  tframe->printBins( stdout );

  delete tframe;

  /////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}

#endif