#include "tadMicelle.h"
#include <iostream>

/******************************* EndpointChain ******************************************/

EndpointChain::EndpointChain() {}

/**
 * Constructs chain from input file.
 */
EndpointChain::EndpointChain(byte numTails, std::ifstream& inFile,
 idx* box_length, const float& pbc_correction_factor) {
  this->numTails = numTails;
  this->tails = new HydrophobicTail*[numTails];
  this->chain_length = numTails;
  this->com = new PosVect();
  this->micelle = NULL;

  for (byte i = 0; i < numTails; i++) {
    // builds tail of 1 bead from reading input file
    this->tails[i] = new HydrophobicTail(this, 1, &inFile, box_length, pbc_correction_factor);
  }

  this->linkTails();
}

/**
 * Destroys contents of chain.
 */
EndpointChain::~EndpointChain() {
  for (byte i = 0; i < numTails; i++) {
    delete this->tails[i];
  }
  this->unlink();
  delete this->tails;
}

/**
 * Prints chain contents to file.
 */
void EndpointChain::printChain( FILE* stream ) {
  for (byte i = 0; i < numTails; i++) {
    this->tails[i]->printBlock( stream );
  }
}
 
/**
 * Remove references.
 */   
void EndpointChain::unlink() {
  for (byte i = 0; i < numTails; i++) {
    this->tails[i] = NULL;
  }
}

/**
 * Link each tail to the previous one in list.
 * The first tail links to the last.
 */
void EndpointChain::linkTails() {
  for (byte i = 1; i < numTails; i++) {
    this->tails[i]->other = this->tails[i-1];
  }

  this->tails[0]->other = this->tails[numTails - 1];
}


/**
 * Determines if chain is stem or petal.
 */
uintptr_t EndpointChain::determineConfiguration() {
  if (numTails < 2) {
    this->config = neither; 
    return (uintptr_t) NULL;
  } else {

    HydrophobicCore* c1;
    HydrophobicCore* c2;

    for (byte i = 0; i < numTails; i++) {
      c1 = tails[i]->getCore();
      c2 = tails[(i+1)%numTails]->getCore();

      if (!c1 || !c2) {
        this->config = neither;
        return (uintptr_t) NULL;
      } else if (c1 == c2) {
        this->config = petal;
        return (uintptr_t) NULL;
      }
    }

    this->config = stem;
    return Stem::hashCores(c1, c2);
  }
}

/******************************* EndpointChain ******************************************/




/******************************* TadMicelle ******************************************/

TadMicelle::TadMicelle() : Micelle() {}

TadMicelle::TadMicelle( DPDFrame<CopolymerChain>* frame ): Micelle( frame ) {}

void TadMicelle::addChain( EndpointChain* chain ) {
  this->chainList.push_back( chain );
  chain->micelle = this;
}

void TadMicelle::deriveChainList() {
  EndpointChain* chain;
  HydrophobicCore* c;
  //printf("%lu\n", this->coreList.size());
  for ( auto core = std::begin( this->coreList ) ; core != std::end( this->coreList ) ; core++ ) {
    for ( auto bin = std::begin( ( *core )->binList ) ; bin != std::end( ( *core )->binList ) ; bin++ ) {
      for ( auto tail = std::begin( ( *bin )->tailList ) ; tail != std::end( ( *bin )->tailList ) ; tail++ ) {
        chain = ( EndpointChain* ) ( *tail )->chain;
        if ( chain && !chain->micelle) {
          this->addChain( chain );

          c = chain->tails[0]->getCore();
          if (c) {
            c->aggregation_num++;
          }

          for (byte i = 1; i < chain->numTails; i++) {
            c = chain->tails[i]->getCore();
            if (c && c != chain->tails[i-1]->getCore()) {
              c->aggregation_num++;
            }
          }       
        }

      }
    }
  }
  //printf("%lu\n", this->chainList.size());
  //if ( this->chainList.size() != this->aggreg_num )
    //printf( "Error in deriving chain in micelle.\n" );
}

void TadMicelle::printMicelle( FILE* stream ) {
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    ( *chain )->printChain( stream );
  }
}

