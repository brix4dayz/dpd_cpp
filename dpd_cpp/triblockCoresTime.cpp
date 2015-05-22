#include "triblockCoresTime.h"

void TriblockCoresTime::analyzeHelp( std::ifstream& inFile, FILE* fp ) {
  TriblockFrame* tframe = new TriblockFrame( this->num_atoms, this->box_length, this->chain_length, 
                                             this->bin_length, &( this->micelle_cutoff ),
                                             ( const float ) this->pbc_correction_factor,  this->tail_length, this->pec_length, &inFile );
  tframe->deriveMicelleList();
 
  fprintf( fp, "%10d\n", (int) tframe->num_cores );

  delete tframe;

}

void TriblockCoresTime::setupHelp( FILE* fp ) {
  fprintf( fp, "Frame #   Cores     \n" );
}

#if defined(USING)

int main () {

  std::unique_ptr<TriblockCoresTime> traj( new TriblockCoresTime() );

  if (traj)
    traj->process();

  traj.reset();

  return 0;
}

#endif