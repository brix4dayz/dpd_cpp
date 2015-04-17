#include "dpdTrajectory.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

DPDTrajectory::DPDTrajectory() {
    unsigned int temp = 0;
    this->numFiles = 0;
    this->num_atoms = 0;
    this->box_length = 0;
    this->bin_length = 0;
    this->numFrames = 0;
    this->startFile = 0;
    this->startFrameOffset = 0;
    this->framesAnalyzed = 0;

    std::cout << "Enter box length: ";
    std::cin >> temp;
    this->box_length = ( idx ) temp;

    std::cout << "Enter bin size: ";
    std::cin >> temp;
    this->bin_length = ( idx ) temp;

    std::cout << "Enter number of files: ";
    std::cin >> this->numFiles;

    this->fileNames = new std::string[ this->numFiles ];

    for ( unsigned int i = 0; i < this->numFiles; i++ ) {
      std::cout << "Enter trajectory " << i << " filename: \n";
      std::cin >> this->fileNames[ i ];
    }

    std::cout << "Enter main output file name: " << std::endl;
    std::cin >> this->outFile;

    this->determineNumFrames();
}

DPDTrajectory::~DPDTrajectory() {
  delete[] this->fileNames;
}

void DPDTrajectory::unlink() {}

void DPDTrajectory::determineNumFrames() {
  srand(time(NULL));

  int randFile = rand() % this->numFiles;
  std::cout << "Reading frame from file" << randFile << "..." << std::endl;

  std::string line;
  std::ifstream infile( this->fileNames[ randFile ].c_str() );

  unsigned long bytesPerFrame = 0;

  std::getline( infile, line );

  this->num_atoms = atoi( line.c_str() );

  bytesPerFrame += line.length() + 1; // + 1 for extra byte in newline that is lost when \n is replaced with \0

  for ( unsigned int i = 0; i < this->num_atoms + 1; i++ ) {
    std::getline( infile, line );
    bytesPerFrame += line.length() + 1;
  }

  infile.close();

  unsigned long* bytesInEachFile = new unsigned long[ numFiles ];
  unsigned long bytesInTraj = 0;
  for ( unsigned int i = 0; i < this->numFiles; i++ ) {
    bytesInEachFile[ i ] = this->numBytesInFile( this->fileNames[ i ] );
    bytesInTraj += bytesInEachFile[ i ];
  }

  std::cout << "Estimated bytes Per Frame: " << bytesPerFrame << std::endl;

  std::cout << "Bytes In Traj: " << bytesInTraj << std::endl;

  this->numFrames = bytesInTraj / bytesPerFrame;

  // Round to the nearest 100
  if ( this->numFrames >= 1000 ) {
    this->numFrames /= 100;
    this->numFrames *= 100;
  }

  std::cout << "Number of atoms: " << this->num_atoms << std::endl;

  std::cout << "Estimated number of frames: " << this->numFrames << std::endl;

  // calc start file and offset
  if ( this->numFrames > 1000 ) {
    this->startFrameOffset = this->numFrames - 1000;
    unsigned int frames = 0;
    for ( unsigned int i = 0; i < this->numFiles; i++ ) {
      frames += bytesInEachFile[ i ]/bytesPerFrame;
      if ( this->startFrameOffset < frames ) {
        this->startFrameOffset = this->startFrameOffset - ( frames - bytesInEachFile[ i ]/bytesPerFrame );
        this->startFile = i;
        break;
      }
    }
  } else {
    this->startFrameOffset = 0;
    this->startFile = 0;
  }

  delete[] bytesInEachFile;

  std::cout << "Start file: " << this->fileNames[ this->startFile ] << std::endl;

  std::cout << "Frame offset: " << this->startFrameOffset << std::endl;

  std::cout << "Box length: " << (int) this->box_length << std::endl;

  std::cout << "Bin size: " << (int) this->bin_length << std::endl;

}

void DPDTrajectory::setupOutputFile( FILE* fp ) {
  std::cout << "Setting up outfile..." << std::endl;
  this->setupHelp( fp );
  std::cout << "Output file initialized." << std::endl;
}