void TadMicelle::pbcCorrectMicelle( idx* box_length, const float& pbc_correction_factor ) {
  if (chainList.size() > 0) {
    Bead* baseBead = this->chainList.at( 0 )->tails[0]->beadList[ 0 ];
    Bead* currentBase;
    EndpointChain* poly = NULL;
    for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
      poly = ( *chain );

      currentBase = baseBead;
      for (byte i = 0; i < poly->numTails; i++) {
          if ( i > 0 )
            currentBase = poly->tails[i-1]->beadList[0];
          poly->tails[i]->getBead(0)->pbcCorrectBeadInChain(currentBase, box_length, pbc_correction_factor);
      }
    
    }
  }
}

void TadMicelle::calcCenterOfMass( idx* box_length, const float& pbc_correction_factor ) {
  this->com->reset();
  this->pbcCorrectMicelle( box_length, pbc_correction_factor );
  
  EndpointChain* poly = NULL;
  for ( auto chain = std::begin( this->chainList ) ; chain != std::end( this->chainList ) ; chain++ ) {
    poly = ( *chain );
    
    for (byte i = 0; i < poly->numTails; i++) {
        this->com->addCoords(poly->tails[i]->getBead(0)->r);
    }
  }
  int ttlBeads = this->aggreg_num * poly->chain_length;
  this->com->divideCoords( &ttlBeads );
}

void TadMicelle::unlink() {
  for ( auto chain = std::begin( this->chainList ); chain != std::end( this->chainList ); chain++ ) {
    *chain = NULL;
  }
}

TadMicelle::~TadMicelle() {
  this->unlink();
}

/******************************* TadMicelle ******************************************/


/******************************* TadFrame ******************************************/

TadFrame::TadFrame( unsigned int num_atoms, idx box_length, idx chain_length, 
                              idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor):
                              CopolymerMicelleFrame( num_atoms, box_length, chain_length, bin_size, micelle_cutoff, pbc_correction_factor ) {
  this->avg_agg_number = 0.0f;
  this->percent_stem_chains = 0.0f;
  this->percent_petal_chains = 0.0f;
  this->num_cores = 0;
  this->avg_distance_btwn_cores = 0.0;
}

TadFrame::TadFrame( unsigned int num_atoms, idx box_length, idx chain_length, 
                              idx bin_size, float* micelle_cutoff, const float& pbc_correction_factor, std::ifstream* inFile ):
                              TadFrame( num_atoms, box_length, chain_length, 
                              bin_size, micelle_cutoff, pbc_correction_factor ) {
  EndpointChain* currentChain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    currentChain = new EndpointChain(chain_length, *inFile, 
                          &box_length, pbc_correction_factor);
    this->addChain( currentChain ); 
  }
}

void TadFrame::compareBin( Bin* b, HydrophobicCore* core ) {
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

void TadFrame::deriveStems() {
  EndpointChain* chain = NULL;
  Stem* newStem = NULL;
  Stem* oldStem = NULL;
  HydrophobicCore* c1 = NULL;
  HydrophobicCore* c2 = NULL;
  uintptr_t stemIdx = (uintptr_t) NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = (EndpointChain*) this->chainList[ i ];
    stemIdx = chain->determineConfiguration();
    if ( stemIdx != (uintptr_t) NULL ) {  
      auto it = this->stems.find( stemIdx );
      if ( it == this->stems.end() ) {
        std::vector< Stem* > stemList;
        auto inserted = this->stems.insert( this->stems.begin(), std::pair< uintptr_t, std::vector< Stem* > >( stemIdx, stemList ) );
        for (byte i = 0; i < chain->numTails; i++) {
          c1 = chain->tails[i]->getCore();
          c2 = chain->tails[(i+1)%chain->numTails]->getCore();
          newStem = new Stem(c1, c2);
          inserted->second.push_back( newStem );
        }
      } 
      // else {
      //   bool found = false;
      //   for ( auto stems = it->second.begin(); stems != it->second.end(); stems++ ) {
      //     oldStem = *stems;
      //     if ( ( oldStem->core1 == chain->tail1->getCore() && oldStem->core2 == chain->tail2->getCore() ) || 
      //        ( oldStem->core2 == chain->tail1->getCore() && oldStem->core1 == chain->tail2->getCore() ) ) {
      //       oldStem->inc();
      //       found = true;
      //       break;
      //     }

      //   }
      //   if ( !found ) {
      //     newStem = new Stem( chain->tail1->getCore(), chain->tail2->getCore() );
      //     it->second.push_back( newStem );
      //   }
      // }
    }
  }
}

