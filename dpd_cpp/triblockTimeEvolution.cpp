#include "triblockTimeEvolution.h"

TriblockTimeEvolution::TriblockTimeEvolution() : TriblockTrajectory() {}

void TriblockTimeEvolution::process() {
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

void TriblockTimeEvolution::analyzeHelp( std::ifstream& inFile, FILE* fp ) {
  TriblockFrame* tframe = new TriblockFrame( this->num_atoms, this->box_length, this->chain_length, 
                                             this->bin_length, &( this->micelle_cutoff ),
                                             ( const float ) this->pbc_correction_factor,  this->tail_length, this->pec_length, &inFile );
  tframe->deriveMicelleList();
  tframe->process();

  tframe->printData( fp );

  delete tframe;

}

void TriblockTimeEvolution::calcHelp() {}

void TriblockTimeEvolution::setupHelp( FILE* fp ) {
  fprintf( fp, "Frame #   Cores     AvgAgg     AvgDistCores     Stem     Petal\n" );
}

TriblockTimeEvolution::~TriblockTimeEvolution() {}

#if defined(USING)
#include <memory>

int main() {
  
  std::unique_ptr<TriblockTimeEvolution> traj( new TriblockTimeEvolution() );

  if (traj)
    traj->process();

  traj.reset();

  return 0;
}

#endif