void DPDTrajectory::process() {
  unsigned int frameCount = 0;
  unsigned int filePtr = this->startFile;
  std::string line;

  FILE* output = fopen( this->outFile.c_str(), "w" );
  this->setupOutputFile( output );

  std::cout << "Processing trajectory..." << std::endl;

  while ( filePtr < this->numFiles ) {

    std::ifstream inFile( this->fileNames[ filePtr ].c_str() );
    filePtr++;

    if ( this->numFrames > 100 ) {
      while ( std::getline( inFile, line ) ) {
        std::getline( inFile, line );
        if ( frameCount >= this->startFrameOffset && ( frameCount - this->startFrameOffset ) % 10 == 0 )
          this->analyze( inFile, output );
        else
          this->skip( inFile );
        frameCount++;
      }
    } else {
      while ( std::getline( inFile, line ) ) {
        std::getline( inFile, line );
        this->analyze( inFile, output );
      }
    }

    inFile.close();

  }

  fclose( output );

  std::cout << "Trajectory processing complete." << std::endl;

  this->calcData();

}

void DPDTrajectory::analyze( std::ifstream& inFile, FILE* fp ) {
  this->analyzeHelp( inFile, fp );
  std::cout << "Analyzed " << ++this->framesAnalyzed << " frames..." << std::endl;
}

void DPDTrajectory::skip( std::ifstream& inFile ) {
  std::string line;
  for ( unsigned int i = 0; i < this->num_atoms; i++ ) {
    std::getline( inFile, line );
  }
}

void DPDTrajectory::calcData() {
  std::cout << "Calculating final trajectory data..." << std::endl;
  this->calcHelp();
}

// source: http://www.cplusplus.com/reference/cstdio/ftell/
unsigned long DPDTrajectory::numBytesInFile( std::string filename ) {
  FILE * pFile;
  unsigned long size = 0;

  pFile = fopen( filename.c_str(),"rb" );
  if ( pFile == NULL) perror ( "Error opening file" );
  else {
    fseek( pFile, 0, SEEK_END );   // non-portable
    size = ftell( pFile );
    fclose( pFile );
  }
  return size;
}

TriblockTrajectory::TriblockTrajectory() : DPDTrajectory() {
  int temp;

  std::cout << "Enter tail length: ";
  std::cin >> temp;
  this->tail_length = (idx) temp;

  std::cout << "Enter pec length: ";
  std::cin >> temp;
  this->pec_length = (idx) temp;

  this->chain_length = this->pec_length + 2*this->tail_length;

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

TriblockTrajectory::~TriblockTrajectory() {
  for ( auto it = this->frameData.begin(); it != this->frameData.end(); it++ ) {
    delete *it;
  }
}

void TriblockTrajectory::setupHelp( FILE* fp ) {
  fprintf( fp, "   Cores     AvgAgg     AvgDistCores     Stem     Petal\n" );
}

void TriblockTrajectory::analyzeHelp( std::ifstream& inFile, FILE* fp ) {
  TriblockFrame* tframe = new TriblockFrame( this->num_atoms, this->box_length, this->chain_length, 
                                             this->bin_length, &( this->micelle_cutoff ),
                                             ( const float ) this->pbc_correction_factor,  this->tail_length, this->pec_length, &inFile );
  tframe->deriveMicelleList();
  tframe->process();

  tframe->printData( fp );

  TriblockFrameData* data = new TriblockFrameData( tframe );
  this->frameData.push_back( data );

  if ( this->framesAnalyzed + 1 >= 100 ) {
    FILE* fp;
    if ( this->framesAnalyzed + 1 == 100 )
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
  }

  delete tframe;

  this->AVG_avg_agg_number += data->avg_agg_number;
  this->AVG_percent_petal_chains += data->percent_petal_chains;
  this->AVG_percent_stem_chains += data->percent_stem_chains;
  this->AVG_num_cores += data->num_cores;
  this->AVG_avg_distance_btwn_cores += data->avg_distance_btwn_cores;
}

void TriblockTrajectory::calcHelp() {

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

  TriblockTrajectory* traj = new TriblockTrajectory();

  traj->process();

  delete traj;

  return 0;
}