void TadFrame::compareCore( HydrophobicCore* core, TadMicelle* micelle ) {
  Stem* currentStem = NULL;
  for ( auto it = this->stems.begin(); it != this->stems.end(); it++ ) {
    for ( auto stem = it->second.begin(); stem != it->second.end(); stem++ ) {
      currentStem = *stem;
      if ( !currentStem->grouped ) {
        if ( currentStem->core1 == core ) {
          currentStem->grouped = true;
          micelle->addCore( currentStem->core2 );
          this->compareCore( currentStem->core2, micelle );
        } else if ( currentStem->core2 == core ) {
          currentStem->grouped = true;
          micelle->addCore( currentStem->core1 );
          this->compareCore( currentStem->core1, micelle );
        }
      }
    }  
  }
}

void TadFrame::deriveMicelleList() {
  this->fillBins();

  // calculate mic_cutoff_sqrd in frame or even processor?

  //this->buildCorePool();

  Bin* current = NULL;
  HydrophobicCore* core = NULL;

  std::vector< HydrophobicCore* > corePool;

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

  this->num_cores = (idx) corePool.size();

  #if defined(TESTING)
  //Test
  printf( "Number of cores: %d\n", this->num_cores );
  int counter = 0;
  for ( auto it = corePool.begin() ; it != corePool.end(); it++) {
      char* filename = new char[ 10 ];
      sprintf( filename, "core%d.xyz", counter++ );
      FILE* fp = fopen( filename, "w" );
      core = *it;
      int numAtoms = core->num_tails * this->tail_length;
      fprintf( fp, "%d\nAtoms. Timestep: 4300\n", numAtoms );
      core->printCore( fp );
      delete[] filename;
      fclose( fp );
  }
  #endif

  this->deriveStems();

  #if defined(TESTING)
  //Test
  printf( "Number of linked cores (aka stems): %lu\n", this->stems.size() );
  /*for ( auto it = this->stems.begin(); it != this->stems.end(); it++) {
    printf( "Stem %lu: %hu\n", it->first, it->second->count );
  }*/
  #endif

  TadMicelle* micelle = NULL;
  Stem* currentStem = NULL;

  if ( this->stems.size() == 0 ) {
    for ( auto it = corePool.begin(); it != corePool.end(); it++ ) {
      micelle = new TadMicelle();
      micelle->addCore( *it );

      micelle->deriveChainList();

      this->micelleList.push_back( micelle );
    }
  } else {
    for ( auto it = this->stems.begin(); it != this->stems.end(); it++ ) {
      for ( auto stem = it->second.begin(); stem != it->second.end(); stem++ ) {
        currentStem = *stem;
        if ( !currentStem->grouped ) {
          currentStem->grouped = true;
          micelle = new TadMicelle();
          micelle->addCore( currentStem->core1 );
          micelle->addCore( currentStem->core2 );

          this->compareCore( currentStem->core1, micelle );
          this->compareCore( currentStem->core2, micelle );

          micelle->deriveChainList();

          this->micelleList.push_back( micelle );
        }
      }

  }

  }

  #if defined(TESTING)
  // Test
  printf( "Number of micelles: %lu\n", this->micelleList.size() );
  counter = 0;
  for ( auto it = this->micelleList.begin() ; it != this->micelleList.end(); it++) {
      char* filename = new char[ 13 ];
      sprintf( filename, "micelle%d.xyz", counter++ );
      FILE* fp = fopen( filename, "w" );
      micelle = *it;
      int numAtoms = micelle->chainList.size() * this->chain_length;
      fprintf( fp, "%d\nAtoms. Timestep: 4300\n", numAtoms );
      micelle->printMicelle( fp );
      delete[] filename;
      fclose( fp );
  }
  #endif

}

bool TadFrame::areAllFilledBinsGrouped() {
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

int TadFrame::numFilledBinsArentGrouped() {
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

void TadFrame::fillBins() {
  Bin* current = NULL;
  EndpointChain* chain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = ( EndpointChain* ) this->chainList[ i ];

    for (byte i = 0; i < chain->numTails; i++) {
      current = this->binBlock(chain->tails[i]);
      if ( !current ) {
        fprintf( stdout, "Tail1 in chain %d outside of bins.\n", i );
        exit( 1 );
      }
      current = NULL;
    }
  }
}

void TadFrame::correctFrame() {
  TadMicelle* micelle = NULL;
  for ( auto it = this->micelleList.begin(); it != this->micelleList.end(); it++ ) {
    micelle = *it;
    micelle->pbcCorrectMicelle( &(this->box_length), this->pbc_correction_factor );
    for ( auto core = micelle->coreList.begin(); core != micelle->coreList.end(); core++ ) {
      ( *core )->calcCenterOfMass( &(this->box_length), this->pbc_correction_factor );
    }
  }
}

void TadFrame::colorChains() {
  EndpointChain* chain = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    chain = ( EndpointChain* ) this->chainList[ i ];
    if ( chain->config == neither || chain->config == stem )
      chain->colorChain( STEM );
    else
      chain->colorChain( PETAL );
  }
}

void TadFrame::colorCores() {
  idx coreCounter = 1;
  TadMicelle* micelle;
  for ( auto it = micelleList.begin(); it != micelleList.end(); it++ ) {
    micelle = ( *it );
    for ( auto core = micelle->coreList.begin(); core != micelle->coreList.end(); core++ ) {
      ( *core )->color( coreCounter++ );
    }
  }
}

void TadFrame::process() {
  this->correctFrame();
  this->calcChainConfigFractions();
  this->calcAvgAggNum();
  this->calcAvgDistBtwnCores();
}

void TadFrame::calcChainConfigFractions() {
  this->percent_stem_chains = 0.0f;
  this->percent_petal_chains = 0.0f;
  EndpointChain* current = NULL;
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
    current = (EndpointChain*) this->chainList[ i ];
    switch( current->config ) {
      case neither:
        this->percent_stem_chains++;
        break;
      case petal:
        this->percent_petal_chains++;
        break;
      case stem:
        this->percent_stem_chains++;
        break;
      default:
        fprintf( stdout, "Error invalid chain configuration.\n" );
        exit( 1 );
    } 
  }
  this->percent_stem_chains /= this->num_chains;
  this->percent_petal_chains /= this->num_chains;
}

void TadFrame::calcAvgAggNum() {
  this->avg_agg_number = 0.0f;
  float numCores = 0.0f;
  for( auto it = this->micelleList.begin(); it != this->micelleList.end(); it++ ) {
    for ( auto core = ( *it )->coreList.begin(); core != ( *it )->coreList.end(); core++ ) {
      this->avg_agg_number += ( *core )->aggregation_num;
      numCores++; 
    }
  }
  this->avg_agg_number /= numCores;
}

void TadFrame::calcAvgDistBtwnCores() {
  HydrophobicCore* c1 = NULL;
  HydrophobicCore* c2 = NULL;
  Stem* currentStem = NULL;
  this->avg_distance_btwn_cores = 0.0;
  for ( auto it = this->stems.begin(); it != this->stems.end(); it++ ) {
    for ( auto stem = it->second.begin(); stem != it->second.end(); stem++ ) {
      currentStem = *stem;
      c1 = currentStem->core1;
      c2 = currentStem->core2;
      this->avg_distance_btwn_cores += sqrt( c1->com->getLinkedDist( c2->com, &(this->box_length), 
                                             this->pbc_correction_factor ) ); 
    }
  }
  if ( this->avg_distance_btwn_cores != 0.0 ) {
    this->avg_distance_btwn_cores /= this->stems.size();
  }
}

TadFrame::~TadFrame() {
  for ( auto stemList = this->stems.begin(); stemList != this->stems.end(); stemList++ ) {
    for ( auto stem = stemList->second.begin(); stem != stemList->second.end(); stem++ ) {
      delete *stem;
      *stem = NULL;
    }
  }
  for ( auto micelle = std::begin( this->micelleList ); micelle != std::end( this->micelleList ); micelle++ ) {
    delete *micelle;
    *micelle = NULL;
  }
}

void TadFrame::printChains( FILE* fp ) {
  for ( unsigned short i = 0; i < this->num_chains; i++ ) {
      #if defined(TESTING)
      fprintf( fp, "printing chain %d\n", i );
      #endif
      this->chainList[ i ]->printChain( fp );
  }
}

void TadFrame::printCores( FILE* fp ) {
  for ( auto it = this->micelleList.begin(); it != this->micelleList.end(); it++ ) {
    ( *it )->printMicelleCore( fp );
  }
}

void TadFrame::printBins( FILE* fp ) {
  for ( idx i = 0; i < this->num_bins; i++ ) {
    for ( idx j = 0; j < this->num_bins; j++ ) {
      for ( idx k = 0; k < this->num_bins; k++ ) {
        fprintf( fp, "printing bin %d %d %d\n", (int) i, (int) j, (int) k );
        this->box[ i ][ j ][ k ]->printBin( fp );
      }
    }    
  }
}

void TadFrame::printData( FILE* fp ) {
  fprintf( fp, "%10d %10.4f %10.4f %10.4f %10.4f\n", (int) this->num_cores, this->avg_agg_number,
           this->avg_distance_btwn_cores, this->percent_stem_chains, this->percent_petal_chains );
}

TadFrameData::TadFrameData( TadFrame* f ) {
  this->avg_agg_number = f->avg_agg_number;
  this->percent_petal_chains = f->percent_petal_chains;
  this->percent_stem_chains = f->percent_stem_chains;
  this->num_cores = f->num_cores;
  this->avg_distance_btwn_cores = f->avg_distance_btwn_cores;
} 
/******************************* TadFrame ******************************************/

const unsigned short lastFrames = 2000;

TadTrajectory::TadTrajectory() : DPDTrajectory() {
  unsigned int temp;

  std::cout << "Enter number of tails per chain: ";
  std::cin >> temp;
  this->chain_length = (idx) temp;

  std::cout << "Enter micelle cutoff: ";
  std::cin >> this->micelle_cutoff;

  std::cout << "Enter PBC correction factor: ";
  std::cin >> this->pbc_correction_factor;

  this->AVG_avg_agg_number = 0.0f;
  this->STDDEV_avg_agg_number = 0.0f;
  this->AVG_percent_stem_chains = 0.0f;
  this->STDDEV_percent_stem_chains = 0.0f;
  this->AVG_percent_petal_chains = 0.0f;
  this->STDDEV_percent_petal_chains = 0.0f;
  this->AVG_num_cores = 0.0f;
  this->STDDEV_num_cores = 0.0f;
  this->AVG_avg_distance_btwn_cores = 0.0;
  this->STDDEV_avg_distance_btwn_cores = 0.0;

}

TadTrajectory::~TadTrajectory() {
  for ( auto it = this->frameData.begin(); it != this->frameData.end(); it++ ) {
    delete *it;
  }
}

void TadTrajectory::process() {
  unsigned int frameCount = 0;
  unsigned int filePtr = 0;
  std::string line;

  FILE* output = fopen( this->outFile.c_str(), "w" );
  this->setupOutputFile( output );

  std::cout << "Processing trajectory..." << std::endl;

  while ( filePtr < this->numFiles ) {

    std::ifstream inFile( this->fileNames[ filePtr ].c_str() );
    filePtr++;

    while ( std::getline( inFile, line ) ) {
      std::getline( inFile, line );
      if ( frameCount % 10 == 0 ) {
        fprintf( output, "%4d ", ( frameCount / 10 ) );
        this->analyze( inFile, output );
      } else
        this->skip( inFile );
      frameCount++;
    }

    inFile.close();

  }

  fclose( output );

  std::cout << "Trajectory processing complete." << std::endl;

  this->calcData();

}

void TadTrajectory::setupHelp( FILE* fp ) {
  fprintf( fp, "   Cores     AvgAgg     AvgDistCores     Stem     Petal\n" );
}

void TadTrajectory::analyzeHelp( std::ifstream& inFile, FILE* output ) {

  TadFrame* tframe = new TadFrame( this->num_atoms, this->box_length, this->chain_length, 
                                             this->bin_length, &( this->micelle_cutoff ),
                                             ( const float ) this->pbc_correction_factor, &inFile );

  tframe->deriveMicelleList();
  tframe->process();

  tframe->printData( output );

  TadFrameData* data = new TadFrameData( tframe );
  this->frameData.push_back( data );

  if ( this->framesAnalyzed + 1 >= ( lastFrames / 10 ) ) {
    FILE* fp;
    if ( this->framesAnalyzed + 1 == ( lastFrames / 10 ) )
      fp = fopen( "lastFrameWrapped.xyz", "w" );
    else
      fp = fopen( "lastFrameWrapped.xyz", "a" );

    if ( fp == NULL ) {
      fprintf( stdout, "Error in opening lastFrameWrapped.xyz...\n" );
      exit( 1 );
    }

    std::cout << "Printing frame " << ( this->framesAnalyzed + 1 ) << " ..." << std::endl;

    fprintf( fp, "%d\nTime: 1\n", tframe->num_atoms );
    tframe->printChains( fp );

    fclose( fp );
  }

  delete tframe;

  this->AVG_avg_agg_number += data->avg_agg_number;
  this->AVG_percent_petal_chains += data->percent_petal_chains;
  this->AVG_percent_stem_chains += data->percent_stem_chains;
  this->AVG_num_cores += data->num_cores;
  this->AVG_avg_distance_btwn_cores += data->avg_distance_btwn_cores;
}

void TadTrajectory::calcHelp() {

  if ( this->framesAnalyzed != this->frameData.size() ) {
    fprintf( stdout, "Error in frame data and number analyzed." );
    exit( 1 );
  }

  this->AVG_avg_agg_number /= this->framesAnalyzed;
  this->AVG_percent_petal_chains /= this->framesAnalyzed;
  this->AVG_percent_stem_chains /= this->framesAnalyzed;
  this->AVG_num_cores /= this->framesAnalyzed; 
  this->AVG_avg_distance_btwn_cores /= this->framesAnalyzed;

  this->setupHelp( stdout );
  fprintf( stdout, "AVG: %8.4f %8.4f %8.4f %8.4f %8.4f\n", this->AVG_num_cores, this->AVG_avg_agg_number, 
           this->AVG_avg_distance_btwn_cores, this->AVG_percent_stem_chains, this->AVG_percent_petal_chains );

  for ( auto it = this->frameData.begin(); it != this->frameData.end(); it++ ) {
    this->STDDEV_num_cores += ( this->AVG_num_cores - (*it)->num_cores )*( this->AVG_num_cores - (*it)->num_cores );
    this->STDDEV_percent_petal_chains += ( this->AVG_percent_petal_chains - (*it)->percent_petal_chains )*( this->AVG_percent_petal_chains - (*it)->percent_petal_chains );
    this->STDDEV_percent_stem_chains += ( this->AVG_percent_stem_chains - (*it)->percent_stem_chains )*( this->AVG_percent_stem_chains - (*it)->percent_stem_chains );
    this->STDDEV_avg_agg_number += ( this->AVG_avg_agg_number - (*it)->avg_agg_number )*( this->AVG_avg_agg_number - (*it)->avg_agg_number );
    this->STDDEV_avg_distance_btwn_cores += ( this->AVG_avg_distance_btwn_cores - (*it)->avg_distance_btwn_cores )*( this->AVG_avg_distance_btwn_cores - (*it)->avg_distance_btwn_cores );
  }

  this->STDDEV_avg_agg_number /= this->framesAnalyzed;
  this->STDDEV_percent_petal_chains /= this->framesAnalyzed;
  this->STDDEV_percent_stem_chains /= this->framesAnalyzed;
  this->STDDEV_num_cores /= this->framesAnalyzed; 
  this->STDDEV_avg_distance_btwn_cores /= this->framesAnalyzed;

  this->STDDEV_avg_agg_number = sqrt( this->STDDEV_avg_agg_number ); 
  this->STDDEV_percent_petal_chains = sqrt( this->STDDEV_percent_petal_chains ); 
  this->STDDEV_percent_stem_chains = sqrt( this->STDDEV_percent_stem_chains ); 
  this->STDDEV_num_cores = sqrt( this->STDDEV_num_cores );  
  this->STDDEV_avg_distance_btwn_cores = sqrt( this->STDDEV_avg_distance_btwn_cores ); 

  fprintf( stdout, "STDDEV: %8.4f %8.4f %8.4f %8.4f %8.4f\n", this->STDDEV_num_cores, this->STDDEV_avg_agg_number, 
           this->STDDEV_avg_distance_btwn_cores, this->STDDEV_percent_stem_chains, this->STDDEV_percent_petal_chains );
}

int main() {

  TadTrajectory* traj = new TadTrajectory();

  traj->process();

  delete traj;

  return 0;
}
